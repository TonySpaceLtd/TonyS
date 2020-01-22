
#ifndef TonySpace_PCF8574_h
#define TonySpace_PCF8574_h

#include "Arduino.h"

#define PCF_INPUT 1
#define PCF_OUTPUT 0

class PCF8574
{
	public:
		PCF8574();
		bool begin(char addr);
		uint8_t read_();
		void  write(uint8_t data);
	private:
	 char addr_;
};

class PCF8574_IO : public PCF8574
{
	public:
		PCF8574_IO();
		void digitalWrite(uint8_t pin,uint8_t mode);
		uint8_t digitalRead(uint8_t pin);
		void pinMode(uint8_t pin,uint8_t mode);
		uint8_t read();
	private:
		uint8_t output_buf;
		uint8_t pin_mode;
	
};

#endif


















