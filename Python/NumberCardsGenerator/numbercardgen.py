from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont
import os

FONT = "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"

#Params
BLOCK_PER_LINE=5
LINE_PER_PAGE=7
SPACER_PX=20

PAGE_WIDTH_PX=2100
PAGE_HEIGHT_PX=2970

START=1

######################################################################
#Globals
#Fonts
font_xbig = ImageFont.truetype(FONT, 256)

img = Image.new("RGB", (PAGE_WIDTH_PX, PAGE_HEIGHT_PX), (255,255,255))
draw = ImageDraw.Draw(img)

w = (PAGE_WIDTH_PX - BLOCK_PER_LINE * SPACER_PX) / BLOCK_PER_LINE
h = (PAGE_HEIGHT_PX - LINE_PER_PAGE * SPACER_PX) / LINE_PER_PAGE

val = START
for l in range(0, LINE_PER_PAGE):
    for c in range(0, BLOCK_PER_LINE):
        #top left
        x = SPACER_PX / 2 + (w + SPACER_PX) * c
        y= SPACER_PX /2 + (h + SPACER_PX) * l
        #border
        draw.rectangle([(x, y), (x+w, y+h)], fill="white", outline="black")
        #text
        txt = str(val)
        _,_,tw,th =draw.textbbox(xy=(0, 0), text=txt, font=font_xbig)
        #tw=th=0
        draw.text((x+(w - tw)/2, y +(h - th)/2), text=txt, font=font_xbig, fill="black")
        val = val +1

img.show()











