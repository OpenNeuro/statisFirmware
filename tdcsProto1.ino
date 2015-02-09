#include <SPI.h>




void setup() {
  LED_init();
  Controller_init();
  // initialize SPI:
  SPI.begin(); 
  Serial.begin(9600);
}

//mainpump
void loop() {
  LED_main();
  Controller_main();
}


