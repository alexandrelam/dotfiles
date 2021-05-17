#!/bin/sh

# to run on startup use /etc/rc.local
# search for script sudo on startup
# https://askubuntu.com/questions/889632/startup-script-with-sudo-in-ubuntu-16-10

sudo modprobe zram num_devices=4

# set algorithm
sudo echo lzo > /sys/block/zram0/comp_algorithm
sudo echo lzo > /sys/block/zram1/comp_algorithm
sudo echo lzo > /sys/block/zram2/comp_algorithm
sudo echo lzo > /sys/block/zram3/comp_algorithm

# set disksize
sudo echo 3G > /sys/block/zram0/disksize
sudo echo 3G > /sys/block/zram1/disksize
sudo echo 3G > /sys/block/zram2/disksize
sudo echo 3G > /sys/block/zram3/disksize

# turn into swap
sudo mkswap /dev/zram0
sudo swapon /dev/zram0

sudo mkswap /dev/zram1
sudo swapon /dev/zram1

sudo mkswap /dev/zram2
sudo swapon /dev/zram2

sudo mkswap /dev/zram3
sudo swapon /dev/zram3
