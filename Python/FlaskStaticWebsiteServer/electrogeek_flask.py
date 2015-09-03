from datetime import datetime
from flask import Flask, render_template, redirect, url_for
import os
import ConfigParser

#http://stackoverflow.com/questions/20646822/how-to-serve-static-files-in-flask
app = Flask(__name__, static_url_path='')
Config = ConfigParser.ConfigParser()

ROOTDIR = None 
LOGFILE = None
HTTPPORT = None

#default page -> redirect
@app.route('/')
@app.route('/index.html')
def homepage():
    return redirect('/home.html')


#serving page through template
@app.route('/<page>.html')
def serveTemplate(page):
    t = None
    year = datetime.now().strftime('%Y')

    #make sure this path is a *safe* path
    vFilePath = ROOTDIR + page.lower() + ".html"

    #read content of the static file
    with open(vFilePath, mode="r") as f:
        t = f.read().decode("utf-8")

    #generate the output by injecting static page content and a couple of variables in the template page
    return render_template(Config.get("Design", "Template"), pagename=page, pagecontent=t, year=year)



########################################################################################
## Main entry point
if __name__ == '__main__':
    import logging
    logging.basicConfig(filename=LOGFILE,level=logging.DEBUG)

    Config.read("electrogeek.ini")

    #loading once and for all the config values
    ROOTDIR = Config.get("WebConfig", "ROOTDIR")
    LOGFILE = Config.get("WebConfig", "LOGFILE")
    HTTPPORT = Config.get("WebConfig", "HTTPPORT")

    app.run(host='0.0.0.0', port=HTTPPORT)
