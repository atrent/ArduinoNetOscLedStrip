// Use this sketch to make Arduino
// listening to OSC messages on port 57000 ...

#include <OscUDP.h>

#include <SPI.h>        
#include <Ethernet.h>
#include <EthernetUdp.h>

#include <Adafruit_WS2801.h>

// Following two instructions vary on your configuration ...
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x22, 0xD7 };
IPAddress listeningIP(192,168,1,12);
unsigned int listeningPort = 57000;

// Wiring var's for ledstrip ...
uint8_t dataPin  = 2;
uint8_t clockPin = 3;

// Def+init of the ledstrip ...
Adafruit_WS2801 strip = Adafruit_WS2801(1, dataPin, clockPin);
// Using a 1-pixel ledstrip ...         ^

// Setup a UDP object ...
EthernetUDP UDP;
// Our OSC-message object ...
OscUDP etherOSC;



void setup() {
  
  // Local debug stuff ...
  Serial.begin(9600);
  
  Ethernet.begin(mac,listeningIP);
  
  // Print Arduino's IP ...
  //Serial.println(Ethernet.localIP());
  
  // Start UDP object, listening on port listeningPort ...
  UDP.begin(listeningPort);
  
  // Pass the UDP object to OSC ...
  etherOSC.begin(UDP);
  
  Serial.println("Starting ...");
  
  strip.begin();
  strip.show();
  
}

void loop() {
  
  etherOSC.listen();

  delay(100);

}


void oscEvent(OscMessage &m) {
  
  Serial.println("Got a message!");
  m.plug("/ledstrip", theCallback); 
  
}


void theCallback(OscMessage &m) {
  
  Serial.println("Address: ledstrip");
  
  strip.setPixelColor(m.getInt(0),Color(m.getInt(1), m.getInt(2), m.getInt(3)));
  strip.show();
  
}

// Create a 24 bit color value from R,G,B ...
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}
