# make nvim the default text editor
export VISUAL=nvim
export EDITOR=nvim

# ZSH config
export ZSH="/home/alex/.oh-my-zsh"
ZSH_THEME="robbyrussell"
plugins=(git)
source $ZSH/oh-my-zsh.sh





######################
# ALIASES            #
######################

alias updateDB='docker-compose run -T api bash -c "rails db:drop && rails db:create && pg_restore --verbose --clean --no-acl --no-owner -d \"\$DATABASE_URL\""  <'
alias chrome="google-chrome-stable --password-store=gnome"
alias sudo="sudo "
alias vi="nvim"
alias vim="nvim"





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

  xinput set-prop "Logitech G300s Optical Gaming Mouse" "libinput Accel Speed" -0.65
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
