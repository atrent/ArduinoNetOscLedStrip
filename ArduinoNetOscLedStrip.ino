// Credits to Brian Lee <cybexsoft@hotmail.com> //foo

// http://liblo.sourceforge.net/
// oscsend 159.149.159.213 57600 /bargraph iiii $LOAD 255 255 255

// https://github.com/adafruit/Adafruit-WS2801-Library

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

// nr. of LEDs
const int length = 20;

// config
Adafruit_WS2801 strip = Adafruit_WS2801(length, dataPin, clockPin);

/* This function return a 24bit color value from parameters r, g and b */
uint32_t Color(byte r, byte g, byte b) {
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
    } else if (ledStatus == LOW) {
        digitalWrite(led, LOW);
    }

}

void udptoStripled(word port, byte ip[4], const char *data, word len) {

    // DEBUG ...
    Serial.println("Received UDP packet.");

    /* Though stupid, silly, faggot, illiterate ...it works! */
    OSCMessage msg(data);
    msg.fill((uint8_t*)data, len);

    /* TODO Checking correct sintax */

    if(msg.fullMatch("/single_led")) {
        /////////////////////////////////
        // param: INDEX R G B (tutti int)

        //msg.fill((uint8_t*)data, len);

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
        strip.setPixelColor(numLed, Color((uint8_t)b,(uint8_t)g,(uint8_t)r)); // BUG R-B
        strip.show();

    } else if (msg.fullMatch("/off")) {

        for (int i=0; i < length; i++) {
            strip.setPixelColor(i, Color(0,0,0));
            strip.show();
        }

    } else if (msg.fullMatch("/all")) {
        /////////////////////////////////
        // param: R G B (tutti int)

        int r = msg.getInt(0);
        int g = msg.getInt(1);
        int b = msg.getInt(2);

        // DEBUG ...
        Serial.print("Red: ");
        Serial.println(r);
        Serial.print("Blue: ");
        Serial.println(b);
        Serial.print("Green: ");
        Serial.println(g);

        for (int i=0; i < length; i++) {
            strip.setPixelColor(i, Color((uint8_t)b,(uint8_t)g,(uint8_t)r)); // BUG R-B
            strip.show();
        }

    } else if (msg.fullMatch("/ping_frw")) {
        /////////////////////////////////
        // param: R G B INCR (tutti int)

        int r = msg.getInt(0);
        int g = msg.getInt(1);
        int b = msg.getInt(2);
        int incr = msg.getInt(3);

        // DEBUG ...
        Serial.print("Red: ");
        Serial.println(r);
        Serial.print("Blue: ");
        Serial.println(b);
        Serial.print("Green: ");
        Serial.println(g);
        Serial.print("Increment: ");
        Serial.println(incr);

        for (int i=0; i < length; i++) {
            /*
            int tmpRed = r * i;
            int tmpGreen = g * i;
            int tmpBlue = b * i;
            */

            if(i!=0) strip.setPixelColor(i-1,Color(0,0,0));

            strip.setPixelColor(i,Color((uint8_t)b, (uint8_t)g, (uint8_t)r)); // BUG R-B
            b=b+incr;
            g=g+incr;
            r=r+incr;

            strip.show();

            delay(20);
        }
        strip.setPixelColor(length-1,Color(0,0,0));
        strip.show();
    } else if (msg.fullMatch("/ping_rev")) {
        //...
    } else if (msg.fullMatch("/bargraph")) {
        /////////////////////////////////
        // param: PERCENT R G B (tutti int)

        int percent = msg.getInt(0);
        int r = msg.getInt(1);
        int g = msg.getInt(2);
        int b = msg.getInt(3);

        int quanti=length*percent/100;
        // TODO eventualmente fattorizzare i vari for nei vari if
        for (int i=0; i < length; i++) {
            if(i<=quanti) {
                strip.setPixelColor(i,
                                    Color((uint8_t)b, (uint8_t)g, (uint8_t)r)); // BUG R-B
            }
            else {
                strip.setPixelColor(i, Color(50,0,0)); //BUG R-B
            }
            strip.show();
        }

    } else {
        Serial.println("Unknown address for received OSC msg.");
    }
}

void setup() {
    Serial.begin(57600);
    Serial.println("\n[backSoon]");

    if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
        Serial.println( "Failed to access Ethernet controller");

    if (!ether.dhcpSetup()) {
        Serial.println("DHCP failed");
        // set fixed 192.168.10.10
        //ether.staticSetup({192,168,10,10},...,...);
    }

    ether.printIp("IP:  ", ether.myip);
    ether.printIp("GW:  ", ether.gwip);
    ether.printIp("DNS: ", ether.dnsip);

    /* Register udpSerialPrint() to port 1337 */
    //ether.udpServerListenOnPort(&udpSerialPrint, 1337);

    /* Setup GPIO stuff ... */
    pinMode(led, OUTPUT);


// forse per il finale farei una sola registrazione di una funzione che dispatcha, a meno che non si possano (ma non credo) agganciare piu' funzioni allo stesso interrupt (porta)

    /* ... and register udptoGPIO() to port 8888 */
    //ether.udpServerListenOnPort(&udptoGPIO, 8888);

    /* Setup ledstrip stuff ... */
    strip.begin();
    strip.show();

    /* ... and register udptoStripled() to port 57600 */
    ether.udpServerListenOnPort(&udptoStripled, 57600);
}

void loop() {
    /* This MUST be called for ethercard functions to work */
    ether.packetLoop(ether.packetReceive());

    /*
    for (int pxIndex=0; pxIndex < length; pxIndex++) {
        strip.setPixelColor(pxIndex, Color((uint8_t)(pxIndex%255),(uint8_t)(pxIndex%255),(uint8_t)(pxIndex%255)));
        strip.show();
        delay(10);
    }
    */

    /* Due to the speed of Arduino, we always feel better if we loop */
    delay(100);
}
