#include "UC20_SMS.h"

void UC20_SMS::initConfig() {
	gsm.println(F("AT+CMGF=1"));
	gsm.waitStr(F("OK"),1000);
	gsm.println(F("AT+CSCS=\"UCS2\""));
	gsm.waitStr(F("OK"),1000);
}

bool UC20_SMS::beginsend(String phone_no) {
	initConfig();
	gsm.print(F("AT+CMGS=\""));
	gsm.printUnicode(phone_no);
	gsm.println(F("\""));
	return (gsm.waitStr(F(">")) != "");
}

bool UC20_SMS::endsend() {
	gsm.write(0x1A);
	String resp = gsm.waitStr(F("+CMGS:"));
	if(resp != "") {
		gsm.waitStr(F("OK"));
		return true;
	}
	else 
		return false;
}

String UC20_SMS::readHeadIndex(int index) {
	initConfig();
	gsm.print(F("AT+CMGR="));
	gsm.println(index);
	String head = "from ";
	String resp = gsm.waitStr(F("+CMGR:"));
	if(resp != "") {
		int index1 = resp.indexOf(F(",\""))+2;
		int index2 = resp.indexOf(F("\""),index1);
		if(index1 == 1 || index2 == -1) 
			return "";
		while(index2-index1 >= 4) {
			head += gsm.ucodeToUtf8(resp.substring(index1,index1+4));
			index1 += 4;
		}
		index1 = resp.indexOf(F(",\""),index2)+2;
		index2 = resp.indexOf(F(","),index1);
		head += ", date ";
		head += resp.substring(index1,index2);
		index1 = index2+1;
		index2 = resp.indexOf(F("\""),index1);
		head += ", time ";
		head += resp.substring(index1,index2);
		gsm.waitStr(F("OK"));
		return head;
	}
	else 
		return "";
}

String UC20_SMS::readIndex(int index) {
	initConfig();
	gsm.print(F("AT+CMGR="));
	gsm.println(index);
	String content = "";
	if(gsm.waitStr(F("+CMGR:")) != "") {
		while(1) {
			String temp = "";
			for(int8_t i=0; i<4; i++) {
				unsigned long prev_t = millis();
				while(!(gsm.available() > 0)) {
					if(millis()-prev_t > 1000) {
						return content;
					}
				}
				char c = gsm.read();
				if(c == '\r') 
					break;
				temp += c;
			}
			if(temp != "") 
				content += gsm.ucodeToUtf8(temp);
			else {
				gsm.waitStr(F("OK"));
				return content;
			}
		}
	}
	else 
		return "";
}

void UC20_SMS::list(String query) {
	initConfig();
	gsm.print(F("AT+CMGL=\""));
	gsm.print(query);
	gsm.println(F("\""));
	String resp = "";
	while(1) {
		resp = gsm.waitStr(F("+CMGL:"));
		if(resp != "") {
			int index1 = resp.indexOf(F(" "))+1;
			int index2 = resp.indexOf(F(","));
			gsm.debug->print(F("index "));							//##### Serial
			gsm.debug->print(resp.substring(index1,index2));		//##### Serial
			index1 = resp.indexOf(F("\",\""))+3;
			index2 = resp.indexOf(F("\""),index1);
			gsm.debug->print(F(": from "));							//##### Serial
			while(index2-index1 >= 4) {
				gsm.debug->print(gsm.ucodeToUtf8(resp.substring(index1,index1+4))); 	//##### Serial
				index1 += 4;
			}
			index1 = resp.indexOf(F(",\""),index2)+2;
			index2 = resp.indexOf(F(","),index1);
			gsm.debug->print(F(", date "));							//##### Serial
			gsm.debug->print(resp.substring(index1,index2));		//##### Serial
			index1 = index2+1;
			index2 = resp.indexOf(F("\""),index1);
			gsm.debug->print(F(", time "));							//##### Serial
			gsm.debug->println(resp.substring(index1,index2));		//##### Serial
		}
		else 
			break;
		
		while(1) {
			String temp = "";
			for(int8_t i=0; i<4; i++) {
				unsigned long prev_t = millis();
				while(!(gsm.available() > 0)) {
					if(millis()-prev_t > 1000) {
						return;
					}
				}
				char c = gsm.read();
				if(c == '\r') 
					break;
				temp += c;
			}
			if(temp != "") 
				gsm.debug->print(gsm.ucodeToUtf8(temp));			//##### Serial
			else {
				gsm.debug->println();								//##### Serial
				break;
			}
		}
	}
}

bool UC20_SMS::del(int index) {
	gsm.print(F("AT+CMGD="));
	gsm.println(index);
	return (gsm.waitStr(F("OK")) != "");
}

int UC20_SMS::listen() {
	if(gsm.available() > 0) {
		String resp = gsm.waitStr(F("+CMTI:"),1000);
		if(resp != "") {
			int8_t index = resp.indexOf(F(","))+1;
			new_sms_index = resp.substring(index).toInt();
			return new_sms_index;
		}
	}
	return -1;
}

int UC20_SMS::newRecvIndex() {
	return new_sms_index;
}









