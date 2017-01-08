"""
Eagle Grid Wiring Fixer 
20170108 - http://electrogeek.cc/
Takes an Eagle .brd file (XML so Eagle v6+) and fix those close-but-not-matching wirings
"""

import sys
import xml.etree.ElementTree as ET

if len(sys.argv) == 1:
    print("""Eagle Grid Wiring Fixer
--------------------
Takes an Eagle .brd file (XML so Eagle v6+) and fix those close-but-not-matching wirings

  Usage: python %s bad.brd
  Output: a fixed .brd file (do a backup before!)""")
    sys.exit(1)
    
vInputFile = str(sys.argv[1])
print ("Make a backup before or make a local commit!")
print ("Input '%s'"  % (vInputFile))

"""
So the idea: open the file, check for all wires begining and ending segments only (begin x1;y1, ending x2;y2), if we can find some connector (not wires) that are at same local +/- a delta and update the wire x;y to that coordinate.
"""
verbose = True
delta = 0.002

#open the file
eTree = ET.parse(vInputFile)
eRoot = eTree.getroot()

eRootSignals = eRoot.find('.//signals')
if eRootSignals is None:
    print("Can't find 'signals' node!")
    sys.exit(2)

for vWire in eRootSignals.findall("""signal[@name='N$115']"""):
    #if a Pad, skip
    if len(vWire.findall('contactref')) != 0:
        continue
    
    if verbose:
        print("Processing %s" % (vWire.get('name')))
        
    #considering that wire, can we match it to another one? Guess what: no search so it's an n*n because we can't skip the past ones in the search (only search forward would be enough)
    #can be improved with xpath
    for vPad in eRootSignals.findall('signal'):
        #self? skip
        if vPad.get('name') == vWire.get('name'):
            continue
        #if not a Pad, skip
        if len(vPad.findall('contactref')) == 0:
            continue
        #now we can consider it's a pad
        
        #does end match?
        lwire = list(vWire)
        vWireEnd = lwire[len(lwire) -1]
        
        vPadEnd = vPad.find('wire')
        
        if abs(float(vWireEnd.get('x2')) - float(vPadEnd.get('x1'))) < 2 * delta and \
        abs(float(vWireEnd.get('y2')) - float(vPadEnd.get('y1'))) < 2 * delta or \
        abs(float(vWireEnd.get('x2')) - float(vPadEnd.get('x2'))) < 2 * delta and \
        abs(float(vWireEnd.get('y2')) - float(vPadEnd.get('y2'))) < 2 * delta :
            print ("> Matched wire %s and pad %s" % (vWire.get('name'), vPad.get('name')))
            #edit the coordinates of the WIRE
            vWireEnd.set('x2', vPadEnd.get('x1'))
            vWireEnd.set('y2', vPadEnd.get('y1'))
            #can match once only
            continue

eTree.write("tinynumitronboard-extended_iv6.brd")            