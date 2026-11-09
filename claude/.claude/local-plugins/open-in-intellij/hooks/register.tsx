import type { Register } from 'claude-code'

type Target = { path: string; line?: number; relative?: string }

// [label](href) — href without spaces/parens, optionally wrapped in <...>
const LINK = /\[([^\]\n]*)\]\(<?([^()\s<>]+)>?\)/g
const SCHEME = /^[a-z][a-z0-9+.-]*:/i

function parseTarget(href: string, base: string): Target | undefined {
  let raw = href
  if (raw.startsWith('file://')) raw = raw.slice('file://'.length)
  else if (SCHEME.test(raw) || raw.startsWith('#')) return undefined

  let path = raw
  let line: number | undefined
  const hash = /#L(\d+)(?:-L?\d+)?$/.exec(path)
  const colon = /:(\d+)(?::\d+)?$/.exec(path)
  if (hash) {
    line = Number(hash[1])
    path = path.slice(0, hash.index)
  } else if (colon) {
    line = Number(colon[1])
    path = path.slice(0, colon.index)
  }
  try {
    path = decodeURIComponent(path)
  } catch {}
  if (!path) return undefined
  if (path.startsWith('~/')) return undefined // home-relative: leave to the app
  if (path.startsWith('/')) return { path, line }
  const relative = path.replace(/^\.\//, '')
  return { path: `${base.replace(/\/$/, '')}/${relative}`, line, relative }
}

type Process = { run: (argv: string[]) => Promise<{ exitCode: number; stdout: string; stderr: string }> }
type Engine = { fs: { exists: (path: string) => Promise<boolean> }; process: Process; ui: { toast: (text: string) => void } }

const trackedFiles = new Map<string, Promise<string[]>>()

function listTrackedFiles($: Engine, root: string): Promise<string[]> {
  let files = trackedFiles.get(root)
  if (!files) {
    files = $.process.run(['git', '-C', root, 'ls-files']).then(({ exitCode, stdout }) =>
      exitCode === 0 ? stdout.split('\n').filter(Boolean) : [],
    )
    trackedFiles.set(root, files)
  }
  return files
}

// A relative link written from a subfolder (e.g. after a `cd`) doesn't exist under the root: match it as a suffix of a tracked file.
async function resolvePath($: Engine, target: Target, root: string, cwd: string): Promise<string | undefined> {
  if (!target.relative || (await $.fs.exists(target.path))) return target.path
  const suffix = `/${target.relative}`
  const matches = (await listTrackedFiles($, root)).filter(file => `/${file}`.endsWith(suffix))
  if (matches.length === 0) {
    $.ui.toast(`No file matches ${target.relative}`)
    return undefined
  }
  const cwdPrefix = cwd.startsWith(`${root}/`) ? `${cwd.slice(root.length + 1)}/` : ''
  const preferred = matches.length === 1 ? matches : matches.filter(file => cwdPrefix && file.startsWith(cwdPrefix))
  if (preferred.length !== 1) {
    $.ui.toast(`${matches.length} files match ${target.relative}`)
    return undefined
  }
  return `${root}/${preferred[0]}`
}

export const register: Register = on => {
  on('ui.render', { component: 'AssistantMessage' }, async ($, e, next) => {
    if (e.surface !== 'desktop') return next(e)

    const text = e.props.text
    if (!text.includes('](')) return next(e)

    const cwd = await $.session.cwd()
    const git = await $.process.run(['git', '-C', cwd, 'rev-parse', '--show-toplevel'])
    const root = git.exitCode === 0 && git.stdout.trim() ? git.stdout.trim() : cwd
    const targets = new Map<string, Target>()
    const rewritten = text.replace(LINK, (whole, label: string, href: string) => {
      const target = parseTarget(href, root)
      if (!target) return whole
      // Markdown only draws http/https/file links, so point it at file://; we answer the press.
      const drawn = `file://${encodeURI(target.path)}${target.line ? `#L${target.line}` : ''}`
      targets.set(drawn, target)
      return `[${label}](${drawn})`
    })
    if (targets.size === 0) return next(e)

    const { Markdown } = $.ui.resolve(e)
    return (
      <Markdown
        key={`msg-${e.requestId}`}
        text={rewritten}
        pressableLinks={[...targets.keys()].slice(0, 256)}
        onLinkPress={async link => {
          const target = targets.get(link.href)
          if (!target) return
          const path = await resolvePath($, target, root, cwd)
          if (!path) return
          const url =
            `idea://open?file=${encodeURIComponent(path)}` +
            (target.line ? `&line=${target.line}` : '')
          const { exitCode, stderr } = await $.process.run(['/usr/bin/open', url])
          if (exitCode !== 0) $.ui.toast(`Could not open in IntelliJ: ${stderr.trim()}`)
        }}
      />
    )
  })
}
