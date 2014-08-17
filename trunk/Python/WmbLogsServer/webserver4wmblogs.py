from datetime import datetime
import time
import BaseHTTPServer
import re
import urllib
import shutil
import ConfigParser
#local custom scripts
import wmbErrorParser

###########################################
##  CONSTANTS
###########################################

HOST_NAME = ''
PORT_NUMBER = 8001 # overriden inn the .ini file.
ROOT_WEBFOLDER = """D:\\Subversion\\Python\\WmbLogsServer\\"""

DEFAULT_XSLFILE = 'Transform.xsl'
DEFAULT_LINE_OUT_FORMATXML = r"""<logItem>
    <line>{lineNumber}</line>
    <time>{time}</time>
    <bipcode>{bipcode}</bipcode>
    <bipcount>{bipcount}</bipcount>
    <msgTitle>{msgTitle}</msgTitle>
    <rawContent>{rawContent}</rawContent>
    <wellKnownErrorPriority>{wkePrio}</wellKnownErrorPriority>
    <wellKnownErrorLabel>{wkeLabel}</wellKnownErrorLabel>
</logItem>
"""

###########################################
##  Subclass for xml pages
###########################################
class XmlLogPage:
    def __init__(self, url, filePath, lineIn, lineOut=DEFAULT_LINE_OUT_FORMATXML, xslFile=DEFAULT_XSLFILE):
        self.URL = url
        self.filePath = filePath
        self.lineIn = lineIn
        self.lineOut = lineOut
        self.xslFile= xslFile
        
        self.regex = re.compile(self.lineIn, re.IGNORECASE)

        self.wkes = []
            
    def __str__(self):
        return """URL = %s
    filePath = %s
    lineIn = %s
    lineOut = %s
    xslFile = %s""" % (self.URL, self.filePath, self.lineIn, self.lineOut, self.xslFile)
#dictionnary of the existing pages
dictXmlLogPages = dict()

###########################################
##  Subclass for Well Known Errors 
###########################################
class WellKnownError:
    def __init__(self, bipCode, messageContains,errorPriority="PrioDefault",errorLabel=None,parseErrorMessage=False):
        self.bipCode=bipCode
        self.messageContains =  messageContains
        self.errorPriority = errorPriority
        self.errorLabel = errorLabel
        self.parseErrorMessage = parseErrorMessage
        
listWellKnownErrors = [
    WellKnownError(bipCode=None, messageContains=r"FAXECMServiceSFDCMainFlow#FCMComposite_1_8.CreateFormImageSFDCSubFlow#FCMComposite_1_3", errorPriority="prioLow", errorLabel="Timeout on FormServer report generation. Ignore unless user complains.", parseErrorMessage=True),
    WellKnownError(bipCode="BIP2153I", messageContains=None, errorPriority="prioHigh"),
    WellKnownError(bipCode="BIP2176S", messageContains=None, errorPriority="prioLow"),
    WellKnownError(bipCode="BIP3051E", messageContains=None, parseErrorMessage=True),
]



    
###########################################
##  Web server 
###########################################
class MyHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    'Source from https://wiki.python.org/moin/BaseHttpServer'
    
    ###########################################
    ##  Process requests
    ###########################################
    def getWellknownErrorDetails (s, bipCode, rawContent, page):
        for wke in page.wkes:
            if (wke.bipCode != None and wke.bipCode == bipCode) or (wke.messageContains != None and rawContent != None and wke.messageContains in rawContent):
                return wke
        for wke in listWellKnownErrors:
            if (wke.bipCode != None and wke.bipCode == bipCode) or (wke.messageContains != None and rawContent != None and wke.messageContains in rawContent):
                return wke
        return None
    
    def handle_getContentAsXML(s, page):
        '''s is the HttpRequestHandler 'self'.
        This method returns the content of the file.'''
        startTime = datetime.now()
        fin = None
        try:
            fin = file(name=page.filePath, mode="r")
            
            #Keeps track of errors while parsing to provide running count
            dictErrors = dict()
            
            s.do_HEAD()

            s.wfile.write(
"""<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet type="text/xsl" href="{xslFile}"?>
""".format (xslFile=page.xslFile))
            s.wfile.write("<logfileExport>\n")
            s.wfile.write("<logItems>\n")
            i = 1
            for line in fin:
                #s.wfile.write(line)
                match = page.regex.match(line)
                if match != None:
                    bipcode=match.group('bipcode')
                    rawContent=match.group("theRest")

                    #running count of errors
                    if bipcode != None:
                        if bipcode in dictErrors:
                            dictErrors[bipcode] = (bipcode, dictErrors[bipcode][1] + 1)
                        else:
                            dictErrors[bipcode] = (bipcode, 1)

                    #get details about the error if already known
                    wke = s.getWellknownErrorDetails(bipcode,rawContent, page)

                    #final output to the XML file
                    s.wfile.write(page.lineOut.format(
                        lineNumber=i,
                        time=match.group('time'),
                        bipcode=bipcode,
                        msgTitle=match.group('msgTitle'),
                        bipcount=str(dictErrors[bipcode][1]),
                        rawContent=(rawContent if wke == None or not wke.parseErrorMessage else wmbErrorParser.parseFormatWmbError(rawContent)),
                        wkePrio=('prioDefault' if wke == None else wke.errorPriority),
                        wkeLabel=('' if wke == None or wke.errorLabel == None else wke.errorLabel),
                        ))
                else:
                    #weird, line was unmatched : export it AS IS
                    s.wfile.write(page.lineOut.format(
                        lineNumber=i,
                        time="",
                        bipcode="",
                        msgTitle="UNREADABLE MESSAGE!",
                        bipcount=-1,
                        rawContent=line, 
                        wkePrio="prioHigh",
                        wkeLabel='',
                        ))

                #goto next line
                i += 1
                #for the tests, TODO REMOVE ME after
                if i > 30: break
                
            #finished: close the xml root element
            s.wfile.write("</logItems>\n")

            endtime = datetime.now()

            s.wfile.write(r"""
<generationDetails>
    <startTime>%s</startTime>
    <generationDuration>%s</generationDuration>
</generationDetails>
""" % (startTime, endtime - startTime))
            
            s.wfile.write("</logfileExport>\n")
            s.wfile.write(r"""

<!-- Generation at : %s -->
<!-- Generation time : %s -->
""" % (startTime, endtime - startTime)) 
        finally:
            if fin != None:
                fin.close()




    def handle_getFile (s, path):
        path = ROOT_WEBFOLDER + path
        f = None
        try:
            s.do_HEAD()
            #print ("serving : '" + path  +"'")
            
            f = open (path, mode="rb")
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
        else:
            s.send_header("Content-type", 'application/octet-stream')
        s.end_headers()


    def do_GET(s):
        """Respond to a GET request."""

        #First: triage of the request, ONLY AUTHORIZED FILE *NAMES*
        #Check the FULL PATH of the request, you can get only files that you authorize here (avoid hacking by URL ../../../secretFile)
        if s.path[1:] in dictXmlLogPages:
            s.handle_getContentAsXML (dictXmlLogPages[s.path[1:]])
        elif s.path == "/Styles.css":
            s.handle_getFile (s.path[1:])
        elif s.path == "/Transform.xsl":
            s.handle_getFile (s.path[1:])
        elif s.path == "/hint.gif":
            s.handle_getFile (s.path[1:])
        else:
            s.send_response(404)
  

