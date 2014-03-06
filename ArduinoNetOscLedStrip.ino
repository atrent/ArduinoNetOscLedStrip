// Credits to Brian Lee <cybexsoft@hotmail.com>

/* Including ENC28J60 libraries */
#include <EtherCard.h>
#include <IPAddress.h>

/* Including OSC interpreter libraries */
#include <OSCMessage.h>
#include <OSCBundle.h>

/* Including Strip-led libraries */
#include <SPI.h>
#include <Adafruit_WS2801.h>

/* LAN-unique MAC address for ENC28J60 controller */
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

/* TCP/IP send/receive buffer */
byte Ethernet::buffer[500];

/* Defining a constant for a LED. Maybe useful on testing */
const unsigned int led=7;

/* Setup strip-led stuff */
uint8_t dataPin = 2; // Yellow wire, on Adafruit-WS2801
uint8_t clockPin = 3; // Green wire, on Adafruit-WS2801

/* I'm using a strip of length 3 at the moment */
const int length = 3;
Adafruit_WS2801 strip = Adafruit_WS2801(length, dataPin, clockPin);

/* This function return a 24bit color value from parameters r, g and b */
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

/* Callback that prints received packets to the serial port */
void udpSerialPrint(word port, byte ip[4], const char *data, word len) {
  IPAddress src(ip[0], ip[1], ip[2], ip[3]);
  Serial.println(src);
  Serial.println(port);
  Serial.println(data);
  Serial.println(len);
}

/* Callback that changes state of a led depending on the received OSC message */
void udptoGPIO(word port, byte ip[4], const char *data, word len) {

  // Local debug here
  Serial.print("Content of datagram: ");
  Serial.println(data);
  Serial.print("Its length: ");
  Serial.println(len);

  OSCMessage msg("/led");
  
  msg.fill((uint8_t*)data, len);

  /* If OSC message contains 1, then switch the led on */
  int ledStatus = msg.getInt(0);
  if (ledStatus == HIGH) {
    digitalWrite(led, HIGH);
  } else if (ledStatus == LOW){
    digitalWrite(led, LOW);
  }
  
}

void udptoStripled(word port, byte ip[4], const char *data, word len) {
    /* Though stupid, silly, faggot, illiterate ...it works! */
    OSCMessage msg(data);

    if(msg.fullMatch("/single_led")) {
    
        /* Checking correct sintax */
	//...
	
	/* If syntax check fails, print an error msg and return */
	//...
	
	/* else, fill msg and update strip */
	msg.fill((uint8_t*)data, len);
	int numLed = msg.getInt(0);
	int r = msg.getInt(1);
	int g = msg.getInt(2);
	int b = msg.getInt(3);
	
	// Local debug
	Serial.print("numLed: ");
	Serial.println(numLed);
	Serial.print("r: ");
	Serial.println(r);
	Serial.print("g: ");
	Serial.println(g);
	Serial.print("b: ");
	Serial.println(b);

        /* TODO: Colours get interpreted in reverse order ...don't know why. */	
	strip.setPixelColor(numLed, Color((uint8_t)r,(uint8_t)g,(uint8_t)b));
	strip.show();
	
    } else if (msg.fullMatch("/off")) {
        
	for (int i=0; i < length; i++) {
	    strip.setPixelColor(i, Color(0,0,0));
	    strip.show();
	}
	
    } else if (msg.fullMatch("/all")) {
        //...
    } else if (msg.fullMatch("/ping_frw")) {
        //...
    } else if (msg.fullMatch("/ping_rev")) {
        //...
    } else {
        Serial.println("Unknown address for received OSC msg.");
    }
}

void setup(){
  Serial.begin(57600);
  Serial.println("\n[backSoon]");
  
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
    Serial.println( "Failed to access Ethernet controller");

  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

  /* Register udpSerialPrint() to port 1337 */
  ether.udpServerListenOnPort(&udpSerialPrint, 1337);

  /* Setup GPIO stuff ... */
  pinMode(led, OUTPUT);


// forse per il finale farei una sola registrazione di una funzione che dispatcha, a meno che non si possano (ma non credo) agganciare piu' funzioni allo stesso interrupt (porta)

  /* ... and register udptoGPIO() to port 8888 */
  ether.udpServerListenOnPort(&udptoGPIO, 8888);

  /* Setup ledstrip stuff ... */
  strip.begin();
  strip.show();

  /* ... and register udptoStripled() to port 57600 */
  ether.udpServerListenOnPort(&udptoStripled, 57600);
}

void loop(){
  /* This MUST be called for ethercard functions to work */
  ether.packetLoop(ether.packetReceive());
  
  // un delay?
}
