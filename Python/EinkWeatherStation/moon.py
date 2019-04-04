import epd2in13
import datetime
import time
import Image
import ImageDraw
import ImageFont
import traceback
import os
import config


#Supposedely calculates moon phases ... just plain wrong :(
def moonPhase1(d):
    #http://www.voidware.com/moon_phase.htm
    y = d.year
    m = d.month
    d = d.day

    if m < 3:
        y = y -1
        m = m + 12
    m = m + 1
    c = 365.25 * y
    e = 30.6 * m
    jd  = c + e + d - 694039.09
    jd = jd / 29.53

    jd = jd - float(int (jd))
    b = jd * 8 + 0.5
    b = b % 8
    return b


#Draws the moon according it's phase (it's a square)
def drawMoon(draw, font, image_width, image_height):
    phase = moonPhase1(now)
    basex = image_width - 50
    basey = 70
    moonw = 40
    moonh = 35
    draw.text((basex + 4, basey + moonh + 1), "Moon", font=font, fill = 0)
    #ext rectangle
    draw.rectangle ([ (basex, basey) , (basex + moonw, basey + moonh)], outline = 0)
    #in color
    if phase == 0:
        #new moon - all black
        draw.rectangle ([ (basex +2 , basey+2) , (basex + moonw - 2, basey + moonh-2 )], outline = 0, fill = 0)
    elif phase == 1:
        draw.rectangle ([ (basex +2 , basey+2) , (basex + (moonw * 0.75) - 2, basey + moonh-2 )], outline = 0, fill = 0)
    elif phase == 2:
        draw.rectangle ([ (basex +2 , basey+2) , (basex + (moonw * 0.5) - 2, basey + moonh-2 )], outline = 0, fill = 0)
    elif phase == 3:
        draw.rectangle ([ (basex +2 , basey+2) , (basex + (moonw * 0.25) - 2, basey + moonh-2 )], outline = 0, fill = 0)
    elif phase == 4:
        #fullmoon
        pass
    elif phase == 5:
        draw.rectangle ([ (basex +2 + (moonw * 0.75) , basey+2) , (basex + moonw - 2, basey + moonh-2 )], outline = 0, fill = 0)
    elif phase == 6:
        draw.rectangle ([ (basex +2 + (moonw * 0.5) , basey+2) , (basex + moonw - 2, basey + moonh-2 )], outline = 0, fill = 0)
    elif phase == 7:
        draw.rectangle ([ (basex +2 + (moonw * 0.25) , basey+2) , (basex + moonw - 2, basey + moonh-2 )], outline = 0, fill = 0)

