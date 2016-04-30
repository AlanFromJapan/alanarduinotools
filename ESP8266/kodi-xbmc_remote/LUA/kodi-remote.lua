-- LUA code for ESP8266 for controlling Kodi
-- by AlanFromJapan - 2016/04/26
-- http://electrogeek.cc/kodi-xbmc%20remote.html

------------------------------------------------------------------------
--
function callURL (ip, port, token, command)
conn = nil
conn=net.createConnection(net.TCP, 0) 

-- show the retrieved web page
--conn:on("receive", 
--	function(conn, payload) 
		-- print(string.sub(payload, 1, 500)) 
--	end) 

-- when connected, request page (send parameters to a script)
conn:on("connection", 
	function(conn, payload) 
		print('\nTCP Connected')
		httpReq = "GET "..command.." HTTP/1.1\r\n" --Get /xxxx HTTP/1.1 
		.."Host: "..ip.."\r\n"
		.."Authorization: Basic "..token.."\r\n"
		.."Connection: close\r\n"
		.."\r\n" --FINAL is needed!

		conn:send(httpReq)
	end) 

-- when disconnected, let it be known
conn:on("disconnection", 
	function(conn, payload) 
		print('\nTCP Disconnected') 
	end)

conn:connect(port,ip) 

--do not close manually
--conn:close()
--conn = nil


end --/callKodiRemoteURL


--------------------------------------------------------------------
--Url encodes a string https://gist.github.com/ignisdesign/4323051
--
function urlencode(str)
   if (str) then
      str = string.gsub (str, "\n", "\r\n")
      str = string.gsub (str, "([^%w ])",
         function (c) return string.format ("%%%02X", string.byte(c)) end)
      str = string.gsub (str, " ", "+")
   end
   return str    
end


---------------------------------------------------------------------
--
function callKodiRemote (ip, port, token, command)

  callURL (ip, port, token, "/jsonrpc?request="..urlencode(command)) 

end 