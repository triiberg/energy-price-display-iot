# energy-price-display-iot

This little guy uses REST service https://github.com/triiberg/energy-pricings and displays the data on a tiny screen.

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/-jWOiwHyNCo/0.jpg)](https://www.youtube.com/watch?v=-jWOiwHyNCo)

Config:
* find REST service URL and change it
* change Wifi creds

Hardware parts:
* ESP8266WiFi
* OLED Shield V1.1.0

Software dependencies (not sure if all of them are in use):
* SPI.h
* Wire.h
* Adafruit_GFX.h
* Adafruit_SSD1306.h
* ESP8266WiFi.h
* WiFiClient.h
* ArduinoJson.h

Problems:
* Didn't find and good HTTP libra, wrote a function instead (with blackjack and ...)

Will merge your pull requests:
* if you fix Wifi creds and REST urls
* find a bug in getData function (it works quite well imho, but not really tested)
