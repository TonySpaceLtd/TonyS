#include "UC20_TCP.h"

UC20_TCP::UC20_TCP(int8_t conid, int8_t sslctxid, int8_t ctxid) : UC20_internet(ctxid), UC20_SSL(sslctxid) {
	if(0 <= conid && conid <= 11) {
		_connectid = conid;
	}
	else {
		_connectid = 11;
	}
	_userid = _connectid+100;
}

void UC20_TCP::enableSSL() {
	disconnect();
	sslversion(1);
	ciphersuite(0xFFFF);
	seclevel(0);
	withSSL = true;
}

void UC20_TCP::disableSSL() {
	disconnect();
	withSSL = false;
}

bool UC20_TCP::connect(const char* server, const char* port) {
	String resp;
	if(!withSSL) {
		gsm.print(F("AT+QIOPEN="));
		gsm.print(_contextid);
		gsm.print(F(","));
		gsm.print(_connectid);
		gsm.print(F(",\"TCP\",\""));
		gsm.print(server);
		gsm.print(F("\","));
		gsm.print(port);
		gsm.println(",0,0");
		gsm.waitStr(F("OK"),1000);
		resp = gsm.waitStr(F("+QIOPEN:"),12000);
	}
	else {
		gsm.print(F("AT+QSSLOPEN="));
		gsm.print(_contextid);
		gsm.print(F(","));
		gsm.print(_sslctxid);
		gsm.print(F(","));
		gsm.print(_connectid);
		gsm.print(F(",\""));
		gsm.print(server);
		gsm.print(F("\","));
		gsm.print(port);
		gsm.println(F(",0"));
		gsm.waitStr(F("OK"),1000);
		resp = gsm.waitStr(F("+QSSLOPEN:"),12000);
	}
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

bool UC20_TCP::connect(const char* server, unsigned int port) {
	String str = (String)port;
	char c[str.length()+1];
	for(int i=0; i<str.length(); i++) {
		c[i] = str[i];
		c[i+1] = 0;
	}
	return connect(server, c);
}

bool UC20_TCP::connected() {
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

bool UC20_TCP::disconnect() {
	gsm.print(F("AT+QICLOSE="));
	gsm.println(_connectid);
	return (gsm.waitStr(F("OK"),12000) != "");
}

bool UC20_TCP::beginSend(int len) {
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

bool UC20_TCP::endSend() {
	gsm.write(0x1A);
	return (gsm.waitStr(F("SEND OK")) != "");
}

int8_t UC20_TCP::waitrecv(unsigned long t_out) {
	unsigned long prev_t = millis();
	while(!(gsm.available() > 0)) {
		if(millis()-prev_t > t_out) 
			return -1;
	}
	String resp = gsm.waitStr(F("URC: \"recv\""), (prev_t+t_out)-millis());
	if(resp != "") {
		int8_t index = resp.indexOf(F(","))+1;
		int8_t conid = resp.substring(index).toInt();
		if(conid == _connectid) 
			recving = true;
		return conid;
	}
	else 
		return -1;
}

long UC20_TCP::available() {
	if(!withSSL) {
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
	else {
		if(gsm.available_buf() > 0 && gsm.getUser_buf() == _userid) 
			return gsm.available_buf();
		else 
			return recving? 1:0;
	}
}

int UC20_TCP::read() {
	if(!(gsm.available_buf() > 0) || (gsm.getUser_buf() != _userid)) {
		if(!withSSL) 
			gsm.print(F("AT+QIRD="));
		else 
			gsm.print(F("AT+QSSLRECV="));
		gsm.print(_connectid);
		gsm.print(F(","));
		gsm.println(BUFFER_SIZE);
		String resp;
		if(!withSSL) 
			resp = gsm.waitStr(F("+QIRD:"));
		else 
			resp = gsm.waitStr(F("+QSSLRECV:"));
		if(resp != "") {
			int8_t index = resp.indexOf(F(" "))+1;
			int data_size = resp.substring(index).toInt();
			gsm.readKeepFor(data_size,_userid); 	// keep data to buf
			gsm.waitStr(F("OK"));
			if(data_size < BUFFER_SIZE) 
				recving = false;
			else 
				recving = true;
		}
	}
	return gsm.read_buf();
}

String UC20_TCP::readString() {
	if(!(gsm.available_buf() > 0) || (gsm.getUser_buf() != _userid)) {
		if(!withSSL) 
			gsm.print(F("AT+QIRD="));
		else 
			gsm.print(F("AT+QSSLRECV="));
		gsm.print(_connectid);
		gsm.print(F(","));
		gsm.println(BUFFER_SIZE);
		String resp;
		if(!withSSL) 
			resp = gsm.waitStr(F("+QIRD:"));
		else 
			resp = gsm.waitStr(F("+QSSLRECV:"));
		if(resp != "") {
			int8_t index = resp.indexOf(F(" "))+1;
			int data_size = resp.substring(index).toInt();
			gsm.readKeepFor(data_size,_userid); 	// keep data to buf
			gsm.waitStr(F("OK"));
			if(data_size < BUFFER_SIZE) 
				recving = false;
			else 
				recving = true;
		}
	}
	return gsm.readString_buf();
}









