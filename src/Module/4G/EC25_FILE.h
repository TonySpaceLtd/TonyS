#ifndef ECXX_FILE_H
#define ECXX_FILE_H

#include "Arduino.h"
#include <Stream.h>
#include "TonyS_X1.h"
#include "EC25_lib.h"

#define UFS		0
#define RAM 	1
#define SD_CARD 2

#define OPEN_RW		0
#define OPEN_ORW	1
#define OPEN_R 		2

#define BEGIN		0
#define CURRENT		1
#define END 		2

class EC25_FILE
{
	public:
		EC25_FILE();
		size_t getTotalSpace(uint8_t storage);
		size_t getFreeSpace(uint8_t storage);
		size_t spaceInfo(uint8_t storage,bool mode); // false = Totalspace,true = free spaceInfo
		String getfileInfo(uint8_t storage,String path);
		bool del(uint8_t storage,String path);
		int open(uint8_t storage,String path,uint8_t mode=OPEN_RW);
		size_t read(int handle,uint8_t *buf,size_t length);
		bool close(int handle);
		bool seek(int handle,size_t offset,uint8_t position=BEGIN);
		size_t fileSize(uint8_t storage,String path);
		
	private:
	
	
};































#endif