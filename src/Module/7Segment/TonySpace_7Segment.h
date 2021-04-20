// V.TS001
// Cr.Rob Tillaart

#ifndef TonySpace_7SEGMENT_h
#define TonySpace_7SEGMENT_h
#include "Arduino.h"


#define HT16K33_ADDR	0x70

class TONY_7SEG
{
public:
  TONY_7SEG();           // 0x70 .. 0x77

  void begin();
  void reset();

  // default _cache is true as it is ~3x faster but if one has noise
  // on the I2C and wants to force refresh one can disable caching
  // for one or more calls.
  void clearCache();
  void cacheOn()  { _cache = true; };
  void cacheOff() { _cache = false; };

  void displayOn();
  void displayOff();

  void brightness(uint8_t val);             // 0 .. 15
  void blink(uint8_t val);                  // 0 .. 3     0 = off

  // 0,1,2,3,4 digits - will replace suppressLeadingZeroPlaces
  void setDigits(uint8_t val);
  // 0 = off, 1,2,3,4 digits  space iso 0
  void suppressLeadingZeroPlaces(uint8_t val);    // will be obsolete

  void displayClear();
  void displayInt(int n);                   // -999 .. 9999
  void displayHex(uint16_t n);              // 0000 .. FFFF

  // Date could be {month.day} or {day.hour}           . as separator
  // Time could be hh:mm or mm:ss or ss:uu (hundreds   : as separator
  //
  void displayDate(uint8_t left, uint8_t right);    // 00.00 .. 99.99
  void displayTime(uint8_t left, uint8_t right);    // 00:00 .. 99:99
  
  void displayFloat(float f);               // -999 .. 0.000 .. 9999

  void display(uint8_t *arr);               // array with 4 elements
  void display(uint8_t *arr, uint8_t pt);   // pt = digit with . (0..3)
  void displayColon(uint8_t on);            // 0 = off
  void displayRaw(uint8_t *arr, bool colon = false);  // max control

  void displayVULeft(uint8_t val);          // 0..8
  void displayVURight(uint8_t val);         // 0..8

  // DEBUG
  void displayTest(uint8_t del);
  void dumpSerial(uint8_t *arr, uint8_t pnt);

private:

  void writeCmd(uint8_t cmd);
  void writePos(uint8_t pos, uint8_t mask);
  void writePos(uint8_t pos, uint8_t mask, bool pnt);

  uint8_t _addr;
  uint8_t _displayCache[5];                 // for performance
  bool    _cache = true;
  uint8_t _digits = 0;
  uint8_t _bright;
};

// -- END OF FILE --


#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
extern TONY_7SEG Tony7Seg;
#endif
#endif





















