#include "UC20_lib.h"

UC20 gsm;

bool UC20::powerOn(int pin) {
	int io;
	if(pin!=0)
	{
		io = gsm.powerPin;
	}
	else
		io = pin;
	
	println(F("AT"));
	if(waitStr(F("OK"),1000) != "") {
		return true;
	}
	Tony.pinMode(io, OUTPUT);
	Tony.digitalWrite(io, HIGH);
	delay(1000);
	Tony.digitalWrite(io, LOW);
	delay(1000);
	if(waitStr(F("RDY"),8000) != "") {
		waitStr(F("PB DONE"),15000);
		initConfig();
		return true;
	}
	else 
		return false;
}

bool UC20::powerOff(int pin) {
	
	int io;
	if(pin!=0)
	{
		io = gsm.powerPin;
	}
	else
		io = pin;
	
	
	Tony.pinMode(io, OUTPUT);
	for(uint8_t i=0; i<2; i++) {
		Tony.digitalWrite(io, HIGH);
		delay(1000);
		Tony.digitalWrite(io, LOW);
		delay(1000);
		if(waitStr(F("POWERED DOWN"),5000) != "") 
			return true;
	}
	return false;
}

void UC20::initConfig() {
	println(F("ATV1"));
	waitStr(F("OK"),1000);
	println(F("ATE0"));
	waitStr(F("OK"),1000);
}

bool UC20::simReady() {
	println(F("AT+CPIN?"));
	String resp = waitStr(F("+CPIN:"));
	if(resp != "") {
		waitStr(F("OK"));
		return (resp.indexOf(F("READY")) != -1);
	}
	else 
		return false;
}

bool UC20::getNetworkStatus() {
	println(F("AT+CREG?"));
	String resp = waitStr(F("+CREG:"));
	if(resp != "") {
		int8_t index = resp.indexOf(F(","))+1;
		int8_t state = resp.substring(index).toInt();
		waitStr(F("OK"));
		if(state == 1) 
			return true;
		else 
			return false;
	}
	else 
		return false;
}

int8_t UC20::getSignal() {
	println(F("AT+CSQ"));
	String resp = waitStr(F("+CSQ:"));
	if(resp != "") {
		int8_t index1 = resp.indexOf(F(" "))+1;
		int8_t index2 = resp.indexOf(F(","));
		int8_t rssi = resp.substring(index1,index2).toInt();
		waitStr(F("OK"));
		return rssi;
	}
	else 
		return -1;
}

String UC20::getIMEI() {
	println(F("ATE1"));
	waitStr(F("OK"),1000);
	println(F("AT+GSN"));
	String imei = "";
	if(waitStr(F("AT+GSN"),3000,"",false) != "") {
		imei = waitStr(F("\n"));	// "\n<IMEI>"
		imei.remove(0,1);			// remove '\n'
		waitStr(F("OK"));
	}
	println(F("ATE0"));
	waitStr(F("OK"),1000);
	return imei;
}

String UC20::getIMSI() {
	println(F("ATE1"));
	waitStr(F("OK"),1000);
	println(F("AT+CIMI"));
	String imsi = "";
	if(waitStr(F("AT+CIMI"),3000,"",false) != "") {
		imsi = waitStr(F("\n"));	// "\n<IMSI>"
		imsi.remove(0,1);			// remove '\n'
		waitStr(F("OK"));
	}
	println(F("ATE0"));
	waitStr(F("OK"),1000);
	return imsi;
}

String UC20::getICCID() {
	println(F("AT+QCCID"));
	String resp = waitStr(F("+QCCID:"));
	String iccid = "";
	if(resp != "") {
		int8_t index = resp.indexOf(F(" "))+1;
		iccid = resp.substring(index);
	}
	waitStr(F("OK"),1000);
	return iccid;
}

String UC20::getOperator() {
	println(F("AT+COPS?"));
	String resp = waitStr(F("+COPS:"));
	String oper = "";
	if(resp != "") {
		int8_t index1 = resp.indexOf(F("\""))+1;
		int8_t index2 = resp.lastIndexOf(F("\""));
		if(index2 != -1) 
			oper = resp.substring(index1,index2);
		waitStr(F("OK"));
	}
	return oper;
}

String UC20::getFirmwareVersion() {
	println(F("ATI"));
	String resp = waitStr(F("Revision:"));
	String ver = "";
	if(resp != "") {
		int8_t index = resp.indexOf(F(" "))+1;
		ver = resp.substring(index);
		waitStr(F("OK"));
	}
	return ver;
}

String UC20::moduleTime() {
	println(F("AT+CCLK?"));
	String resp = waitStr(F("+CCLK:"));
	String t = "";
	if(resp != "") {
		int8_t index1 = resp.indexOf(F("\""))+1;
		int8_t index2 = resp.lastIndexOf(F("\""));
		t = resp.substring(index1,index2);
		waitStr(F("OK"));
	}
	return t;
}









