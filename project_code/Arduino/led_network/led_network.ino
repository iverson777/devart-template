#include "LPD8806.h"
#include "SPI.h"

#include "Ethernet.h"


int nLEDs = 32;

int dataPin  = 2;
int clockPin = 3;
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

byte mac[] = {  
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
EthernetClient client;
void setup() {
  strip.begin();
  strip.show();
  
  
  Serial.begin(9600);
  // this check is only needed on the Leonardo:
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
}

void loop() {
  colorChase(strip.Color(127,  0,  0), 100); // Red
  colorChase(strip.Color(  0,127,  0), 100); // Green
  colorChase(strip.Color(  0,  0,127), 100); // Blue
  colorChase(strip.Color(127,127,127), 100); // White
}

void colorChase(uint32_t c, uint8_t wait) {
  int i;
  
  for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
    delay(wait);
  }

  strip.show(); 
}

