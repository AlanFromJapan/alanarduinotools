from AbstractWeatherProvider import AbstractWeatherProvider
import json
import requests
import datetime

#placeholders for city and key
URL_CURRENTWEATHER="https://api.weatherbit.io/v2.0/current?city_id=%s&key=%s"
URL_1DAYS3H="https://api.weatherbit.io/v2.0/forecast/3hourly?city_id=%s&days=1&key=%s"
URL_HOURLY="https://api.weatherbit.io/v2.0/forecast/hourly?city_id=%s&hours=%d&key=%s"

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

class WbitWeatherProvider(AbstractWeatherProvider):
    #attributes
    __CITYCODE=""
    __KEY=""

    #Get CURRENT data
    def getCurrentWeather(self):
       	response = requests.get(URL_CURRENTWEATHER % (self.__CITYCODE, self.__KEY))
        j  = json.loads(response.text)
        
        try:
            resp = dict()
            resp["city_name"] = j["data"][0]["city_name"]

            self.__unwrapOneData(resp, j["data"][0])
        except Exception:
            #log
            #print ("Response.text=" + response.text)
            #and continue
            raise

        return resp


    #returns the weather for the "next" period (to the discretion of implementation) 
    def getNextWeather(self):
        deltah = 9

        #find the right "next time": evening I want next morning, morning afternoon, afternoon evening
        now = datetime.datetime.now()
        if now.hour >=18:
            deltah = 24+9 - now.hour
        elif now.hour <= 7:
            deltah = 9 - now.hour
        elif now.hour <= 12:
            deltah = 14 - now.hour
        elif now.hour <= 17:
            deltah = 20 - now.hour


        response = requests.get(URL_HOURLY % (self.__CITYCODE, deltah, self.__KEY))
        j  = json.loads(response.text)

        #print ("DEBUG: " + response.text)
            
        resp = dict()
        
        resp["city_name"] = j["city_name"]

        #go through them all, return the last
        for d in j["data"]:
            n = self.__unwrapOneData(resp, d)

        return resp


    #constructor
    def __init__(self, citycode, privatekey):
        self.__CITYCODE = citycode
        self.__KEY = privatekey

    
    #pass the initialized dict in resp
    def __unwrapOneData(self, resp, k):	
        resp["temp"] = k["temp"]
        resp["weather"] = k["weather"]["description"]
            
        code = int(k["weather"]["code"])
        resp["code"] = int(k["weather"]["code"])

        if code < 800 or code > 801:
                code = (code / 100)*100

                resp["status"] = "Unknown" if not code in code2weather else code2weather[code]
        else:
                #800/801 are sunny, other 8xx are cloudy 
                resp["status"] = "Sunny"

        resp["time"] = datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S") if not "timestamp_local" in k else k["timestamp_local"]

        return resp

