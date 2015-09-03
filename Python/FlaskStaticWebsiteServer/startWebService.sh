#!/bin/bash

ROOTFOLDER=/usr/local/websites/Flask.site
sudo -u webuser -s nohup /usr/bin/python $ROOTFOLDER/electrogeek_flask.py &

#give 2 secs for service to start
sleep 2
#check that the startup went nicely
PID=`ps -fe | grep 'electrogeek_flask.py' | grep webuser | grep -v root | awk '{print $2}' `
if [ -z "$PID" ]; then
    echo "ERROR! FLASK webserver not started."
else
    echo "FLASK Webserver started !"
fi