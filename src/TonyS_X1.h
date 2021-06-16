// V.TS003
#ifndef TonyS_X1_h
#define TonyS_X1_h

#include <Wire.h>
#include <SPI.h>

//include Onboard Library
#include "Onboard/TonySpace_MAX11301.h"
#include "Onboard/TonySpace_Pins.h"
#include "Onboard/TonySpace_IO.h"
#include "Onboard/RTClib/RTClib.h"

//------- ADDR -------//
#define ADDR_MAX11301	   0x38
#define ADDR_STM32         0x36
#define ADDR_MCP7940 	   0x6F
#define ADDR_DS3231		   0x68

//------- MAX11301 ------//
#define modeGPI 	0x00
#define modeGPO 	0x01
#define modeADC 	0x02
#define modeDAC 	0x03
#define modeDACADC  0x04

//------ STM32 -------//
#define SET_INPUT          0x01    
#define SET_OUTPUT         0x02   
#define OUTPUT_LOW         0x03   
#define OUTPUT_HIGH        0x04    
#define READ_DIGITAL_INPUT 0x05   
#define READ_ADC           0x06

#define UNKNOWN_MODEL      0x00
#define PRO_MODEL          0x01
#define BASIC_MODEL        0x02

class TonyS_X1
{
public:
	TonyS_X1();
	bool begin();
	void pinMode(uint8_t pin, uint16_t type);
	void digitalWrite(uint8_t pin, bool value);
	void digitalnormalWrite(uint8_t pin, bool value);
	bool digitalRead(uint8_t pin);
	uint16_t analogRead(uint8_t pin);
	void analogWrite(uint8_t pin, uint16_t Output); // Output 0-10V
	void preparePower();
	void onPower(); // On power on board
	void offPower(); // Off power on board
	void onExtension(); // On MAX11301's power and STM32's Power
	void offExtension(); // Off MAX11301's power and STM32's Power
	void checkIC(); // Check working IC on board
	bool ismax_rdy();
	bool isstm_rdy();
	void sendCommand(uint8_t pin, uint8_t command);
	void receiveData();
	uint16_t buildcheckSum(uint8_t pin,uint8_t value);
	bool reCheck();
	void clearBuffer();
	uint16_t readADC_STM32(uint8_t pin);
	bool digitalRead_STM32(uint8_t pin);
	void pinMode_STM32(uint8_t pin, uint8_t pinmode);
	void digitalWrite_STM32(uint8_t pin, uint8_t pinmode);
	void handleData(uint8_t pin);
	 
	HardwareSerial SerialBegin(uint8_t slot,unsigned long baud,uint32_t config=SERIAL_8N1, int8_t rxPin=-1, int8_t txPin=-1, bool invert=false, unsigned long timeout_ms = 20000UL); //Serial
	
private:
	
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
extern TonyS_X1 Tony;
#endif
#endif





















