# OpenWeatherMap
# https://openweathermap.org/

from AbstractWeatherProvider import AbstractWeatherProvider
import json
import requests
import datetime
import sys

#placeholders for city and key
URL_CURRENT="https://api.openweathermap.org/data/2.5/weather?id=%s&appid=%s"
URL_5DAYS3H="https://api.openweathermap.org/data/2.5/forecast?id=%s&appid=%s"

KELVINZERO = -273.15

#Maps weather code to a easy to one word code
code2weather = {
	200: "Thunder",
	300: "Drizzle",
	500: "Rain",
	600: "Snow",
	700: "Mist",
	800: "Cloudy",
	900: "Unknown"
}

WBIT_DATETIME_PATTERN = "%Y-%m-%dT%H:%M:%S"

class OWMWeatherProvider(AbstractWeatherProvider):
    #attributes
    __CITYCODE=""
    __KEY=""

    #Get CURRENT data
    def getCurrentWeather(self):
       	response = requests.get(URL_CURRENT % (self.__CITYCODE, self.__KEY))
        j  = json.loads(response.text)
        
        try:
            resp = dict()
            resp["city_name"] = j["name"]

            self.__unwrapOneData(resp, j)
        except Exception:
            #log
            eType = sys.exc_info()[0]
            print ("ERROR! %s" % (eType))
            print("Response received;")
            print(j)
            
            #let it go to hell
            raise 

        return resp


    #returns the weather for the "next" period (to the discretion of implementation) 
    def getNextWeather(self):
        #just for now...
        return self.getCurrentWeather()

    #constructor
    def __init__(self, citycode, privatekey):
        self.__CITYCODE = citycode
        self.__KEY = privatekey

    
    #pass the initialized dict in resp
    def __unwrapOneData(self, resp, k):	
        resp["temp"] = float(k["main"]["temp"]) + KELVINZERO
        resp["weather"] = k["weather"][0]["description"]
            
        code = int(k["weather"][0]["id"])
        resp["code"] = int(k["weather"][0]["id"])

        if code < 800 or code > 801:
                code = (code / 100)*100

                resp["status"] = "Unknown" if not code in code2weather else code2weather[code]
        else:
                #800/801 are sunny, other 8xx are cloudy 
                resp["status"] = "Sunny"

        t = datetime.datetime.utcfromtimestamp(int(k["dt"])) + datetime.timedelta(hours=9)
        resp["time"] = t.strftime(WBIT_DATETIME_PATTERN)
        resp["datetimeobj"] = t
        
        return resp
