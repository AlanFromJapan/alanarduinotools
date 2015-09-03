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

@app.route('/')
@app.route('/index.html')
def homepage():
#    app.logger.debug('homepage')
#    return app.send_static_file('home.html')
    return redirect('/home.html')

"""
@app.route('/<page>.html')
def servePage(page):
    t = None
    vFilePath = "" + page + ".html"
    with os.open(vFilePath, mode="r") as f:
        t = f.read()
    return t
"""

"""
#test of the templating
@app.route('/T.html')
def testTemplate(page="whoami"):
    t = None
    vFilePath = ROOTDIR + page + ".html"
    with open(vFilePath, mode="r") as f:
        t = f.read().decode("utf-8")
    return render_template('template01.html', pagename=page, pagecontent=t)
"""
"""
#standard static page serving
@app.route('/<page>.html')
def servePage(page):
    app.logger.debug('servePage: ' + page)
    return app.send_static_file(page.lower() + ".html")
"""

#serving page through template
@app.route('/<page>.html')
def serveTemplate(page):
    t = None
    year = datetime.now().strftime('%Y')
    #make sure this path is a *safe* path
    vFilePath = ROOTDIR + page.lower() + ".html"
    with open(vFilePath, mode="r") as f:
        t = f.read().decode("utf-8")
    return render_template(Config.get("Design", "Template"), pagename=page, pagecontent=t, year=year)


if __name__ == '__main__':
    import logging
    logging.basicConfig(filename=LOGFILE,level=logging.DEBUG)

    Config.read("electrogeek.ini")

    #loading once and for all the config values
    ROOTDIR = Config.get("WebConfig", "ROOTDIR")
    LOGFILE = Config.get("WebConfig", "LOGFILE")
    HTTPPORT = Config.get("WebConfig", "HTTPPORT")

    app.run(host='0.0.0.0', port=HTTPPORT)
