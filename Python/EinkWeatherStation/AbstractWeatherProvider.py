
class AbstractWeatherProvider:

    #Get CURRENT data
    def getCurrentWeather(self):
        raise NotImplementedError

    #returns the weather for the "next" period (to the discretion of implementation) 
    def getNextWeather(self):
        raise NotImplementedError


