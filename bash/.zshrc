# make nvim the default text editor
export VISUAL=nvim
export EDITOR=nvim

# ZSH config
export ZSH="/home/alex/.oh-my-zsh"
ZSH_THEME="robbyrussell"
plugins=(git)
source $ZSH/oh-my-zsh.sh

# ls after cd
chpwd() ls




######################
# ALIASES            #
######################

alias updateDB='docker-compose run -T api bash -c "rails db:drop && rails db:create && pg_restore --verbose --clean --no-acl --no-owner -d \"\$DATABASE_URL\""  <'
alias chrome="google-chrome-stable --password-store=gnome"
alias cproxy="google-chrome-stable --password-store=gnome --proxy-server='10.100.1.4:9090'" # not working ...??
alias sudo="sudo "
alias vi="nvim"
alias vim="nvim"
alias wifi="nmcli d wifi"

alias gwta="git worktree add"
alias gwtl="git worktree list"
alias gwtr="git worktree remove"
alias gitsave="git config --global credential.helper store"
alias gitunsave="git config --global --unset credential.helper"

alias front="cd Documents/okarito-front"






######################
# PATH               #
######################

path+=/home/alex/.local/bin # python libraries




######################
# DEV                #
######################

# Add RVM to handle multiple version of ruby
[[ -s "$HOME/.rvm/scripts/rvm" ]] && source "$HOME/.rvm/scripts/rvm"
export PATH="$PATH:$HOME/.rvm/bin"

# nvm used to handle multiple version of nodejs
export NVM_DIR="$HOME/.nvm"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"  # This loads nvm
[ -s "$NVM_DIR/bash_completion" ] && \. "$NVM_DIR/bash_completion"  # This loads nvm bash_completion

# pyenv for multiple python version
export PYENV_ROOT="$HOME/.pyenv"
export PATH="$PYENV_ROOT/bin:$PATH"
eval "$(pyenv init --path)"

# Load pyenv into the shell by adding
# the following to ~/.zshrc:

eval "$(pyenv init -)"




######################
# FUNCTIONS          #
######################

function connect(){
  bluetoothctl power on
  bluetoothctl connect AC:12:2F:50:96:AC
}

function start(){
  setxkbmap -layout fr,us
  setxkbmap -option 'grp:win_space_toggle'

  xinput set-prop "Logitech G300s Optical Gaming Mouse" "libinput Accel Speed" -0.50
  xinput set-prop "ETPS/2 Elantech Touchpad" "libinput Tapping Enabled" 1
  xinput set-prop "ETPS/2 Elantech Touchpad" "libinput Tapping Enabled Default" 1
  xinput set-prop "ETPS/2 Elantech Touchpad" "libinput Natural Scrolling Enabled" 1
  xinput set-prop "ETPS/2 Elantech Touchpad" "libinput Natural Scrolling Enabled Default" 1
}

function battery(){
  upower -i /org/freedesktop/UPower/devices/battery_BAT0
}

function monitor(){
  xrandr --auto && xrandr --output eDP-1-1 --off
}

function laptop(){
  xrandr --output eDP-1-1 --auto
}

function ssd(){
  sudo smartctl -A /dev/nvme0n1 
}

function mic(){
  pacmd load-module module-alsa-source device=hw:Loopback,1,0
}

function d(){
  sudo systemctl start docker
  cd ~/Documents/okarito-api
  sudo docker-compose up
}

function dev(){
  cd ~/Documents/okarito-front
  yarn dev
}

function killimwheel(){
  imwheel -d --kill
}

function b(){
  bluetoothctl
}

function gwr(){
  git worktree remove 
}

function brightness(){
  brightnessctl set 1666
}
