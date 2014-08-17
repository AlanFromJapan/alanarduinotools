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
    <wellKnownErrorPriority>{wkePrio}</wellKnownErrorPriority>
    <wellKnownErrorLabel>{wkeLabel}</wellKnownErrorLabel>
</logItem>"""

reLine = re.compile(LINE_IN_FORMAT, re.IGNORECASE)
dictErrors = dict()
dictWellKnownBipCodeStyles = dict(BIP2153I="prioHigh", BIP2176S="prioLow")



class WellKnownError:
    def __init__(self, bipCode, messageContains,errorPriority,errorLabel):
        self.bipCode=bipCode
        self.messageContains =  messageContains
        self.errorPriority = errorPriority
        self.errorLabel = errorLabel
        
listWellKnownErrors = [
WellKnownError(bipCode=None, messageContains=r"FAXECMServiceSFDCMainFlow#FCMComposite_1_8.CreateFormImageSFDCSubFlow#FCMComposite_1_3", errorPriority=1, errorLabel="Timeout on FormServer report generation. Ignore unless user complains.")
    ]



    
###########################################
##  Web server 
###########################################
class MyHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    'Source from https://wiki.python.org/moin/BaseHttpServer'
    
    ###########################################
    ##  Process requests
    ###########################################
    def getWellknownErrorDetails (s, bipCode, rawContent):
        for wke in listWellKnownErrors:
            if (wke.bipCode != None and wke.bipCode == bipCode) or (wke.messageContains != None and rawContent != None and wke.messageContains in rawContent):
                return wke
        return None
    
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
                rawContent=match.group("theRest")
                
                if bipcode != None:
                    if bipcode in dictErrors:
                        dictErrors[bipcode] = (bipcode, dictErrors[bipcode][1] + 1)
                    else:
                        dictErrors[bipcode] = (bipcode, 1)

                #get details about the error if already known
                wke = s.getWellknownErrorDetails(bipcode,rawContent)

                #final output to the XML file
                s.wfile.write(LINE_OUT_FORMATXML.format(
                    time=match.group('time'),
                    bipcode=bipcode,
                    msgTitle=match.group('msgTitle'),
                    bipcount=str(dictErrors[bipcode][1]),
                    prioStyle=("prioDefault" if not bipcode in dictWellKnownBipCodeStyles else dictWellKnownBipCodeStyles[bipcode]),
                    rawContent=(rawContent if bipcode != "BIP3051E" else wmbErrorParser.parseFormatWmbError(rawContent)),
                    wkePrio=('' if wke == None else wke.errorPriority),
                    wkeLabel=('' if wke == None else wke.errorLabel),
                    ))
            else:
                #weird, line was unmatched : export it AS IS
                #s.wfile.write("Unknown format line:" + line)
                s.wfile.write(LINE_OUT_FORMATXML.format(
                    time="",
                    bipcode="",
                    msgTitle="UNREADABLE MESSAGE!",
                    bipcount=-1,
                    prioStyle="prioHigh",
                    rawContent=line, 
                    wkePrio=-1,
                    wkeLabel='',
                    ))

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
    ##  Base HTTP server (GET request management)
    ###########################################            
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
            s.handle_getContentAsXML ()
        elif s.path == "/Styles.css":
            s.handle_getFile (s.path[1:])
        elif s.path == "/Transform.xsl":
            s.handle_getFile (s.path[1:])
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
