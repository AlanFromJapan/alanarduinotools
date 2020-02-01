#!/bin/bash

#sudo su - radio -c "pulseaudio -D; cd /usr/local/src/git/alanarduinotools/Python/WebRadioPlayer; python3 WebRadioPlayer.py" > /usr/local/src/git/alanarduinotools/Python/WebRadioPlayer/log/radioplayer.log

#to be ran as "radio" user
echo "## STARTING WebRadioPlayer ..."

#DONT start pulseaudio, it's systemwide BUT you must be in pulse-access to use it
#pulseaudio -k
#pulseaudio -D 
#nohup pulseaudio --start


cd /usr/local/src/git/alanarduinotools/Python/WebRadioPlayer
python3 WebRadioPlayer.py > /tmp/radioplayer.log 2>&1 &
echo "## STARTING WebRadioPlayer completed."


