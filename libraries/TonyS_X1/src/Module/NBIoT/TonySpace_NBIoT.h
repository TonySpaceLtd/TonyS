
#ifndef TonySpace_NBIoT_h
#define TonySpace_NBIoT_h

#include "Arduino.h"
#include "TonyS_X1.h"
#include "TonySpace_NBIoT_SER.h"
#include <Stream.h>

class NBIoT : public NBIoT_SER
{
	public:
		NBIoT();
		void begin(uint8_t slot,unsigned long baud = 9600);		
		void powerOn(bool on_off);
		bool powerOnReady(long tout=10000);
		bool setAutoConnect();
		void setConfig();
		String getIMEI();
		String getIMSI();
		String getSIMSerial();
		bool getNetworkStatus();
		uint8_t getSignal();
		int getSignaldBm();
		uint8_t getSignalLevel();
		String getDeviceIP();
		
	private:
		uint8_t nbPowerPin;
};
extern NBIoT_SER nbSer;
#endif





















