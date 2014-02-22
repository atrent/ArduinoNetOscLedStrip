ArduinoNetOscLedStrip
=====================


Put together these Arduino libraries:

- ethernet (https://github.com/jcw/ethercard.git), article (pinout): http://www.komputer.de/wordpress/archives/497 [*]
- osc
- ledstrip (Adafruit_WS2801)

To create a ledstrip controlled from the local.net (or Internet), to drive different light patterns associating them to generic "events" (such as an email, a disk full, home power consumption to high, etc.)







Notes:
+ [*] remember to set pin 10 in ether.begin
+ maybe Arduino OSC 
might be a good choice for managing OSC msg's.
    + http://cnmat.berkeley.edu/oscuino
    + (fork) http://www.deadpixel.ca/arduino-osc/



check Brianza's code:
https://github.com/cylinderlight/udp2serial
https://github.com/cylinderlight/Flavin
