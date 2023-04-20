#ifndef ECXX_OTA_H
#define ECXX_OTA_H

#include "Arduino.h"
#include <Stream.h>
#include "TonyS_X1.h"
#include "EC25_lib.h"
#include "EC25_HTTP.h"
#include <Update.h>

#define OTA_BUFFER_SIZE 1024
class EC25_OTA
{
	public:
		EC25_OTA();
		bool updateFirmware(String url);
	private:
		bool updateFlash(size_t updateSize);
		EC25_HTTP httpOAT;
};

#endif