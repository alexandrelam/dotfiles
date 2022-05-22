#!/bin/bash
intern=eDP-1-1
extern=HDMI-0

if xrandr | grep "$extern disconnected"; then
    xrandr --output "$extern" --off --output "$intern" --auto
    brightness 1400
else
    xrandr --output "$intern" --off --output "$extern" --auto
fi