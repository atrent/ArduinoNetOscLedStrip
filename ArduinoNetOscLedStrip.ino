// Credits to Brian Lee <cybexsoft@hotmail.com>

#include <EtherCard.h>
#include <IPAddress.h>

#include <OSCMessage.h>
#include <OSCBundle.h>

/* Network-unique MAC address for the ENC28J60 controller */
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

/* TCP/IP send/receive buffer */
byte Ethernet::buffer[500];

/* Defining a constant for a LED. Maybe useful on testing */
const unsigned int led=7;

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

// a occhio:
// - ip e' indirizzo IP del mittente, NON il data ;)
// - data e' data

  // Local debug here
  Serial.print("Content of datagram: ");
  Serial.println(data);
  Serial.print("Its length: ");
  Serial.println(len);

  OSCMessage msg("/led"); // a questo punto perche' inizializzarlo se tanto lo riempi subito dopo?
  // sono obbligato. OSCMessage() vuole l'address
  // come argomento.
  // OK
  
  // Sospetti msg venga riempito come
  // /led/led/on ?
  // non so, se lo stampi dopo cosa viene?
  // Roba poco leggibile -- a parte l'address
  // mentre prima era leggibile?
  // no, neanche udpSerialPrint() -- che
  // era gia' scritta dall'autore -- riusciva
  // a stampare correttamente il contenuto
  // del messaggio (a parte l'address).
  // OK
  // Tuttavia, i dati numerici nel messaggio
  // riesci a 'retrieve'arli.
  // ok, a questo punto prova con un match invece di fullmatch e vedi che ti butta fuori (un int)
  // Si, sempre zero.
  // Sia cercando match con "/led/on", "/led/led/on",
  // "/*/on", etc...
  // getAddress cosa ti dice?
  // Cacchio, non ho provato -- e sono in Comelico, non ho lan.
  // ok
  
  
  // BTW poi lascia tutti questi commenti anche nel git che magari ci servono
  // ok
  
  
  // per curiosita' prova a compilare e testare l'esempio di Brianza:
  // https://github.com/cylinderlight/Flavin/blob/master/CylinderLightFlavin/CylinderLightFlavin.ino
  // eventualmente togliendo la roba SPI
  // ok, provo.

  msg.fill((uint8_t*)data, len);

// quindi qui se devi "riempire" con qualcosa riempi con 'data'
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  // ... Hudio. Aspetta, qui c'e' un errore di
  // 'cut+paste'. Il codice era
  // msg.fill((uint8_t)data, len);
  // ;)
  // ... ma anche cosi' non funzionava.
  // Guarda, sono ""sicuro"" che il msg viene riempito,
  // perche' se nel messsaggio vado ad appendere
  // un intero, o un float, riesco poi a recuperarlo
  // da questo sketch, usando msg.getInt() -- o ...getFloat()
  // OK
  
  // avevi guardato questo? https://github.com/cylinderlight/Flavin/blob/master/CylinderLightFlavin/CylinderLightFlavin.ino
  
  // no. Provo a scorrere velocemente
  
  // avevo messo i link nel github, sono i sorgenti degli esempi di Marco Brianza
  
  // il cuore: Udp.read(packetBuffer,s);
  
  // ah! ...magari potessi usare EthernetUDP pero'! // non si puo'?
  // Alla fine avevo abbandonato ... Sto cercando
  // di ricordarmi perche'. 
  // OK
  // Di sicuro la libreria per l'ENC28J60 andava
  // modificata, altrimenti non riuscivo a compilare
  // nulla.
  // modificata come? (a parte un parametro che avevo segnalato, il pin sbagliato)
  // sei sicuro sia sbagliato?
  // o cambi il sorgente o cambi il cavetto ;)
  // http://jeelabs.net/pub/docs/ethercard/classENC28J60.html
  // initialize non usa di default il pin8 ...? appunto, ho messo il 10 nel sorgente perche' il cablaggio che avevo trovato usava il 10
  // Comunque non era quello il problema.
  // ok
  // Ti apro sempre in darkstar il sorgente
  // della libreria Ethercard
  // OK, cosa devo guardare?

  /* If OSC message contains "on", then switch the led on */
  /*
  if (msg.fullmatch("/led/on")) {
    digitalWrite(led, HIGH);
  }
  */
  
  /*
  bool fullMatch(const char * pattern, int addressOffset = 0);
Returns true only if the OSCMessage’s address is a full match of the pattern. Optionally takes a the address offset as an argument which is the starting position in the address to match from.

int match(const char * pattern, int addressOffset = 0);
Similar to fullMatch, but instead returns the number of address characters matched by the pattern. Returns 0 if there was no match or the match did not terminate at the end of the address or at a ‘/’.
  */
    
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

  /* Setup GPIO stuff ...*/
  pinMode(led, OUTPUT);

  /* ... and register udptoGPIO() to port 8888 */
  ether.udpServerListenOnPort(&udptoGPIO, 8888);  // registering "interrupt"
  // cioe'? e' una funzione di risposta interrupt, viene chiamata quando ci sono dati immagino
  // si
  // hai verificato che venga chiamata?
  // si
  // Non solo. I parametri vengono anche effettivamente
  // 'riempiti'. Il buffer 'data' *non* e' vuoto.
  // Vedi EtherCard.h, linea 41
  
  // OK, forse ho trovato errore, torna su e seguimi ;)
  // ok
}

void loop(){
  /* This MUST be called for ethercard functions to work */
  ether.packetLoop(ether.packetReceive());
} //ah 