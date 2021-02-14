# Dotfiles for Ubuntu

![rice](https://user-images.githubusercontent.com/25727549/102390566-d8550800-3fd4-11eb-8908-bb47aebe6b85.png)

## Packages

```
sudo apt install i3 nitrogen rofi polybar
```

## Add fonts

```
# copy fonts to local fonts dir (i'll put the fonts in all dirs)
cp -r fonts/* ~/.local/share/fonts

# reload font cache
fc-cache -v

# delete current font config (to be able to display bitmap fonts)
sudo rm /etc/fonts/conf.d/70-no-bitmaps.conf
```

> Polybar style --> Feather

## Source

Polybar theme : [https://github.com/adi1090x/polybar-themes](https://github.com/adi1090x/polybar-themes)

## NeoVim

coc install
`:CocInstall coc-tsserver coc-json coc-html coc-css coc-vetur coc-git coc-java coc-python coc-prettier`

## Vim raccourci

| raccourci | action                       |
| --------- | ---------------------------- |
| , ,       | emmet autocomplete html tags |
| ctrl p    | fzf find file in dir         |
| gj        | previous buffer              |
| gk        | next buffer                  |
| gx        | delete buffer                |
| gd        | jump to definition           |
| space a   | linting info                 |
| ff        | format                       |

### Installation de Vim-Plug

```
curl -fLo ~/.vim/autoload/plug.vim --create-dirs \
    https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
```

## Settings

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

#### Imwheel

1. `sudo apt install imwheel`
2. Download `mousewheel.sh`
3. `chmod +x mousewheel.sh`
4. `./mousewheel.sh`
5. Add `mousewheel.sh` to startup

## Setup Arch Nvidia driver hybrid

https://www.youtube.com/watch?v=OlIXQRpfJQ4

install `optimus-manager-qt` and `bbswitch`

## Sur windows10

Alt-Resize window : https://stefansundin.github.io/altdrag/

## Cool website

- Free illustration for app : https://2.flexiple.com/scale/all-illustrations
- List of illustrations :https://dev.to/davidepacilio/50-free-tools-and-resources-to-create-awesome-user-interfaces-1c1b
- Another list of illustration websites : https://dev.to/theme_selection/best-design-resources-websites-every-developer-should-bookmark-1p5d

```

```
