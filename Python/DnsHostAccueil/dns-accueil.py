from datetime import datetime
from flask import Flask, render_template, redirect, url_for, request, make_response
import os
import string
import re

#http://stackoverflow.com/questions/20646822/how-to-serve-static-files-in-flask
app = Flask(__name__, static_url_path='')

#DNS file is in current folder for the test
DNS_FILE=os.path.join(os.path.dirname(os.path.abspath(__file__)), "sample.dnsmask.hosts")

REGEX_DNSENTRY=r"(?P<ip>(\d+\.\d+\.\d+\.\d+))\s+(?P<dns>(\S+))"

##########################################################################################
#default page -> redirect
@app.route('/')
@app.route('/index.html')
def homepage():
    t = """ 
<html>
<head>
<style>
body {font-family: Sansation, 'Lato', sans-serif;}
</style>
</head>
<body>

    <span style="
background-color: lavender;
display: block;
border: solid 1px darkblue;
text-align:center;
font-size:20pt;
"
>DNS Entries list</span>
"""
    with open(DNS_FILE, mode="r") as f:
        for l in f:
            l2 = re.sub (REGEX_DNSENTRY, r""" <a href="http://\g<ip>">Go to <span style="font-weight:bold;text-transform: capitalize;">\g<dns></span></a> (\g<ip>) """, l)

            if l2.startswith("#"):
                l2 = r""" <span style="color:grey;">""" +l2+ """</span> """

            l2 = l2.replace ("\n", "<br/>")
            print (l + " => " + l2)            
            t = t + l2
    

    t = t + """ 
</body>
</html>
"""
#    t = re.sub (REGEX_DNSENTRY, """ "<a href="\g<ip>">\g<dns></a" """, t)


    return t


########################################################################################
## Main entry point
#
if __name__ == '__main__':
    app.debug = True

    #start serving pages
    app.run(host='0.0.0.0', port=5000, threaded=True)


