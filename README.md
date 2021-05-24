# Dotfiles

> My i3-gaps setup on Arch with rofi, polybar, nitrogen and NeoVim !

![arch](https://user-images.githubusercontent.com/25727549/119369789-dc3c3300-bcb4-11eb-8cf9-88492803768a.png)

## Packages

```
i3-gaps nitrogen rofi polybar stow
```

Create all the symlinks with STOW !

Delete the `README.md` and `.git` folder and execute `stow *`

> Polybar style --> blocks (gruvbox theme)

> Rofi style --> colorful (style_5)

## Source

Polybar theme : [https://github.com/adi1090x/polybar-themes](https://github.com/adi1090x/polybar-themes)

Rofi theme : [https://github.com/adi1090x/rofi](https://github.com/adi1090x/rofi)

j# i3 raccourci

| raccourci      | action                                   |
| -------------- | ---------------------------------------- |
| win+hjkl       | changer de focus entre les fenetres      |
| win+1234       | swap entre les bureaux virtuels          |
| win+q          | fermer une fenetre                       |
| win+entre      | ouvrir un nouveau terminal               |
| win+c          | prochaine fenetre ouverte en horizontale |
| win+v          | prochaine fenetre ouverte en verticale   |
| win+r          | resize mode                              |
| win+shift+r    | reload settings                          |
| win+i          | rofi menu                                |
| win+o          | rofi opened windows                      |
| win+space      | swap between fr and us keyboard layout   |
| win+rightclick | resize window                            |

## i3 programmes utiles

- neovim
- arandr
- stow
- ranger

## NeoVim

coc install
`:CocInstall coc-tsserver coc-json coc-html coc-css coc-vetur coc-git coc-java coc-python coc-prettier`

## Picom

if using picom compositor remove transparency in /etc/xdg/picom.conf --> inactive-opacity=1

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
| space c   | coc commands                 |
| ff        | format                       |
| m         | create bookmark              |
| ù         | jump to bookmark             |

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
