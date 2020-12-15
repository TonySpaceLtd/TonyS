#ifndef ECXX_GNSS_H
#define ECXX_GNSS_H

#include "Arduino.h"
#include <Stream.h>
#include "TonyS_X1.h"
#include "EC25_lib.h"

typedef struct GNSS_RES
{
	bool status;
	String latitude;
	String longitude;
	float latitude_Float;
	float longitude_Float;
	
}GNSS_Resp;

class EC25_GNSS
{
	public:
		EC25_GNSS();
		bool open();
		bool close();
		String get(uint8_t mode=2);
		GNSS_Resp getData(uint8_t mode=2);
	private:
};








































#endif