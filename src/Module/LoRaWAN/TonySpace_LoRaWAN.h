#ifndef TonySpace_LoRaWAN_h
#define TonySpace_LoRaWAN_h

#include "Arduino.h"
#include <stdlib.h>
#include <inttypes.h>
#include "Stream.h"
#include "esp32-hal.h"

class TONY_LORA 
{
	public:
		TONY_LORA();
		void slot(uint8_t slot);
		bool reset(void);
		bool begin(String deveui="", String appeui="", String appkey="");
		bool setDeveui(String deveui, uint16_t timeout = 2000);
		bool setAppeui(String appeui, uint16_t timeout = 2000);
		bool setAppkey(String appkey, uint16_t timeout = 2000);
		bool getDeveui(char * deveui, uint16_t timeout = 1000);
		bool getAppeui(char * appeui, uint16_t timeout = 1000);
		bool getAppkey(char * appkey, uint16_t timeout = 1000);
		bool joinOTAA(uint16_t timeout = 1000);
		bool joinStatus(uint16_t timeout = 15000);
		bool stringWrite(uint8_t port, String stringdata, uint16_t timeout = 15000);
		bool setSyncword(String sync, uint16_t timeout = 1000);
		bool receiveBrodcast(char * receivedata, uint16_t timeout = 20000);
		bool sendBrodcast(String stringdata, uint16_t timeout = 2000);	
		bool getRespond(char * respond, uint16_t timeout);
		
	protected:

};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
extern TONY_LORA TonyLORA;
extern HardwareSerial LoRaSerial;
#endif
#endif















