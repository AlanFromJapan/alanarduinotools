-- LUA code for ESP8266 for controlling Kodi
-- by AlanFromJapan - 2016/04/26
-- http://electrogeek.cc/kodi-xbmc%20remote.html

function callKodiRemoteURL (ip, port, token, command)
conn = nil
conn=net.createConnection(net.TCP, 0) 

-- show the retrieved web page

conn:on("receive", function(conn, payload) 
                       success = true
                       print(strsub(payload, 1, 50)) 
                       end) 

-- when connected, request page (send parameters to a script)
conn:on("connection", function(conn, payload) 
                       print('\nConnected') 
                       conn:send("GET "..command.."\r\n"
                        .." HTTP/1.1\r\n" 
						.."Host: "..ip.."\r\n"
               		    .."Connection: close\r\n"
                        .."Accept: text/html\r\n" 
                        .."User-Agent: Mozilla/4.0 (compatible; esp8266 Lua; Windows NT 5.1)\r\n" 
						.."Pragma: no-cache\r\n"
						.."Cache-Control: no-cache\r\n"
                        .."\r\n")
                       end) 
					   
-- when disconnected, let it be known
conn:on("disconnection", function(conn, payload) print('\nDisconnected') end)
                                             
conn:connect(port,ip) 

end --/callKodiRemoteURL