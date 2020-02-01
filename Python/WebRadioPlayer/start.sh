#!/bin/bash

#sudo su - radio -c "pulseaudio -D; cd /usr/local/src/git/alanarduinotools/Python/WebRadioPlayer; python3 WebRadioPlayer.py" > /usr/local/src/git/alanarduinotools/Python/WebRadioPlayer/log/radioplayer.log

#to be ran as "radio" user
echo "## STARTING WebRadioPlayer ..."
sleep 5

pulseaudio -k
pulseaudio -D 

sleep 5

cd /usr/local/src/git/alanarduinotools/Python/WebRadioPlayer
nohup python3 WebRadioPlayer.py > /tmp/radioplayer.log 2>&1 &
echo "## STARTING WebRadioPlayer completed."