###########################################
##  Reading configuration
###########################################
def emptyIsDefault(val, default):
    if val== None or val == '':
        return default
    else:
        return val

def loadConfig(config):
    for sec in config.sections():
        if not (len(sec) > 5 and sec[-4:] == ".xml"):
            continue
        print ("Loading config for " + sec)
        logfile = config.get(sec, 'logfile')
        lineIn = config.get(sec, 'lineInFormat')

        lineOut = None
        if config.has_option(sec, 'lineOutFormat'):
            lineOut = config.get(sec, 'lineOutFormat')
        else:
            lineOut = DEFAULT_LINE_OUT_FORMATXML

        xslFile = None
        if config.has_option(sec, 'xslFile'):
            xslFile = config.get(sec, 'xslFile')
        else:
            xslFile = DEFAULT_XSLFILE
        
        page = XmlLogPage(
            url = sec,
            filePath = logfile,
            lineIn = lineIn,
            lineOut = lineOut,
            xslFile = xslFile)
        
        dictXmlLogPages[sec] = page

        i = 1
        while True:
            if not config.has_option(sec, 'WKE_bipCode' + str(i)):
                break
            bipcode =  emptyIsDefault(config.get(sec, 'WKE_bipCode' + str(i)), None)
            messageContains=  emptyIsDefault(config.get(sec, 'WKE_messageContains' + str(i)), None)
            errorPriority =  emptyIsDefault(config.get(sec, 'WKE_errorPriority' + str(i)), "prioDefault")
            errorLabel =  config.get(sec, 'WKE_errorLabel' + str(i))
            parseErrorMessage =  emptyIsDefault(config.getboolean(sec, 'WKE_parseErrorMessage' + str(i)), False)

            wke = WellKnownError(bipcode, messageContains, errorPriority, errorLabel, parseErrorMessage)
            page.wkes.append(wke)
            i += 1
        print (page)
            
###########################################
##  Main()
###########################################

if __name__ == '__main__':
    #read config file
    config = ConfigParser.ConfigParser()
    config.read('wmblogsserver.ini')
    PORT_NUMBER = int(config.get('Global', 'port'))
    loadConfig(config)
    
    server_class = BaseHTTPServer.HTTPServer
    httpd = server_class((HOST_NAME, PORT_NUMBER), MyHandler)
    print time.asctime(), "Server Starts - %s:%s" % (HOST_NAME, PORT_NUMBER)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    print time.asctime(), "Server Stops - %s:%s" % (HOST_NAME, PORT_NUMBER)
