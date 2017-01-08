"""
Eagle Grid Wiring Fixer 
20170108 - http://electrogeek.cc/
Takes an Eagle .brd file (XML so Eagle v6+) and fix those close-but-not-matching wirings
"""

import sys
import xml.etree.ElementTree as ET

#constants
verbose = True
delta = 0.01

#Is the Pad airwire matched by one of the extremities of the unpadded wire?
def wireMatch(vWire, vPad):
    vPadEnd = vPad.find('wire')
    
    #Eagle is not very picky, so ANY end of ANY wire might match the pads 
    vMatchStart = False
    vMatchEnd = False
    
    for w in vWire:
        vMatchStart = vMatchStart or \
            (abs(float(w.get('x2')) - float(vPadEnd.get('x1'))) < 2 * delta and \
            abs(float(w.get('y2')) - float(vPadEnd.get('y1'))) < 2 * delta or \
            abs(float(w.get('x1')) - float(vPadEnd.get('x1'))) < 2 * delta and \
            abs(float(w.get('y1')) - float(vPadEnd.get('y1'))) < 2 * delta)
            
        vMatchEnd = vMatchEnd or \
            (abs(float(w.get('x2')) - float(vPadEnd.get('x2'))) < 2 * delta and \
            abs(float(w.get('y2')) - float(vPadEnd.get('y2'))) < 2 * delta or \
            abs(float(w.get('x1')) - float(vPadEnd.get('x2'))) < 2 * delta and \
            abs(float(w.get('y1')) - float(vPadEnd.get('y2'))) < 2 * delta)
            
    return vMatchStart and vMatchEnd

########################################################################################
## Main entry point
if __name__ == '__main__':
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
    So the idea: 
    - find a wire that is NOT connected to any pad yet (airwire, layer 19), and of whose ends match (+/- delta) of a pad.
    - inject in the padded-wire the content of the unpadded-wire
    - remove the unpadded-wire
    """


    #open the file, and make 2 trees, because parsing a tree and modifying at the same time is always a bad idea at best
    eTree = ET.parse(vInputFile)
    eRoot = eTree.getroot()

    eOutTree = ET.parse(vInputFile)

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
                
            #if not airwire, skip
            if vPad.find('./wire[@layer="19"]') is None:
                #no airwire, skip
                continue

            #pure airwire or skip
            if len(vPad.findall('./wire[@layer="19"]')) != 1:
                #more than 1 airwire? ignore for now
                continue
                
            #now we can consider it's an UNWIRED PAD            
            
            if wireMatch (vWire, vPad):
                print ("> Matched free wire %s and padded wire %s" % (vWire.get('name'), vPad.get('name')))
                
                #update the copy of the tree
                vTargetPad = eOutTree.getroot().find("signal[@name='%s']" % (vPad.get('name')))
                #for vW in vWire:
                    #first, remove the target padded wire (what the pad want to have wired
                
                #can match once only
                continue

    #eOutTree.write("tinynumitronboard-extended_iv6.brd")            