from datetime import datetime
from flask import Flask, render_template, redirect, url_for, request, make_response
import os
import string
import re

#http://stackoverflow.com/questions/20646822/how-to-serve-static-files-in-flask
app = Flask(__name__, static_url_path='')

DNS_FILE="/home/alan/temp/sample.dnsmask.hosts"
REGEX_DNSENTRY=r"(?P<ip>(\d+\.\d+\.\d+\.\d+))\s+(?P<dns>(\S+))"

##########################################################################################
#default page -> redirect
@app.route('/')
@app.route('/index.html')
def homepage():
    t = ""
    with open(DNS_FILE, mode="r") as f:
        for l in f:
            l2 = re.sub (REGEX_DNSENTRY, """ <a href="\g<ip>">Go to \g<dns></a> """, l)
            print (l + " => " + l2)
            t = t + l + "=&gt;" + l2
#    t = f.read()
    
    t = string.replace(t, "\n", "<br/>")

#    t = re.sub (REGEX_DNSENTRY, """ "<a href="\g<ip>">\g<dns></a" """, t)
    return t


########################################################################################
## Main entry point
#
if __name__ == '__main__':
    app.debug = True

    #start serving pages
    app.run(host='0.0.0.0', port=5000, threaded=True)


