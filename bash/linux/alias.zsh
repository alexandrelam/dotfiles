alias homeserver="ssh alex@95.217.210.24"
alias lg=lazygit
alias capture='grim -g "$(slurp)" - | swappy -f -'
alias sd='shutdown now'
alias update='sudo timeshift --check && yay -Syu --noconfirm'

alacritty-theme() {
  local config="${ALACRITTY_CONFIG:-$HOME/.alacritty.toml}"
  local light_theme="catppuccin-latte.toml"
  local dark_theme="catppuccin-mocha.toml"
  local target_theme
  local current_theme

  if [[ -L "$config" ]]; then
    config="$(readlink -f "$config")"
  fi

  if [[ ! -f "$config" ]]; then
    echo "Alacritty config not found: $config" >&2
    return 1
  fi

  current_theme="$(grep -E '^[[:space:]]*"~/.config/alacritty/[^"]+"' "$config" | head -n 1)"

  case "${1:-toggle}" in
    light)
      target_theme="$light_theme"
      ;;
    dark)
      target_theme="$dark_theme"
      ;;
    toggle)
      if [[ "$current_theme" == *"$light_theme"* ]]; then
        target_theme="$dark_theme"
      else
        target_theme="$light_theme"
      fi
      ;;
    *)
      echo "Usage: alacritty-theme [light|dark|toggle]" >&2
      return 1
      ;;
  esac

  sed -i "s|^[[:space:]]*\"~/.config/alacritty/.*\"|   \"~/.config/alacritty/$target_theme\"|" "$config"
  echo "Alacritty theme set to ${target_theme%.toml}"
}

alias alight='alacritty-theme light'
alias adark='alacritty-theme dark'
alias atoggle='alacritty-theme toggle'
