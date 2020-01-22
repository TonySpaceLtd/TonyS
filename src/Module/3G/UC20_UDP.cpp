#include "UC20_UDP.h"

UC20_UDP::UC20_UDP(int8_t conid, int8_t ctxid) : UC20_internet(ctxid) {
	if(0 <= conid && conid <= 11) {
		_connectid = conid;
	}
	else {
		_connectid = 11;
	}
	_userid = _connectid+100;
}

bool UC20_UDP::connect(const char* server, const char* port) {
	gsm.print(F("AT+QIOPEN="));
	gsm.print(_contextid);
	gsm.print(F(","));
	gsm.print(_connectid);
	gsm.print(F(",\"UDP\",\""));
	gsm.print(server);
	gsm.print(F("\","));
	gsm.print(port);
	gsm.println(",0,0");
	gsm.waitStr(F("OK"),1000);
	String resp = gsm.waitStr(F("+QIOPEN:"),12000);
	if(resp != "") {
		int8_t index = resp.indexOf(F(","))+1;
		int err = resp.substring(index).toInt();
		if(err == 0) 
			return true;
		else 
			return false;
	}
	else 
		return false;
}

bool UC20_UDP::connect(const char* server, unsigned int port) {
	String str = (String)port;
	char c[str.length()+1];
	for(int i=0; i<str.length(); i++) {
		c[i] = str[i];
		c[i+1] = 0;
	}
	return connect(server, c);
}

bool UC20_UDP::connected() {
	gsm.print(F("AT+QISTATE=1,"));
	gsm.println(_connectid);
	String resp = gsm.waitStr(F("+QISTATE:"));
	if(resp != "") {
		int8_t index = 0;
		for(uint8_t i=0; i<5; i++) {
			index = resp.indexOf(F(","),index)+1;
		}
		int8_t state = resp.substring(index).toInt();
		return (state == 2);
	}
	else 
		return false;
}

bool UC20_UDP::disconnect() {
	gsm.print(F("AT+QICLOSE="));
	gsm.println(_connectid);
	return (gsm.waitStr(F("OK"),12000) != "");
}

bool UC20_UDP::beginSend(int len) {
	gsm.print(F("AT+QISEND="));
	gsm.print(_connectid);
	if(len != -1) {
		gsm.print(F(","));
		gsm.print(len);
	}
	gsm.println();
	if(gsm.waitStr(F(">")) != "") 
		return true;
	else 
		return false;	
}

bool UC20_UDP::endSend() {
	gsm.write(0x1A);
	return (gsm.waitStr(F("SEND OK")) != "");
}

int8_t UC20_UDP::waitrecv(unsigned long t_out) {
	String resp = gsm.waitStr(F("+QIURC: \"recv\""), t_out);
	if(resp != "") {
		int8_t index = resp.indexOf(F(","))+1;
		return resp.substring(index).toInt();
	}
	else 
		return -1;
}

long UC20_UDP::available() {
	gsm.print(F("AT+QIRD="));
	gsm.print(_connectid);
	gsm.println(F(",0"));
	String resp = gsm.waitStr(F("+QIRD:"));
	long qibuf = 0;
	if(gsm.getUser_buf() == _userid) 
		qibuf += gsm.available_buf();
	if(resp != "") {
		int8_t index = resp.lastIndexOf(F(","))+1;
		gsm.waitStr(F("OK"));
		qibuf += resp.substring(index).toInt();
	}
	return qibuf;
}

int UC20_UDP::read() {
	if(!(gsm.available_buf() > 0) || (gsm.getUser_buf() != _userid)) {
		gsm.print(F("AT+QIRD="));
		gsm.print(_connectid);
		gsm.print(F(","));
		gsm.println(BUFFER_SIZE);
		String resp = gsm.waitStr(F("+QIRD:"));
		if(resp != "") {
			int8_t index = resp.indexOf(F(" "))+1;
			int data_size = resp.substring(index).toInt();
			gsm.readKeepFor(data_size,_userid);		// keep data to buf
			gsm.waitStr(F("OK"));
		}
	}
	return gsm.read_buf();
}

String UC20_UDP::readString() {
	if(!(gsm.available_buf() > 0) || (gsm.getUser_buf() != _userid)) {
		gsm.print(F("AT+QIRD="));
		gsm.print(_connectid);
		gsm.print(F(","));
		gsm.println(BUFFER_SIZE);
		String resp = gsm.waitStr(F("+QIRD:"));
		if(resp != "") {
			int8_t index = resp.indexOf(F(" "))+1;
			int data_size = resp.substring(index).toInt();
			gsm.readKeepFor(data_size,_userid);			// keep data to buf
			gsm.waitStr(F("OK"));
		}
	}
	return gsm.readString_buf();
}









