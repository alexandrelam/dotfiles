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




