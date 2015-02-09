// set pin 10 as the slave select for the digital pot:
const int ADCSelectPin = 10;

//Voltage diff / R = I
//0.1% tolerance resistor should give a variation of +- 0.02mA
//ADC should be able to measure voltage changes of 0.004V
//TL;DR: it should work accurately.

enum CURRENTmode {
  off,
  minimum,
  auto//looks at targetCurrent and adjusts pot incrementially
};

void Controller_init()
{
  // set the slaveSelectPin as an output:
  pinMode (ADCSelectPin, OUTPUT);
}


void Digital_pot_write(int address, int value) {
  // take the SS pin low to select the chip:
  digitalWrite(ADCSelectPin,LOW);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(ADCSelectPin,HIGH); 
}

