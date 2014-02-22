# ArduinoNetOscLedStrip (on development)

## Abstract

Putting together these Arduino libraries:

- [EtherCard](https://github.com/jcw/ethercard.git).
  See article [here](http://www.komputer.de/wordpress/archives/497) *[1]*
- A OSC receiver library (maybe [this](http://cnmat.berkeley.edu/oscuino)
  one)
- ledstrip (i.e. *Adafruit_WS2801*)

to create a ledstrip controlled from the local.net (or the Internet),
to drive different light patterns, associating them with
generic *events*
(e.g. new email received,
hard-disk's full,
home power consumption too high, etc.)

## TODO list

+ Making arduino able to receive UDP
packets.
+ Writing (say, stealing) an OSC interpreter
for the controller.
+ Writing an interface between the OSC interpreter
and the ledstrip controller.

## DOTO list

...

###### Notes:
+ *[1]* remember to set pin 10 in ether.begin


Check Brianza's code:

+ https://github.com/cylinderlight/udp2serial
+ https://github.com/cylinderlight/Flavin
