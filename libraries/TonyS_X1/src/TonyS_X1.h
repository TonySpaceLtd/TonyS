#ifndef TonyS_X1_h
#define TonyS_X1_h

#include <Wire.h>
#include <SPI.h>

//include Onboard Library
#include "Onboard/TonySpace_MAX11301.h"
#include "Onboard/TonySpace_Pins.h"
#include "Onboard/TonySpace_IO.h"
#include "Onboard/RTClib/RTClib.h"

class TonyS_X1
{
public:
	TonyS_X1();
	void begin();
	void pinMode(uint8_t pin, uint8_t type);
	void digitalWrite(uint8_t pin, bool value);
	bool digitalRead(uint8_t pin);
	uint16_t analogRead(uint8_t pin);
	void analogWrite(uint8_t pin, uint16_t Output); // Output 0-10V
	void onPower(); // On power on board
	void offPower(); // Off power on board
	void onMAX11301(); // On MAX11301's power
	void offMAX11301(); // Off MAX11301's power
	void checkIC(); // Check working IC on board
	bool isrtc_rdy();
	bool ismax_rdy();
	 
	HardwareSerial SerialBegin(uint8_t slot,unsigned long baud,uint32_t config=SERIAL_8N1, int8_t rxPin=-1, int8_t txPin=-1, bool invert=false, unsigned long timeout_ms = 20000UL); //Serial
	
private:
		
	
};
extern TonyS_X1 Tony;




#endif





















