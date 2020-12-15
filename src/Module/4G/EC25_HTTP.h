#ifndef ECXX_HTTP_H
#define ECXX_HTTP_H

#include "Arduino.h"
#include <Stream.h>
#include "TonyS_X1.h"
#include "EC25_lib.h"
#include "EC25_FILE.h"

#define HTTP_STORAGE 	RAM
#define HTTP_PATH 		"http.temp"

class EC25_HTTP
{
	public:
	EC25_HTTP();
	bool setURL(String url,bool respHeader=false);
	int get();
	size_t read(uint8_t *data,size_t buffSize);
	void (*callback)(uint8_t *data,size_t size,bool finish);
	private:
		EC25_FILE httpFile;
		size_t readTail;
		int httphandle;
	
	
};































#endif