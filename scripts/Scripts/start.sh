#!/bin/bash

setxkbmap -layout fr,us
setxkbmap -option 'grp:win_space_toggle'

xinput set-prop "Microsoft Bluetooth Mouse" "libinput Accel Speed" -0.6
xinput set-prop "ETPS/2 Elantech Touchpad" "libinput Tapping Enabled" 1
xinput set-prop "ETPS/2 Elantech Touchpad" "libinput Tapping Enabled Default" 1
xinput set-prop "ETPS/2 Elantech Touchpad" "libinput Natural Scrolling Enabled" 1
xinput set-prop "ETPS/2 Elantech Touchpad" "libinput Natural Scrolling Enabled Default" 1
