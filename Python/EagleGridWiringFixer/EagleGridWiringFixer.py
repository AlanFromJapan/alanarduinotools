"""
Eagle Grid Wiring Fixer 
20170108 - http://electrogeek.cc/
Takes an Eagle .brd file (XML so Eagle v6+) and fix those close-but-not-matching wirings
"""

import sys
import xml.etree.ElementTree as ET
import math

#constants
verbose = False
delta = 0.5

#global variables
totalwire=0
totalmatched=0

###############################################################################################3
##
#Is the Pad airwire matched by one of the extremities of the unpadded wire?
def wireMatch(vWire, vAirWire):
    vAirWireEnd = vAirWire.find('wire')

    if verbose:
        None
        #print("wireMatch try to match x1=%f y1=%f and x2=%f y2=%f" % ( float(vAirWireEnd.get('x1')),  float(vAirWireEnd.get('y1')),  float(vAirWireEnd.get('x2')),  float(vAirWireEnd.get('y2'))))
    
    #Eagle is not very picky, so ANY end of ANY wire might match the pads 
    vMatchStart = False
    vMatchEnd = False
    
    for w in vWire.findall('wire'):
        vMatchStart = vMatchStart or \
                distance(w,vAirWireEnd, '1', '1') < delta  or \
                distance(w,vAirWireEnd, '2', '1') < delta 

        vMatchEnd = vMatchEnd or \
                distance(w,vAirWireEnd, '1', '2') < delta  or \
                distance(w,vAirWireEnd, '2', '2') < delta 
                
    if verbose:
        print("wireMatch Start=%s End=%s" % (vMatchStart, vMatchEnd))
            
    return vMatchStart and vMatchEnd


###############################################################################################3
##
# Returns the distance btw 2 points
def distance(w1, w2, w1suffix, w2suffix):
    return math.sqrt( \
                      (float(w1.get('x' + w1suffix)) - float(w2.get('x' + w2suffix))) ** 2 \
                      + (float(w1.get('y' + w1suffix)) - float(w2.get('y' + w2suffix))) ** 2 )

###############################################################################################3
##
# Find the best candidate segment within the wire to match the position and update
def forceMatchWires(vWire, vAirWire):
    vAirWireEnd = vAirWire.find('./wire[@layer="19"]')

    if verbose:
        None
        #print("wireMatch try to match x1=%f y1=%f and x2=%f y2=%f" % ( float(vAirWireEnd.get('x1')),  float(vAirWireEnd.get('y1')),  float(vAirWireEnd.get('x2')),  float(vAirWireEnd.get('y2'))))
    
    #Eagle is not very picky, so ANY end of ANY wire might match the pads 
    vMatchStart = None
    vDistStart = 10000000.0
    vMatchStart12 = 0

    vMatchEnd = None
    vDistEnd = 10000000.0
    vMatchEnd12 = 0

    for w in vWire.findall("wire"):
        d = distance(w, vAirWireEnd, '1', '1')
        if vMatchStart is None or d < vDistStart:
            vMatchStart = w
            vMatchStart12 = 1
            vDistStart = d

        d = distance(w, vAirWireEnd, '2', '1')
        if vMatchStart is None or d < vDistStart:
            vMatchStart = w
            vMatchStart12 = 2
            vDistStart = d

        d = distance(w, vAirWireEnd, '1', '2')
        if vMatchEnd is None or d < vDistEnd:
            vMatchEnd = w
            vMatchEnd12 = 1
            vDistEnd = d

        d = distance(w, vAirWireEnd, '2', '2')
        if vMatchEnd is None or d < vDistEnd:
            vMatchEnd = w
            vMatchEnd12 = 2
            vDistEnd = d


    #now the closest points are in vMatchStart and vMatchEnd
    vMatchStart.set('x' + str(vMatchStart12), vAirWireEnd.get('x1'))
    vMatchStart.set('y' + str(vMatchStart12), vAirWireEnd.get('y1'))
    
    vMatchEnd.set('x' + str(vMatchEnd12), vAirWireEnd.get('x2'))
    vMatchEnd.set('y' + str(vMatchEnd12), vAirWireEnd.get('y2'))


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
    - for each airwire
      - find a wire that is NOT connected , and of whose ends match (+/- delta) of the airwire.
      - inject in the wire the content of the airwire
      - remove the airwire details from the now wired airwire
    """


    #open the file, and make 2 trees, because parsing a tree and modifying at the same time is always a bad idea at best
    eTree = ET.parse(vInputFile)
    eRoot = eTree.getroot()

    eOutTree = ET.parse(vInputFile)

    eRootSignals = eRoot.find('.//signals')
    if eRootSignals is None:
        print("Can't find 'signals' node!")
        sys.exit(2)

#    for vWire in eRootSignals.findall("""signal[@name='N$115']"""):
    for vWire in eRootSignals.findall("""signal"""):
        #if an airwire, skip
        if len(vWire.findall('contactref')) != 0:
            continue
        if not vWire.find('./wire[@layer="19"]') is None:
            #airwire -> skip
            continue
        
        print("Processing %s" % (vWire.get('name')))
        totalwire = totalwire+1

        #considering that wire, can we match it to another one? Guess what: no search so it's an n*n because we can't skip the past ones in the search (only search forward would be enough)
        #can be improved with xpath
        for vAirWire in eRootSignals.findall('signal'):
            #self? skip
            if vAirWire.get('name') == vWire.get('name'):
                continue
            #if not a Pad, skip
            if len(vAirWire.findall('contactref')) == 0:
                continue
                
            #if not airwire, skip
            if vAirWire.find('./wire[@layer="19"]') is None:
                #no airwire, skip
                continue

            #pure airwire or skip
            if len(vAirWire.findall('./wire[@layer="19"]')) != 1:
                #more than 1 airwire? ignore for now
                continue
                
            #now we can consider it's an UNWIRED PAD            
            
            if wireMatch (vWire, vAirWire):
                print ("> Matched free wire %s and padded wire %s" % (vWire.get('name'), vAirWire.get('name')))
                
                #update the copy of the tree
                vTargetAirWire = eOutTree.getroot().find(".//signal[@name='%s']" % (vAirWire.get('name')))

                #first, update the startend
                forceMatchWires (vWire, vAirWire)

                #insert all the wires and Vias post-force matching
                vTargetAirWire.extend (list(vWire))

                #lastly, remove the target padded wire 
                vTargetAW2delete = vTargetAirWire.find('./wire[@layer="19"]')
                vTargetAirWire.remove(vTargetAW2delete)
                
                #And remove the wire you used because they fusionned now
                vWire2remove = eOutTree.getroot().find(".//signal[@name='%s']" % (vWire.get('name')))
                eOutTree.getroot().find(".//signals").remove(vWire2remove)

                totalmatched = totalmatched +1
                #can match once only
                continue

    eOutTree.write(vInputFile)            

    print ("""SUMMARY:
Total umatched wires: %s
Matched wires: %s (%f%%)
""" % (totalwire, totalmatched, 100.0 * float(totalmatched) / float(totalwire)))
