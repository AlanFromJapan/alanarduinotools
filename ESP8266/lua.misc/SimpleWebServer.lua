-- a simple http server
srv=net.createServer(net.TCP) 
srv:listen(80,function(conn) 
    conn:on("receive",function(conn,payload) 
    print(payload) 
    conn:send("<h1> Hello, monde.</h1>")
    end) 
end)