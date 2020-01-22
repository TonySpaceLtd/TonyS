#ifndef UC20_GNSS_H
#define UC20_GNSS_H

#include <Arduino.h>
#include "UC20_lib.h"

#define GGA   "GGA"
#define RMC   "RMC"
#define GSV   "GSV"
#define GSA   "GSA"
#define VTG   "VTG"
#define GNS   "GNS"

class UC20_GNSS {
	public:
		bool begin();
		bool end();
		bool setNMEA(bool enable);
		String getNMEA(const char* type);
		bool getPosition();
		String getInfo();
		String date();
		String time(int8_t timezone = 0);
		float latitude();
		float longtitude();
		float hdop();
		float altitude();
		int fix();
		String cog();
		float spkm();
		float spkn();
		int nsat();
		
	private:
		String gpsInfo = "";
};

#endif
