#!/usr/bin/env bash

# Terminate already running bar instances
killall -q polybar

polybar bar >>/tmp/bar.log 2>&1 & disown

echo "Bars launched..."
