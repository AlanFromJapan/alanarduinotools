import time
import BaseHTTPServer
import re
import urllib
import shutil
#local custom scripts
import wmbErrorParser

HOST_NAME = ''
PORT_NUMBER = 8001 # Maybe set this to 9000.


LINE_IN_FORMAT = r"^(?P<time>\w+\s\d+\s\d+:\d+:\d+)[^]]+\][^]]+\](?P<bipcode>\w+):(?P<msgTitle>[^:]+):(?P<theRest>.*)"
LINE_OUT_FORMATXML = r"""<logItem>
    <time>{time}</time>
    <bipcode>{bipcode}</bipcode>
    <bipcount>{bipcount}</bipcount>
    <msgTitle>{msgTitle}</msgTitle>
    <prioStyle>{prioStyle}</prioStyle>
    <rawContent>{rawContent}</rawContent>
</logItem>"""

reLine = re.compile(LINE_IN_FORMAT, re.IGNORECASE)
dictErrors = dict()
dictWellKnownBipCodeStyles = dict(BIP2153I="prioHigh", BIP2176S="prioLow")

###########################################
##  Process requests
###########################################

def handle_getContentAsXML(s):
    '''s is the HttpRequestHandler 'self'.
    This method returns the content of the file.'''
    fin = file(name=r"D:\temp\wmbmessages.log.20140811byAVI\wmbmessages.log.20140811byAVI", mode="r")

    dictErrors.clear()
    
    s.do_HEAD()

    s.wfile.write("""<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet type="text/xsl" href="Transform.xsl"?>
""")

    s.wfile.write("<logItems>")
    i = 0
    for line in fin:
        #s.wfile.write(line)
        match = reLine.match(line)
        if match != None:
            bipcode=match.group('bipcode')
            if bipcode != None:
                if bipcode in dictErrors:
                    dictErrors[bipcode] = (bipcode, dictErrors[bipcode][1] + 1)
                else:
                    dictErrors[bipcode] = (bipcode, 1)
                    
            s.wfile.write(LINE_OUT_FORMATXML.format(
                time=match.group('time'),
                bipcode=bipcode,
                msgTitle=match.group('msgTitle'),
                bipcount=str(dictErrors[bipcode][1]),
                prioStyle=("prioDefault" if not bipcode in dictWellKnownBipCodeStyles else dictWellKnownBipCodeStyles[bipcode]),
                rawContent=(line if bipcode != "BIP3051E" else wmbErrorParser.parseFormatWmbError(match.group("theRest"))), 
                ))
        else:
            s.wfile.write("Unknown format line:" + line)

        #dummy limit to 100 for test
        i += 1
        if i > 100:
            break
    
    s.wfile.write("</logItems>")
    
    fin.close()




def handle_getFile (s, path):
    path = """D:\\Subversion\\Python\\WmbLogsServer\\""" + path
    f = None
    try:
        s.do_HEAD()
        print ("serving : '" + path  +"'")
        
        f = open (path, mode="r")
        shutil.copyfileobj(f, s.wfile)
    finally:
        if f != None:
            f.close()
    
###########################################
##  Web server part
###########################################

class MyHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    'Source from https://wiki.python.org/moin/BaseHttpServer'
    def do_HEAD(s):
        s.send_response(200)
        if len(s.path) > 4 and s.path[-3:] == "xml":
            s.send_header("Content-type", "text/xml")
        elif len(s.path) > 4 and s.path[-3:] == "css":
            s.send_header("Content-type", "text/css")
        elif len(s.path) > 5 and s.path[-4:] == "logs":
            s.send_header("Content-type", "text/html")
        else:
            s.send_header("Content-type", 'application/octet-stream')
        s.end_headers()


    def do_GET(s):
        """Respond to a GET request."""

        #First: triage of the request
        if s.path == "/XXX.xml":
            handle_getContentAsXML (s)
        elif s.path == "/Styles.css":
            handle_getFile (s, s.path[1:])
        elif s.path == "/Transform.xsl":
            handle_getFile (s, s.path[1:])
        else:
            s.send_response(404)
  

###########################################
##  Main()
###########################################

if __name__ == '__main__':
    server_class = BaseHTTPServer.HTTPServer
    httpd = server_class((HOST_NAME, PORT_NUMBER), MyHandler)
    print time.asctime(), "Server Starts - %s:%s" % (HOST_NAME, PORT_NUMBER)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    print time.asctime(), "Server Stops - %s:%s" % (HOST_NAME, PORT_NUMBER)
