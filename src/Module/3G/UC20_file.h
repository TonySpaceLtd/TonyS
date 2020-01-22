#ifndef UC20_FILE_H
#define UC20_FILE_H

#include <Arduino.h>
#include "UC20_lib.h"

#define FMODE_NORMAL      0
#define FMODE_OVERWRITE   1
#define FMODE_READONLY    2

class UC20_file {
	public:
		void list(String fname = "ALL");
		long filesize(String fname);
		bool del(String fname);
		bool mov(String srcPath, String destPath, bool copy = false, bool overwrite = false);
		int open(String fname, uint8_t mode = FMODE_NORMAL);
		int opened(String fname);
		bool close(int handle = -1);
		int read();
		String readString();
		int readbufAvailable();
		bool beginWrite(long len = -1);
		bool endWrite();
		long getPointerPos();
		long movPointer(long pos);
		long movPointerRight(long offset);
		long movPointerEnd();
		
	private:
		int _handle = -1;
		int _userid = -1;
};

#endif
