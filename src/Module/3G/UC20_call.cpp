#include "UC20_call.h"

bool UC20_call::call(String phone_no) {
	gsm.print(F("ATD"));
	gsm.print(phone_no);
	gsm.println(F(";"));
	if(gsm.waitStr(F("OK")) != "") {
		_state = BUSY;
		return true;
	}
	else 
		return false;
}

bool UC20_call::answer() {
	gsm.println(F("ATA"));
	if(gsm.waitStr(F("OK"),3000,F("NO CARRIER")) != "") {
		_state = BUSY;
		return true;
	}
	else 
		return false;
}

bool UC20_call::disconnect() {
	gsm.println(F("ATH"));
	if(gsm.waitStr(F("OK")) != "") {
		_state = IDLE;
		return true;
	}
	else 
		return false;
}

String UC20_call::currentCallME() {
	gsm.println(F("AT+CLCC"));
	String number = "";
	String resp = gsm.waitStr(F("+CLCC:"));
	if(resp != "") {
		int8_t index1 = resp.indexOf(F("\""))+1;
		int8_t index2 = resp.lastIndexOf(F("\""));
		number = resp.substring(index1,index2);
		gsm.waitStr(F("OK"));
	}
	return number;
}

int8_t UC20_call::listen() {
	if(gsm.available() > 0) {
		String resp = gsm.readStringUntil('\r');
		gsm.readStringUntil('\n');
		if(resp.startsWith(F("RING"))) {
			_state = RING;
		}
		else if(resp.startsWith(F("BUSY"))) {
			_state = IDLE;
		}
		else if(resp.startsWith(F("NO CARRIER"))) {
			_state = IDLE;
		}
		else if(resp.startsWith(F("NO ANSWER"))) {
			_state = IDLE;
		}
		return _state;
	}
	return -1;
}

int8_t UC20_call::checkCallStat() {
	gsm.println(F("AT+CPAS"));
	String resp = gsm.waitStr(F("+CPAS:"));
	if(resp != "") {
		int8_t index = resp.indexOf(F(" "))+1;
		_state = resp.substring(index).toInt();
		gsm.waitStr(F("OK"));
		return _state;
	}
	else 
		return -1;
}

int8_t UC20_call::getStat() {
	return _state;
}









