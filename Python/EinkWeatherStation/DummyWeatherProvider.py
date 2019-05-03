from AbstractWeatherProvider import AbstractWeatherProvider

class DummyWeatherProvider (AbstractWeatherProvider):

    #Get CURRENT data
    def getCurrentWeather(self):
        return self.getDummyWeather()

    #returns the weather for the "next" period (to the discretion of implementation) 
    def getNextWeather(self):
        return self.getDummyWeather()


    #helper in case to get an pre-filled dummy object
    def getDummyWeather(self):
        wNow = dict()
        wNow["city_name"] = "Tokyo"
        wNow["temp"] = 23
        wNow["weather"] = "Grand soleil"        
        wNow["code"] = 800
        wNow["status"] = "Sunny"
        wNow["time"] = "now"
        return wNow