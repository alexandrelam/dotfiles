#!/bin/bash

setxkbmap -layout fr,us
setxkbmap -option 'grp:win_space_toggle'

xinput set-prop "Logitech G300s Optical Gaming Mouse" "libinput Accel Speed" -0.7
