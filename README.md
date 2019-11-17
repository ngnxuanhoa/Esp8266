# Esp8266
Code for Esp8266 MCU
Control relay module by mobile application & web server

Upload code for ESP 8266 MCU

Using Android App: https://play.google.com/store/apps/details?id=com.i.myhome_socketio&hl=vi 
to Control device to ESP8266.

git clone https://github.com/ngnxuanhoa/iQQSmartHome --> Web server --> Upto your hosting (test in heroku - free)

V2:

THIETBI --> get all devices status

THIETBI0 --> Control output1

THIETBI1 --> Control output2

THIETBI2 --> Control output3

THIETBI3 --> Control output4

....

V5:

THIETBI --> get all devices status

set Output here --> byte output[] = {14, 12, 13, 15, 2};

ESP Library:
https://github.com/ngohuynhngockhanh/Socket.io-v1.x-Library

https://github.com/bblanchon/ArduinoJson

https://github.com/tzapu/WiFiManager
