#include "TonySpace_TactSwitch.h"
#include "TonyS_X1.h"
#include <stdlib.h>
#include <stdio.h>



#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
TONY_TACT_SWITCH TonyTactSW;
#endif


TONY_TACT_SWITCH::TONY_TACT_SWITCH()
{
}

void TONY_TACT_SWITCH::begin(uint8_t slot)
{
	switch(slot)
	{
		case  SLOT1 :
			Tony.pinMode(IO0, OUTPUT);
			Tony.digitalWrite(IO0, HIGH);
			break;
		case  SLOT2 :
			Tony.pinMode(IO2, OUTPUT);
			Tony.digitalWrite(IO2, HIGH);
			break;
		case  SLOT3 :
			Tony.pinMode(IO4, OUTPUT);
			Tony.digitalWrite(IO4, HIGH);
			break;
		case  SLOT4 :
			Tony.pinMode(IO6, OUTPUT);
			Tony.digitalWrite(IO6, HIGH);
			break;
		case  SLOT5 :
			Tony.pinMode(IO8, OUTPUT);
			Tony.digitalWrite(IO8, HIGH);
			break;
		case  SLOT6 :
			Tony.pinMode(IO10, OUTPUT);
			Tony.digitalWrite(IO1, HIGH);
			break;
	}	
	TONY_TACT_SWITCH::write8(0xFF);
	TonyTactSW.write(5, 0); // OFF Buzzer
	TonyTactSW.write(6, 1); // OFF LED1
	TonyTactSW.write(7, 1); // OFF LED2
}

// removed Wire.beginTransmission(addr);
// with  @100KHz -> 265 micros()
// without @100KHz -> 132 micros()
// without @400KHz -> 52 micros()
// TODO @800KHz -> ??
uint8_t TONY_TACT_SWITCH::readAll()
{
  if (Wire.requestFrom(ADDR_TACT_SW, (uint8_t)1) != 1)
  {
    _error = CHIP_I2C_ERROR;
    return _dataIn; // last value
  }
  _dataIn = Wire.read();
  return (_dataIn&0b00011111);
}

void TONY_TACT_SWITCH::write8(const uint8_t value)
{
  _dataOut = value;
  Wire.beginTransmission(ADDR_TACT_SW);
  Wire.write(_dataOut);
  _error = Wire.endTransmission();
}

uint8_t TONY_TACT_SWITCH::read(const uint8_t pin)
{
  if (pin > 7)
  {
    _error = CHIP_PIN_ERROR;
    return 0;
  }
  TONY_TACT_SWITCH::readAll();
  return (_dataIn & (1 << pin)) > 0;
}

void TONY_TACT_SWITCH::write(const uint8_t pin, const uint8_t value)
{
  if (pin > 7)
  {
    _error = CHIP_PIN_ERROR;
    return;
  }
  if (value == LOW)
  {
    _dataOut &= ~(1 << pin);
  }
  else
  {
    _dataOut |= (1 << pin);
  }
  write8(_dataOut);
}


//added 0.1.07/08 Septillion
uint8_t TONY_TACT_SWITCH::readButton8(const uint8_t mask)
{
  uint8_t temp = _dataOut;
  TONY_TACT_SWITCH::write8(mask | _dataOut); 
  TONY_TACT_SWITCH::readAll();
  TONY_TACT_SWITCH::write8(temp);

  return _dataIn;
}

//added 0.1.07 Septillion
uint8_t TONY_TACT_SWITCH::readButton(const uint8_t pin)
{
  if (pin > 7)
  {
    _error = CHIP_PIN_ERROR;
    return 0;
  }

  uint8_t temp = _dataOut;
  TONY_TACT_SWITCH::write(pin, HIGH);
  uint8_t rtn = TONY_TACT_SWITCH::read(pin);
  TONY_TACT_SWITCH::write8(temp);

  return rtn;
}

void TONY_TACT_SWITCH::onBuzzer()
{
  TonyTactSW.write(5, 1); 
}

void TONY_TACT_SWITCH::offBuzzer()
{
  TonyTactSW.write(5, 0); 
}

void TONY_TACT_SWITCH::onLED1()
{
  TonyTactSW.write(6, 0); 
}

void TONY_TACT_SWITCH::offLED1()
{
  TonyTactSW.write(6, 1); 
}

void TONY_TACT_SWITCH::onLED2()
{
  TonyTactSW.write(7, 0); 
}

void TONY_TACT_SWITCH::offLED2()
{
  TonyTactSW.write(7, 1); 
}
// -- END OF FILE --
