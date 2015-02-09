
#define OP_VERSIONSET 0
#define OP_VERSIONSTR 1
#define OP_FIRMWARE 2

#define OP_LEDREAD 10
#define OP_LEDREADY 11
#define OP_LEDFAULT 12
#define OP_LEDACTIVE 13

#define OP_GETCURRENT 20
#define OP_SETCURRENT 21
#define OP_CURRENTMODE 22      //0=off,1=minimum,2=based on current setting
#define OP_REPORTSPEED 23       //0=off,else is the gap in milliseconds between current reports. Current reports will never come quicker than once per cycle.
#define OP_EMERGENCY 25

int Report_speed = 0;
unsigned long Last_report = 0;//millis()

void Interface_init()
{
  Serial.begin(9600);
}

void Interface_main()
{
  if(Serial.available())
  {
    int opcode = Serial.parseInt();
    char comma1 = Serial.read();
    int value = Serial.parseInt();
    Serial.readStringUntil('\n');
    Interface_process_command(opcode, value);
  }
  
  if(Report_speed)//if continuous current reporting is enabled
  {
    if(millis() > (Last_report+Report_speed))
    {
      Last_report = millis();
      Serial.print(OP_GETCURRENT);
      Serial.print(",");
      Serial.println(Controller_current());
    }
  }
}


void Interface_process_command(int opcode, int value)
{
  switch(opcode)
  {
    case OP_VERSIONSET:
      Serial.print(OP_VERSIONSET);
      Serial.print(",");
      Serial.println(VERSIONSET);
      break;
    case OP_VERSIONSTR:
      Serial.print(OP_VERSIONSTR);
      Serial.print(",");
      Serial.print(VERSION_MAJOR);
      Serial.print(".");
      Serial.println(VERSION_MINOR);
      break;
    case OP_FIRMWARE:
      Serial.print(OP_FIRMWARE);
      Serial.print(",");
      Serial.println(FIRMWARE_NAME);
      break;
      
      
    case OP_LEDREAD:
      Serial.print(OP_LEDREAD);
      Serial.print(",");
      Serial.println(LED_read(value));
      break;
    case OP_LEDREADY:
      Serial.print(OP_LEDREADY);
      Serial.print(",");
      LED_ready(value);
      Serial.println(value);
      break;
    case OP_LEDFAULT:
      Serial.print(OP_LEDFAULT);
      Serial.print(",");
      LED_fault(value);
      Serial.println(value);
      break;
    case OP_LEDACTIVE:
      Serial.print(OP_LEDACTIVE);
      Serial.print(",");
      LED_active(value);
      Serial.println(value);
      break;
      
      
    case OP_GETCURRENT:
      Serial.print(OP_GETCURRENT);
      Serial.print(",");
      Serial.println(Controller_current());
      break;
      
   case OP_SETCURRENT:
      Serial.print(OP_SETCURRENT);
      Serial.print(",");
      Controller_set_current_target(value);
      Serial.println(value);
      break;
   
   case OP_CURRENTMODE:
      Serial.print(OP_SETCURRENT);
      Serial.print(",");
      Controller_set_mode(value);
      Serial.println(value);
      break;
   case OP_EMERGENCY:
      Fault("REMOTE");
      break;
   
   case OP_REPORTSPEED:
      Report_speed = value;
      Serial.print(OP_REPORTSPEED);
      Serial.print(",");
      Serial.println(value);
      break;
 }
}
