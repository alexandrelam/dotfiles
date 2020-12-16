# Dotfiles for Ubuntu

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




