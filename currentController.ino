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
int currentSetting = 0;




void Controller_init()
{
  // set the slaveSelectPin as an output:
  pinMode (ADCSelectPin, OUTPUT);
  pinMode (Cathode_Pin, OUTPUT);
  pinMode (Anode_Pin, INPUT);
}


//called from main pump
void Controller_main()
{
  Measure_current();
  
  switch(current)
  {
    case none:
      if (currentSetting != 0)
        Digital_pot_write(ADCAddr,0);
      break;
      
    case minimum:
      if (currentSetting != 0)
        Digital_pot_write(ADCAddr,0);
      Output_enable();
      break;
  }
}




//Voltage diff / R = I
//0.1% tolerance resistor should give a variation of +- 0.02mA
//ADC should be able to measure voltage changes of 0.004V
//TL;DR: it should work accurately.
void Measure_current()
{
  long int diff = abs(analogRead(CurrentSense_1)  -  analogRead(CurrentSense_2)) * AREF_Voltage;    //change in potential as V/1024
  float current = float(diff) / CS_Resistance / float(1024) * 1000;
  lastMeasuredCurrent = current * 1000//mA -> uA therefore * 1000
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
}

void Output_enable()
{
  digitalWrite(Cathode_Pin, HIGH);
}
