#ifndef EC25_LIB_H
#define EC25_LIB_H


#include "Arduino.h"
#include "TonyS_X1.h"
#include <Stream.h>
#include "EC25_SER.h"

class EC25 : public EC25_SER
{
	
	public:
		EC25();
		EC25_SER ECser;
		
		void initial(uint8_t slot);
		void powerOn(bool onoff);
		void reset();
		
		bool waitModuleReady(uint8_t timeout = 1); // Timeout = sec
		void setDefaultConfig();
		String getIMEI();
		String getCIMI();
		bool   getNetworkRegis();
		String getOperator_();
		
		String getOperator();
		String getAccessTech();
		int getSignal_();
		int getSignal();
		int getSignaldBm();
		
		bool connectInternet();
		bool disconnectInternet();
		String getIPAddress();
		
		
	private:
		uint8_t powerPin;
		
};
extern EC25 LTE;
























#endif