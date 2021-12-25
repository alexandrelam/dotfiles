# Dotfiles

Welcome to my linux dotfiles. It can be used on all distros but is best on Arch based distros.

![image](https://user-images.githubusercontent.com/25727549/130975604-0a3eabd2-67af-409d-bb37-790226f91abb.png)

## Table of content

- [How to install](#how-to-install%3F)
- [Neovim config](#neovim-config)
  - [VimPlug](#plugin-manager%3A-vimplug)
  - [Coc](#coc)
  - [shortcuts](#shortcuts)
- [i3](#i3)
- [Tmux](#tmux)
- [FAQ](#faq)
  - [What’s the vision for this config?](#what’s-the-vision-for-this-config%3F)
  - [Why use this config?](#why-use-this-config%3F)
  - [Is such a simple desktop really usable?](#is-such-a-simple-desktop-really-usable%3F)
  - [Why base it mainly on Arch?](#why-base-it-mainly-on-arch%3F)
  - [Where is polybar and rofi?](#where-is-polybar-and-rofi%3F)
- [Other](#other-settings)

## How to install?

**Get the following software:**

- stow
- nitrogen
- ❤️ i3
  - i3-wm
  - i3blocks
  - i3status
  - i3lock (optional)
  - i3-gaps (optional)
- alacritty (default terminal in i3)
- dmenu
- ❤️ neovim
- rofi (optional)
- polybar (optional)

**Clone the repo**

```
git clone https://github.com/alexandrelam/dotfiles
```

**Use stow to create symlinks**

e.g:

```
cd dotfiles
```

```
stow nvim
```

## Neovim config

### Plugin manager: VimPlug

**Linux VimPlug:**

```
sh -c 'curl -fLo "${XDG_DATA_HOME:-$HOME/.local/share}"/nvim/site/autoload/plug.vim --create-dirs \
       https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim'
```

**Windows VimPlug :**

```
md ~\AppData\Local\nvim\autoload
$uri = 'https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim'
(New-Object Net.WebClient).DownloadFile(
  $uri,
  $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath(
    "~\AppData\Local\nvim\autoload\plug.vim"
  )
)
```

Config location `AppData\Local\nvim\init.vim`

If no folder or file, you need to create it.

To get this config, replace `init.vim` with `win_init.vim` and rename the file

### Coc

I use coc for autocompletion, file explorer, eslint / prettier and whole bunch of things.

![hello](https://user-images.githubusercontent.com/25727549/130977821-390a5c38-8259-44ae-9adf-74c08165738b.gif)

![explorer](https://user-images.githubusercontent.com/25727549/130978148-e54a4b78-a525-4604-a3a8-7042a5891677.gif)

**⚠️ You need node to install coc**

Coc plugins are installed automatically. To install more use `CocInstall` or add package to `g:coc_global_extensions` in `plugins-config.vim`.

### Shortcuts

| shortcut | action                         |
| -------- | ------------------------------ |
| , ,      | emmet autocomplete html tags   |
| ctrl p   | fzf find file in dir           |
| gj       | previous buffer                |
| gk       | next buffer                    |
| gx       | delete buffer                  |
| gd       | jump to definition             |
| space a  | linting info                   |
| space c  | coc commands                   |
| ff       | format                         |
| fl       | eslint auto fix                |
| m        | create bookmark                |
| ù        | jump to bookmark               |
| space+f  | file explorer                  |
| space+do | propose fixes like auto import |

## I3

### Shortcuts

| shortcut       | action                                   |
| -------------- | ---------------------------------------- |
| win+hjkl       | changer de focus entre les fenetres      |
| win+1234       | swap entre les bureaux virtuels          |
| win+q          | fermer une fenetre                       |
| win+entre      | ouvrir un nouveau terminal               |
| win+b          | open bluetooth + volume app              |
| win+c          | prochaine fenetre ouverte en horizontale |
| win+v          | prochaine fenetre ouverte en verticale   |
| win+r          | resize mode                              |
| win+shift+r    | reload settings                          |
| win+i          | rofi menu                                |
| win+d          | d menu                                   |
| win+o          | rofi opened windows                      |
| win+space      | swap between fr and us keyboard layout   |
| win+rightclick | resize window                            |
| win+shift+c    | toggle floating window                   |
| win+shift+w    | tab layout                               |
| win+shift+e    | default split layout                     |
| win+shift+z    | logout                                   |

## Tmux

Very basic config for tmux!

- Remapped `ctrl-b` to `ctrl-q` bacause azerty layout keyboard
- Remapped vertical and horizontal split to `|` and `-`
- Mouse mode activated
- Removed auto rename
- Default pane number is 1
- Removed delay for vim escape

## FAQ

### What's the vision for this config?

The goal is to have a lightweight, lean yet effective desktop to work and have fun 😊.

### Why use this config?

It's home. And it's also lightweight enough for heavy workflow on 8go of ram (docker + node + chrome and hundreds of tabs).

### Is such a simple desktop really usable?

Yes I used this config daily for 5 months while working on frontend dev at Okarito.

### Why base it mainly on Arch?

I use Arch btw.

### Where is polybar and rofi?

I used it for a while but in the end I don't like it that much. I rather have i3 default status bar and dmenu. I don't really care about heavy ricing I just want the best software for my use.

## Other settings

### Gnome

When I'm not using i3, I'm using Gnome because Gnome is the best non tilingwm for me.

The following settings are settings I'm always enabling when I install Gnome:

#### Alt right click resize

```
gsettings set org.gnome.desktop.wm.preferences resize-with-right-button true
gsettings set org.gnome.desktop.wm.preferences mouse-button-modifier '<Alt>'
```

#### Fix alt-tab on gnome

```
gsettings set org.gnome.desktop.wm.keybindings switch-applications "[]"
gsettings set org.gnome.desktop.wm.keybindings switch-applications-backward "[]"
gsettings set org.gnome.desktop.wm.keybindings switch-windows "['<Alt>Tab', '<Super>Tab']"
gsettings set org.gnome.desktop.wm.keybindings switch-windows-backward "['<Alt><Shift>Tab', '<Super><Shift>Tab']"
```

### Other settings and info

#### Deactivate suspend on close lib laptop

Edit :

```
/etc/systemd/logind.conf
```

uncomment lines :

- `HandleLidSwitch` -> ignore
- `HandleLidSwitchExternalPower` -> ignore

apply changes :

```
systemctl kill -s HUP systemd-logind
```

#### Imwheel

1. `sudo apt install imwheel`
2. Download `mousewheel.sh`
3. `chmod +x mousewheel.sh`
4. `./mousewheel.sh`
5. Add `mousewheel.sh` to startup

#### Setup Arch Nvidia driver hybrid

https://www.youtube.com/watch?v=OlIXQRpfJQ4

install `optimus-manager-qt` and `bbswitch`

#### Sur windows10

Alt-Resize window : https://stefansundin.github.io/altdrag/

#### Cool website

- Free illustration for app : https://2.flexiple.com/scale/all-illustrations
- List of illustrations :https://dev.to/davidepacilio/50-free-tools-and-resources-to-create-awesome-user-interfaces-1c1b
- Another list of illustration websites : https://dev.to/theme_selection/best-design-resources-websites-every-developer-should-bookmark-1p5d

Polybar theme : [https://github.com/adi1090x/polybar-themes](https://github.com/adi1090x/polybar-themes)

Rofi theme : [https://github.com/adi1090x/rofi](https://github.com/adi1090x/rofi)
