
#define READY_LED_PIN 5
#define FAULT_LED_PIN 4
#define ACTIVE_LED_PIN 3

//setup blink speeds
#define FAST_BLINK_MILLIS 600
#define SLOW_BLINK_MILLIS 1400

enum LEDstate {
  off,
  on,
  slowblink,
  fastblink
};

LEDstate activeLED = fastblink;
LEDstate faultLED = fastblink;
LEDstate readyLED = fastblink;
unsigned long lastFastBlink = 0;//millis()
unsigned long lastSlowBlink = 0;//millis()

//called from pump
void LED_main()
{
  boolean isFastBlinkOn = false;
  boolean isSlowBlinkOn = false;
  if (millis() >= (lastFastBlink+FAST_BLINK_MILLIS))
  {
    isFastBlinkOn = true;
    lastFastBlink = millis();
  }
  if (millis() >= (lastSlowBlink+SLOW_BLINK_MILLIS))
  {
    isSlowBlinkOn = true;
    lastSlowBlink = millis();
  }
  
  LED_active_main(isSlowBlinkOn,isFastBlinkOn);
  LED_fault_main(isSlowBlinkOn,isFastBlinkOn);
  LED_ready_main(isSlowBlinkOn,isFastBlinkOn);
}


void LED_ready_main(boolean isSlowBlinkOn, boolean isFastBlinkOn)
{
  switch(readyLED)
  {
    case on:
      digitalWrite(READY_LED_PIN, HIGH);
      break;
    case off:
      digitalWrite(READY_LED_PIN, LOW);
      break;
    case slowblink:
      digitalWrite(READY_LED_PIN, isSlowBlinkOn);
      break;
    case fastblink:
      digitalWrite(READY_LED_PIN, isFastBlinkOn);
      break; 
  }
}

void LED_fault_main(boolean isSlowBlinkOn, boolean isFastBlinkOn)
{
  switch(faultLED)
  {
    case on:
      digitalWrite(FAULT_LED_PIN, HIGH);
      break;
    case off:
      digitalWrite(FAULT_LED_PIN, LOW);
      break;
    case slowblink:
      digitalWrite(FAULT_LED_PIN, isSlowBlinkOn);
      break;
    case fastblink:
      digitalWrite(FAULT_LED_PIN, isFastBlinkOn);
      break; 
  }
}

void LED_active_main(boolean isSlowBlinkOn, boolean isFastBlinkOn)
{
  switch(activeLED)
  {
    case on:
      digitalWrite(ACTIVE_LED_PIN, HIGH);
      break;
    case off:
      digitalWrite(ACTIVE_LED_PIN, LOW);
      break;
    case slowblink:
      digitalWrite(ACTIVE_LED_PIN, isSlowBlinkOn);
      break;
    case fastblink:
      digitalWrite(ACTIVE_LED_PIN, isFastBlinkOn);
      break; 
  }
}


void LED_init()
{
  pinMode(READY_LED_PIN, OUTPUT);
  pinMode(FAULT_LED_PIN, OUTPUT);
  pinMode(ACTIVE_LED_PIN, OUTPUT);
  LED_main();//update LEDS
}

int LED_read(int led)
{
  switch(led)
  {
    case 0:
      return (int)activeLED;
    case 1:
      return (int)readyLED;
    case 2:
      return (int)faultLED;
  }
}

void LED_ready(int value){
  readyLED = (LEDstate)value;
}
void LED_fault(int value){
  faultLED = (LEDstate)value;
}
void LED_active(int value){
  activeLED = (LEDstate)value;
}
