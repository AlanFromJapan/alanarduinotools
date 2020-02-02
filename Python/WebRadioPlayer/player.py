#misc imports
import time
import os
import config

import subprocess

class Player:
    vlcproc = None
    
    def play(self, url):
        if self.vlcproc == None:
            print ("Debug: about to start VLC for url '%s'" % (url))
            #self.vlcproc = subprocess.Popen("vlc %s -I dummy" % (url))            
            self.vlcproc = subprocess.Popen(["vlc", url,"-I", "dummy"])
            print ("Debug: VLC process started as PID %d for url '%s'" % (self.vlcproc.pid, url))
        else:
            print("Debug: can't play, there's already a VLC running")

            
    def pause(self):
        if self.vlcproc == None:
            return
        else:
            print ("Debug: tryin to kill VLC process PID %d ..." % (self.vlcproc.pid))
            self.vlcproc.kill()
            print ("Debug: killed VLC process PID %d." % (self.vlcproc.pid))
            self.vlcproc = None


    #plays the sound "asynchrounously" (kick the process and it will stop after playing)
    def playStartupSound(self):
        subprocess.Popen(["vlc", os.path.join("sounds", "351920__richerlandtv__o-s-start-up.mp3"),"-I", "dummy", "--play-and-exit"])

        
        
