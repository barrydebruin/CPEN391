-- This information is used by the Wi-Fi dongle to make a wireless connection to the router in the Lab
-- or if you are using another router e.g. at home, change ID and Password appropriately
SSID = "testwifi"
SSID_PASSWORD = "12345678"

-- configure ESP as a station
wifi.setmode(wifi.STATION)
wifi.sta.config(SSID,SSID_PASSWORD)
wifi.sta.autoconnect(1)

-- pause for connection to take place - adjust time delay if necessary or repeat until connection made
tmr.delay(1000000) -- wait 1,000,000 us = 1 second

tmr.alarm(0, 1000, 1, function()
   if wifi.sta.getip() == nil then
      print("Connecting to AP...\n")
   else
      print("connected\n")
      tmr.stop(0)
   end
end)

-- header and url for wifi switch
header = 'Content-Type: application/json\r\n'
URL = 'https://use1-wap.tplinkcloud.com/?token=2d2c8422-A6AWd80Sco51JaW7L7yQ5B0'


function light_on()

    ok, json1 = pcall(cjson.encode, {deviceId="800616E52D10AABA186645E5A5BFFB6617ECFA24", requestData="{\"system\":{\"set_relay_state\":{\"state\":1}}}"})
    if ok then
      print(json1)
    else
      print("failed to encode!")
    end
    ok, data_on = pcall(cjson.encode, {method="passthrough", params=cjson.decode(json1)})
    if ok then
      print(json)
    else
      print("failed to encode!")
    end

    http.request(URL,"POST",header,data_on,
    function(code, data)
    if (code < 0) then
      print("HTTP request failed")
    else
      print(code, data)
    end
  end)

end

function light_off()

  ok, json1 = pcall(cjson.encode, {deviceId="800616E52D10AABA186645E5A5BFFB6617ECFA24", requestData="{\"system\":{\"set_relay_state\":{\"state\":0}}}"})
    if ok then
      print(json1)
    else
      print("failed to encode!")
    end
    ok, data_off = pcall(cjson.encode, {method="passthrough", params=cjson.decode(json1)})
    if ok then
      print(data1)
    else
      print("failed to encode!")
    end

  http.request(URL,"POST",header,data_off,
  function(code, data)
    if (code < 0) then
      print("HTTP request failed")
    else
      print(code, data)
    end
  end)

end
