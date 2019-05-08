
import datetime
import time
from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont
import traceback
import os
import config

#some utils functions
import alan_utils
from designerSquares import drawWeatherPanel
from designer import  drawShutdownPanel, drawOthersPanel, drawEndPanel

from DummyWeatherProvider import DummyWeatherProvider
from WbitWeatherProvider import WbitWeatherProvider

################################################################################################3
##
##  Constants & Globals
##
################################################################################################3

#Tokyo
CITYCODE="1850147"
KEY=config.weatherio["key"]



#last time weather was updated
lastWeatherDT = None

#available panels and current one
PANELS = ["Weather", "Shutdown", "Others"]
currentPanelIdx = 0


################################################################################################3
##
##  Functions
##
################################################################################################3






    
################################################################################################3
##
##  Main entry point
##
################################################################################################3
if __name__ == '__main__':
    #start on weather
    currentPanelIdx = 0

    #img = drawShutdownPanel()

    #wprovider = DummyWeatherProvider()
    wprovider = WbitWeatherProvider(CITYCODE, KEY)

    wNow = wprovider.getCurrentWeather()    
    wLater = wprovider.getNextWeather()

    img = drawWeatherPanel(wNow, wLater).convert('1')
    
    img.show()            
    
    print("Good bye.")
        
