
import json
import urllib2
import urllib


#headers = {'Content-type': 'application/json'}

foo = { "id" : "1", "jsonrpc" : "2.0", "method": "Input.Up" } 
json_foo = json.dumps(foo)

x = "%7B%22jsonrpc%22%3A%20%222.0%22%2C%20%22id%22%3A%20%221%22%2C%20%22method%22%3A%20%22Input.Up%22%7D"
#connection.request('GET', '/request=' + json_foo.encode(), json_foo, headers)
#reqstring = "http://192.168.0.5:8090/jsonrpc?request=" + str(json_foo)
#reqstring = "http://192.168.0.5:8090/jsonrpc?request=" + x

print ("json string is " + str(json_foo))
reqstring = "http://192.168.0.5:8090/jsonrpc?request=" + urllib.quote_plus(str(json_foo))

print ("About to call > " + reqstring)


req = urllib2.Request(reqstring)
req.add_header("Authorization", "Basic eGJtYzpkZWphbmVpcm8=")
response = urllib2.urlopen (req)
print(response.read().decode())
