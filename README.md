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

`udpListener`, among the examples, already contains
a (working) implementation of UDP packets readers.

Once _include_d the `OSCMessage.h` it should
be easy parsing the data part (?) of the received
UDP packet.

## DOTO list

...

###### Notes:
+ *[1]* remember to set pin 10 in ether.begin


Check Brianza's code:

+ https://github.com/cylinderlight/udp2serial
+ https://github.com/cylinderlight/Flavin
+ 

Check OSC spec [here](http://opensoundcontrol.org/spec-1_0)

> The next example shows the 40 bytes in the representation of the OSC Message with
OSC Address Pattern "/foo" and 5 arguments:
  + The int32 1000
  + The int32 -1
  + The string "hello"
  + The float32 1.234
  + The float32 5.678
<pre> 2f (/)  66 (f)  6f (o)  6f (o)<br> 0 ()    0 ()    0 ()    0 ()<br> 2c (,)  69 (i)  69 (i)  73 (s)<br> 66 (f)  66 (f)  0 ()    0 ()<br> 0 ()    0 ()    3 ()    e8 (&egrave;)<br> ff (&yuml;)  ff (&yuml;)  ff (&yuml;)  ff (&yuml;)<br> 68 (h)  65 (e)  6c (l)  6c (l)<br> 6f (o)  0 ()    0 ()    0 ()<br> 3f (?)  9d ()   f3 (&oacute;)  b6 (&para;)<br> 40 (@)  b5 (&micro;)  b2 (&#148;)  2d (-)</pre>



CHECK !!!

http://cnmat.berkeley.edu/oscuino
