const int ADCSelectPin   = 10;  //set pin 10 as the slave select for the digital pot:
const int Cathode_Pin    = 2;
const int Anode_Pin      = 8;
const int CurrentSense_1 = 4;    //pin
const int CurrentSense_2 = 5;    //pin
const int AREF_Voltage   = 5;    //V
const int CS_Resistance  = 22.1;  //R
int ADCAddr = 0;//0 for Ciphersink tDCS, 5 for openStim


enum CURRENTmode {
  none,
  minimum,           //~10022 ohms @5V = ~0.4ma.
  feedback          //looks at targetCurrent and adjusts pot incrementially
};

CURRENTmode current = none;
long targetCurrent = 0;//uA.
long lastMeasuredCurrent = 0;//uA.
int currentSetting = 0;//as in the setting presently, not the target current.




void Controller_init()
{
  // initialize SPI:
  SPI.begin(); 
  // set the slaveSelectPin as an output:
  pinMode (ADCSelectPin, OUTPUT);
  //setup current supply/sink pins
  pinMode (Cathode_Pin, OUTPUT);
  pinMode (Anode_Pin, OUTPUT);
}


//called from main pump
void Controller_main()
{
  Measure_current();
  
  switch(current)
  {
    case none:
      if (currentSetting != 255)
        Digital_pot_write(ADCAddr,255);
      Output_disable();
      break;
      
    case minimum:
      if (currentSetting != 255)
        Digital_pot_write(ADCAddr,255);
      Output_enable();
      break;
    case feedback:
      if (lastMeasuredCurrent < targetCurrent)
        currentSetting = max(currentSetting-1, 0);
      else if (lastMeasuredCurrent > targetCurrent)
        currentSetting = min(currentSetting+1, 255);
      Output_enable();
      Digital_pot_write(ADCAddr,currentSetting);
      //Serial.println(lastMeasuredCurrent);
      //Serial.println(currentSetting);
  }
}




//Voltage-drop / R = I
//0.1% tolerance resistor should give a variation of +- 0.02mA
//ADC should be able to measure voltage changes of 0.004V
//TL;DR: it should work accurately.
void Measure_current()
{
  long int drop = abs(analogRead(CurrentSense_1)  -  analogRead(CurrentSense_2)) * AREF_Voltage;//change in potential as V*1024
  double current = float(drop) / CS_Resistance / float(1024) * 1000;
  lastMeasuredCurrent = current * 1000;//mA -> uA therefore * 1000
}


void Digital_pot_write(int address, int value) {
  currentSetting = value;
  // take the SS pin low to select the chip:
  digitalWrite(ADCSelectPin,LOW);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(ADCSelectPin,HIGH); 
  delay(5);
}


void Output_enable()
{
  digitalWrite(Cathode_Pin, HIGH);
}

void Output_disable()
{
  digitalWrite(Cathode_Pin, LOW);
}


///////////////////////////////////////
// SETTERS / GETTERS - PUBLIC METHODS /
///////////////////////////////////////
long Controller_current()
{
  return lastMeasuredCurrent;
}

void Controller_set_current_target(long target)
{
  targetCurrent = target;
}

void Controller_set_mode(int mode)
{
  current = (CURRENTmode)mode;
}
