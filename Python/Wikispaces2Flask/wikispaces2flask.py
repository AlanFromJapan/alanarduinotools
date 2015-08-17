import os
import shutil
import re
import sys

if len(sys.argv) == 1:
    print("""Wikispaces to Flask converter
-----------------------------
Fixes the html export for a easy integration in Flask

  Usage: python %s <path to source files> <path to output>
""" % (str(sys.argv[0])))
    sys.exit(1)

	
def postReplace1 (str):
	return str.replace("+","%20")
	
#TODO replace	
TEMP_FILE = """D:/temp/_wikisp2flask_temp_out.html"""

#Fixed strings
replacePairs = [("/mainSpace/", "")]
#Regex replace
replaceRegex = [
	#negative lookahead (?<!) : find href not pointing to a .html page that are not a http url (= only internal page links)
	#only the <a> to avoid messing the stylesheet refs
	(re.compile(r'<a (\.*)href="(?!http)([^"]+)(?<!\.html)"', re.IGNORECASE), postReplace1)
	]

def processFile (pFilename):
	with open(os.path.join(str(sys.argv[1]), pFilename), "r") as f:
		with open(TEMP_FILE, "w") as t:
			for l in f:
				#simple replace
				for pair in replacePairs:
					if pair[0] in l:
						l = l.replace(pair[0], pair[1])
				#regex replace
				for r in replaceRegex:
					l = r[1](r[0].sub(r'<A \1 href="\2.html"', l))

				#final write		
				t.write(l)
		shutil.copy (t.name, os.path.join(os.path.join(str(sys.argv[2]), "static"), pFilename))

		
#########################################################################
# MAIN
#processFile ("""D:/temp/in.html""")
for f in os.listdir(str(sys.argv[1])):
	if os.path.isfile(os.path.join(str(sys.argv[1]), f)):
		print("> %s" % (f))
		processFile(f)
	