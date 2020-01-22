#ifndef UC20_FTP_H
#define UC20_FTP_H

#include <Arduino.h>
#include "UC20_lib.h"
#include "UC20_internet.h"
#include "UC20_SSL.h"

#define BINARY   0
#define ASCII    1

class UC20_FTP : public UC20_internet, public UC20_SSL {
	public:
		UC20_FTP(int8_t sslctxid = 1, int8_t ctxid = 1);
		void initConfig();
		void account(const char* user, const char* pass);
		void fileType(bool type);
		// void enableSSL();
		void disableSSL();
		int login(const char* server, const char* port);
		int login(const char* server, unsigned int port);
		bool logout();
		String getPath();
		bool setPath(String path);
		void list(String path = ".", bool detail = true);
		bool mkDir(String dirname);
		bool delDir(String dirname);
		bool del(String fn);
		bool rename(String old_name, String new_name);
		bool put(String fn, String localname, int startpos = -1);
		bool wait_putdone();
		bool get(String fn, String localname, int startpos = -1);
		
	private:
		
};

#endif
