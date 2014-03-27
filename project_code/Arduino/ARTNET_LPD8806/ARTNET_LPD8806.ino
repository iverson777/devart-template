#include "LPD8806.h"
#include "SPI.h"

// Simple test for 160 (5 meters) of LPD8806-based RGB LED strip

/*****************************************************************************/

// Number of RGB LEDs in strand:
int nLEDs = 32;

// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 6;
int clockPin = 8;

LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);





#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#define short_get_high_byte(x) ((HIGH_BYTE & x) >> 8)
#define short_get_low_byte(x)  (LOW_BYTE & x)
#define bytes_to_short(h,l) ( ((h << 8) & 0xff00) | (l & 0x00FF) );

//MAC and IP of the ethernet shield
//MAC adress of the ethershield is stamped down the shield
//to translate it from hexa decimal to decimal, use: http://www.frankdevelopper.com/outils/convertisseur.php
//HARDWARE
byte mac[] = {144, 162, 218, 00, 16, 96}; //the mac adress of ethernet shield or uno shield board
byte ip[] = {192, 168, 2, 2}; // the IP adress of your device, that should be in same universe of the network you are using, here: 192.168.1.x

// the next two variables are set when a packet is received
byte remoteIp[4];        // holds received packet's originating IP
unsigned int remotePort; // holds received packet's originating port

//customisation: edit this if you want for example read and copy only 4 or 6 channels from channel 12 or 48 or whatever.
const int number_of_channels=512; //512 for 512 channels
const int channel_position=1; // 1 if you want to read from channel 1

//buffers
const int MAX_BUFFER_UDP=1024;//definition to do to make work UDP lib
char packetBuffer[530]; //buffer to store incoming data
byte buffer_dmx[number_of_channels+channel_position]; //buffer to store filetered DMX data

// art net parameters
unsigned int localPort = 6454;      // artnet UDP port is by default 6454
const int art_net_header_size=17;
const int max_packet_size=576;
char ArtNetHead[8]="Art-Net";
char OpHbyteReceive=0;
char OpLbyteReceive=0;
short is_artnet_version_1=0;
short is_artnet_version_2=0;
short seq_artnet=0;
short artnet_physical=0;
short incoming_universe=0;
boolean is_opcode_is_dmx=0;
boolean is_opcode_is_artpoll=0;
boolean match_artnet=1;
short Opcode=0;
EthernetUDP Udp;

void setup() {
  strip.begin();
  strip.show();
  //setup pins as PWM output
  //pinMode(3, OUTPUT);  //check with leds + resistance in pwm, this will not work with pins 10 and 11, used by RJ45 shield
  //pinMode(9, OUTPUT);  //check with leds + resistance in pwm, this will not work with pins 10 and 11, used by RJ45 shield
  //pinMode(5, OUTPUT);  //check with leds + resistance in pwm, this will not work with pins 10 and 11, used by RJ45 shield 
  pinMode(10, OUTPUT);
  //setup ethernet and udp socket
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  Serial.begin(9600);
}

void loop() {
  int packetSize = Udp.parsePacket();
  
  //FIXME: test/debug check
  //if(packetSize>art_net_header_size && packetSize<=max_packet_size)//check size to avoid unneeded checks
  if(packetSize) {
    /*Serial.print("Packet received! (size: ");
    Serial.print(packetSize);
    Serial.print(") \n");
    Serial.print(art_net_header_size);
    Serial.print("\n");
    Serial.print(max_packet_size);
    Serial.print("\n");
    */
    IPAddress remote = Udp.remoteIP();    
    remotePort = Udp.remotePort();
    Udp.read(packetBuffer,576);//UDP_TX_PACKET_MAX_SIZE
    
    //read header
    match_artnet=1;
    for (int i=0;i<7;i++) {
      //if not corresponding, this is not an artnet packet, so we stop reading
      if(char(packetBuffer[i])!=ArtNetHead[i]) {
        match_artnet=0;break;
      } 
    } 
       
     //if its an artnet header
    if(match_artnet==1) { 
        //artnet protocole revision, not really needed
        is_artnet_version_1=packetBuffer[10]; 
        is_artnet_version_2=packetBuffer[11];
      
        //sequence of data, to avoid lost packets on routeurs
        seq_artnet=packetBuffer[12];
          
        //physical port of  dmx 
        artnet_physical=packetBuffer[13];
        
      //operator code enables to know wich type of message Art-Net it is
      Opcode=bytes_to_short(packetBuffer[9],packetBuffer[8]);
      
     //if opcode is DMX type
      if(Opcode==0x5000) {
        is_opcode_is_dmx=1;is_opcode_is_artpoll=0;
      }   
       
      //if opcode is artpoll 
      else if(Opcode==0x2000) {
         is_opcode_is_artpoll=1;is_opcode_is_dmx=0;
         //( we should normally reply to it, giving ip adress of the device)
      } 
       
      //if its DMX data we will read it now
      if(is_opcode_is_dmx=1) {
         //if you need to filter DMX universes, uncomment next line to have the universe rceived
         incoming_universe= bytes_to_short(packetBuffer[15],packetBuffer[14])
          //getting data from a channel position, on a precise amount of channels, this to avoid to much operation if you need only 4 channels for example
          //channel position
          for(int i=channel_position-1;i< number_of_channels;i++) {
            buffer_dmx[i]= char(packetBuffer[i+17]);
            //Serial.println(packetBuffer[i+17]);
          }
      }
    }//end of sniffing
    
    /*
    for(int k=1; k<dmxsize; k+=4) {
       strip.setPixelColor(ledcount, strip.Color(buffer_dmx[k],  buffer_dmx[k+1],  buffer_dmx[k+2]));   
       ledcount++;
    }
    
    Serial.print(buffer_dmx[10]);
    Serial.print(buffer_dmx[11]);
    Serial.print(buffer_dmx[12]);
    */
    //Serial.println("");

    //stuff to do on PWM or whatever
    //analogWrite(3,buffer_dmx[6]);  
    //analogWrite(5,buffer_dmx[7]); 
    //analogWrite(6,buffer_dmx[8]); 
     // Set new pixel 'on'
     
    int dmxsize = sizeof(buffer_dmx);
    int ledcount=0;
    //Serial.println(dmxsize);
    for(int m=0;m<dmxsize;m+=4){
      /*Serial.print(buffer_dmx[m+1]);
      Serial.print(", ");
      Serial.print(buffer_dmx[m+2]);
      Serial.print(", ");
      Serial.print(buffer_dmx[m+3]);
      Serial.print(", ");
      */
      //Serial.println(buffer_dmx[m+1]);
      strip.setPixelColor(ledcount, strip.Color(buffer_dmx[m+1]/2,  buffer_dmx[m+2]/2,  buffer_dmx[m+3]/2));
      ledcount++;
    }
    //Serial.println(", ");
    //strip.setPixelColor(1, strip.Color(255,  255, 255));   
    //analogWrite(3,buffer_dmx[1]/2);
    //analogWrite(9,buffer_dmx[5]/2);
    //analogWrite(5,buffer_dmx[9]/2);
    analogWrite(10,buffer_dmx[1]/2);
    //analogWrite(A0,buffer_dmx[1]/2);
    strip.show();    
  }  
}




