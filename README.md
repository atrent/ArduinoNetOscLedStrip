ArduinoNetOscLedStrip
=====================


Put together these Arduino libraries:

- ethernet
- osc
- ledstrip (Adafruit_WS2801)

To create a ledstrip controlled from the local.net (or Internet), to drive different light patterns associating them to generic "events" (such as an email, a disk full, home power consumption to high, etc.)







Notes:
+ remember to set pin 10 in ether.begin
+ maybe [Arduino OSC](http://www.deadpixel.ca/arduino-osc/)
might be a good choice for managing OSC msg's.
