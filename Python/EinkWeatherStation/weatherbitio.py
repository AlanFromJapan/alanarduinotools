import json
import requests

#placeholders for city and key
URL_CURRENTWEATHER="https://api.weatherbit.io/v2.0/current?city_id=%s&key=%s"
URL_1DAYS3H="https://api.weatherbit.io/v2.0/forecast/3hourly?city_id=%s&days=1&key=%s"

#Maps weather code to a easy to one word code
code2weather = {
	200: "Thunder",
	300: "Drizzle",
	500: "Rain",
	600: "snow",
	700: "Mist",
	800: "Cloudy",
	900: "Unknown"

}


#pass the initialized dict in resp
def unwrapOneData(resp, k):	
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

	resp["time"] = "now" if not "timestamp_local" in k else k["timestamp_local"]

	return resp


#Get CURRENT data
def getCurrentWeather(pKey, pCity):
	response = requests.get(URL_CURRENTWEATHER % (pCity, pKey))
	j  = json.loads(response.text)

	resp = dict()
	resp["city_name"] = j["data"][0]["city_name"]

	unwrapOneData(resp, j["data"][0])

	return resp


#Get the weather for the next 24h with a status every 3h 
def getNext24hby3h(pKey, pCity):
	response = requests.get(URL_1DAYS3H % (pCity, pKey))
	j  = json.loads(response.text)

        print ("DEBUG: " + response.text)
        
	resp = dict()
	
	resp["city_name"] = j["city_name"]
	resp["data"] = list()

	for d in j["data"]:
		n = unwrapOneData(dict(), d)

		#resp["data"][n["time"]] = n
		resp["data"].append(n)

	
	return resp
