import io
import datetime
import time
import json
import requests
import sys

import weatherbitio as wbit

#Tokyo
CITYCODE="1850147"


#placeholders for city and key
URL_CURRENTWEATHER="https://api.weatherbit.io/v2.0/current?city_id=%s&key=%s"
URL_2DAYS3H="https://api.weatherbit.io/v2.0/forecast/3hourly?city_id=%s&days=1&key=%s"


def showOne(weather):
	print(weather)
	print("City = " + weather["city_name"] if "city_name" in weather else "?")
	print("Current temp  = " + str(weather["temp"]))
	print("Current weather  = " + weather["weather"])
	print("Current status = %s (%d)" % (weather["status"], weather["code"]))
	print ("Time = %s" % (weather["time"]))


def testToday(pKey):
	print("**TODAY**")
	result = wbit.getDaily(pKey, CITYCODE)
	showOne(result)


def loadJson(filename):
    t = ""
    with open(filename, 'r') as f:
    	t = f.read()
    return json.loads(t)

def testNext (pKey):
	print ("** TEST next **")

	"""
	j = loadJson("sample_next.json")
	for d in j["data"]:
		print ("%s temp %s : %s" % (d["timestamp_local"], d["temp"], d["weather"]["description"]))
	"""

	result = wbit.getNext(pKey, CITYCODE)
	for n in result["data"]:
		showOne(result["data"][n])
		print ("-------------------------------------------------------------")


def main(pKey, pData=""):
	#testToday(pKey)
	testNext(pKey)



if __name__ == '__main__':
    if len(sys.argv) == 1:
        print("No key, will use sample data")
        sample = ""
        with open('sample_daily.json', 'r') as f:
        	sample = f.read()

        main(pKey=None, pData=sample)
    else:
    	vKey = str(sys.argv[1])
    	main(pKey=vKey)
