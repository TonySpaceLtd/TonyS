#ifndef UC20_SMS_H
#define UC20_SMS_H

#include <Arduino.h>
#include "UC20_lib.h"

class UC20_SMS {
	public:
		void initConfig();
		bool beginsend(String phone_no);
		bool endsend();
		String readHeadIndex(int index);
		String readIndex(int index);
		void list(String query);
		bool del(int index);
		int listen();
		int newRecvIndex();
		
	private:
		int new_sms_index = -1;
};

#endif
