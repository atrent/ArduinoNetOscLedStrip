// Code largely inspired by the udpListener.ino sketch
// you can find in his library "EtherCard"
//
// Most of the code here has been written by
// Brian Lee <cybexsoft@hotmail.com> in 2013-4-7

#include <EtherCard.h>
#include <IPAddress.h>

#include <OSCMessage.h>

/* Network-unique MAC address for ENC28J60 controller */
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

/* TCP/IP send/receive buffer */
byte Ethernet::buffer[500];

/* Defining a constant for a LED. I find it useful on testing */
const unsigned int led=7;

/* Callback that prints received packets to the serial port */
void udpSerialPrint(word port, byte ip[4], const char *data, word len) {
  IPAddress src(ip[0], ip[1], ip[2], ip[3]);
  Serial.println(src);
  Serial.println(port);
  Serial.println(data);
  Serial.println(len);
}

/* Callback that changes state of a led depending on a hardcoded string */
void udptoGPIO(word port, byte ip[4], const char *data, word len) {

  // Local debug here
  Serial.print("Content of datagram: ");
  Serial.println(data);
  Serial.print("Its length: ");
  Serial.println(len);

  if(!strcmp(data, "led.on"))
    digitalWrite(led, HIGH);
    
  if(!strcmp(data, "led.off"))
    digitalWrite(led, LOW);
    
}

/* Callback that get the OSC content of a datagram */
void udptoOSC(word port, byte ip[4], const char *data, word len) {
  
  // Local debug here
  Serial.println("Content of datagram: ");
  Serial.println(data);
  
  OSCMessage msg(...);
  msg.fill((uint8_t*)data, len);
  
  // The following does not work at the moment ...
  /* if(msg.match("/led/on")) { digitalWrite(led, HIGH); } */
}

void setup(){
  Serial.begin(57600);
  Serial.println("\n[backSoon]");
  
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
    Serial.println( "Failed to access Ethernet controller");

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

  /* Register udpSerialPrint() to port 1337 */
  ether.udpServerListenOnPort(&udpSerialPrint, 1337);

  /* Register udpSerialPrint() to port 42 */
  ether.udpServerListenOnPort(&udpSerialPrint, 42);

  /* Setup udptoGPIO stuff */
  pinMode(led, OUTPUT);

  /* Register udptoGPIO() to port 5000 */
  ether.udpServerListenOnPort(&udptoGPIO, 5000);
  
  /* Register udptoOSC() to port 8888 */
  ether.udpServerListenOnPort(&udptoOSC, 8888);
}

void loop(){
  /* This MUST be called for ethercard functions to work */
  ether.packetLoop(ether.packetReceive());
}
