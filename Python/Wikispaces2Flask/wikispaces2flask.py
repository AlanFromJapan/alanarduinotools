import os
import shutil
import re

TEMP_FILE = """D:/temp/_wikisp2flask_temp_out.html"""
replacePairs = [("/mainSpace/", "")]
replaceRegex = [
	#negative lookahead (?<!) : find href not pointing to a .html page that are not a http url (= only internal page links)
	re.compile(r'href="(?!http)([^"]+)(?<!\.html)"', re.IGNORECASE)
	]

def processFile (pFilename):
	with open(pFilename, "r") as f:
		with open(TEMP_FILE, "w") as t:
			for l in f:
				#simple replace
				for pair in replacePairs:
					if pair[0] in l:
						l = l.replace(pair[0], pair[1])
				#regex replace
				for r in replaceRegex:
					l = r.sub(r'href="\1.html"', l).replace("+","%20")
				#final write		
				t.write(l)
		shutil.copy (t.name, """D:/temp/out.html""")

		
#########################################################################
# MAIN
processFile ("""D:/temp/in.html""")