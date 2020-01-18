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
        except Exception, e:
            #log
            eType = sys.exc_info()[0]
            print ("ERROR! %s - %s" % (eType, str(e)))
            print("Response received;")
            print(j)
            
            #let it go to hell
            raise 

        return resp


    #returns the weather for the "next" period (to the discretion of implementation) 
    def getNextWeather(self):
        j = "{nothing}"
        try:
            #find the right "next time": evening I want next morning, morning afternoon, afternoon evening
            now = datetime.datetime.now()
            if now.hour >=18:
                #get tomorrow 9am
                later = datetime.datetime(now.year, now.month, now.day, 9) + datetime.timedelta(days=1)
            elif now.hour >= 12:
                #afternoon give me today evening
                later = datetime.datetime(now.year, now.month, now.day, 21)
            elif now.hour >= 6:
                #orning give me this noon
                later = datetime.datetime(now.year, now.month, now.day, 12)
            else:
                #before 6am, give me today 9am
                later = datetime.datetime(now.year, now.month, now.day, 9)

            #print ("DEBUG : later is JST  %s" % (later.strftime("%Y-%m-%d %H:%M:%S")))
            
            response = requests.get(URL_5DAYS3H % (self.__CITYCODE, self.__KEY))
            j  = json.loads(response.text)

            resp = dict()
        
            resp["city_name"] = j["city"]["name"]


            #go through them all, return the one just after "later"
            for d in j["list"]:
                self.__unwrapOneData(resp, d)

                #print ("DEBUG : JST %s" % (resp["datetimeobj"].strftime("%Y-%m-%d %H:%M:%S")))
                if resp["datetimeobj"] >= later:
                    break

            return resp
        except Exception, e:
            #log
            eType = sys.exc_info()[0]
            print ("ERROR! %s - %s" % (eType, str(e)))
            print("Response received;")
            print(j)
            
            #let it go to hell
            raise 




        

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
