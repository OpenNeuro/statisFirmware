#include <SPI.h>

#define VERSIONSET 2
#define VERSION_MINOR 1
#define VERSION_MAJOR 0
#define FIRMWARE_NAME "statis core"

#define EMERGENCY_STOP_PIN 6

void setup() {
  LED_init();
  Controller_init();
  Interface_init();
}

//mainpump
void loop() {
  Interface_main();
  LED_main();
  Controller_main();
}


void Fault(char* str)
{
  Serial.print("-1,");
  Serial.println(str);
  Controller_set_mode(0);
  Controller_main();//update the pins to the new mode
  LED_ready(0);
  while(1)
  {
    LED_fault(1);
    LED_active(0);
    LED_main();
    delay(25);
    LED_fault(0);
    LED_active(1);
    LED_main();
    delay(25);
  }
}
