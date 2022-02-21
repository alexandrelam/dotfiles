#!/bin/bash

setxkbmap -layout fr,us
setxkbmap -option 'grp:win_space_toggle'

xinput set-prop "Logitech G300s Optical Gaming Mouse" "libinput Accel Speed" -0.8
xinput set-prop "ETPS/2 Elantech Touchpad" "libinput Tapping Enabled" 1
xinput set-prop "ETPS/2 Elantech Touchpad" "libinput Tapping Enabled Default" 1
xinput set-prop "ETPS/2 Elantech Touchpad" "libinput Natural Scrolling Enabled" 1
xinput set-prop "ETPS/2 Elantech Touchpad" "libinput Natural Scrolling Enabled Default" 1
