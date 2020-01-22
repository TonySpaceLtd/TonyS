
#ifndef TonySpace_JOYSTICK_h
#define TonySpace_JOYSTICK_h

#include "Arduino.h"
#include "Module/PCF8574/TEE_PCF8574.h"

#define JOYSTICK_ADDR 0x24

#define PIN_UP  	4
#define PIN_DOWN 	0
#define PIN_LEFT  	3
#define PIN_RIGHT  	2
#define PIN_ENTER  	1

#define PIN_LED1  	7
#define PIN_LED2 	5
#define PIN_BUZZ	6

#define PRESS_UP 	16
#define PRESS_DOWN 	1
#define PRESS_LEFT	8	
#define PRESS_RIGHT	4
#define PRESS_ENTER	2

#define LED_ON  0
#define LED_OFF 1

class JOYSTICK 
{
	public:
		JOYSTICK();
		bool begin(char addr=JOYSTICK_ADDR);
		uint8_t getCursor();
		void led1(uint8_t onoff){io.digitalWrite(PIN_LED1,onoff);};
		void led2(uint8_t onoff){io.digitalWrite(PIN_LED2,onoff);};
		void buzz(uint8_t onoff){io.digitalWrite(PIN_BUZZ,onoff);};
	private:
		PCF8574_IO io;
};
#endif





















