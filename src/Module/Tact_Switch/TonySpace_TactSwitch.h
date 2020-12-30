
#ifndef TonySpace_TactSwitch_h
#define TonySpace_TactSwitch_h

#include "Arduino.h"
#include "Wire.h"


#define ADDR_TACT_SW     0x20
#define CHIP_OK          0x00
#define CHIP_PIN_ERROR   0x81
#define CHIP_I2C_ERROR   0x82


class TONY_TACT_SWITCH
{
public:
        TONY_TACT_SWITCH(); //explicit TONY_TACT_SWITCH();
	
  void    begin(uint8_t slot);

  uint8_t readAll();
  uint8_t read(uint8_t pin);
  uint8_t value() const { return _dataIn; };

  void    write8(const uint8_t value);
  void    write(const uint8_t pin, const uint8_t value);
  uint8_t valueOut() const { return _dataOut; }
  
  void onBuzzer();
  void offBuzzer();
  void onLED1();
  void offLED1();
  void onLED2();
  void offLED2();

  //added 0.1.07/08 Septillion
  inline uint8_t readButton8()  { return TONY_TACT_SWITCH::readButton8(_buttonMask); }
  uint8_t        readButton8(const uint8_t mask = 0xFF);
  uint8_t        readButton(const uint8_t pin);
  inline void    setButtonMask(uint8_t mask) { _buttonMask = mask; };


  int lastError();

private:
  uint8_t _dataIn;
  uint8_t _dataOut;
  uint8_t _buttonMask;
  int     _error;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
extern TONY_TACT_SWITCH TonyTactSW;
#endif

#endif
















