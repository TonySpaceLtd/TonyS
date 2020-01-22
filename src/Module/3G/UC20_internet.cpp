#include "UC20_internet.h"

UC20_internet::UC20_internet(int8_t ctxid) {
	if(1 <= ctxid && ctxid <= 16)
		_contextid = ctxid;
	else 
		_contextid = 1;
}

void UC20_internet::configContext(const char* apn, const char* user, const char* pass) {
	gsm.print(F("AT+QICSGP="));
	gsm.print(_contextid);
	gsm.print(F(",1,\""));
	gsm.print(apn);
	gsm.print(F("\",\""));
	gsm.print(user);
	gsm.print(F("\",\""));
	gsm.print(pass);
	gsm.println(F("\",1"));
	gsm.waitStr(F("OK"),1000);
}

bool UC20_internet::activate() {
	gsm.print(F("AT+QIACT="));
	gsm.println(_contextid);
	return (gsm.waitStr(F("OK"),12000) != "");
}

bool UC20_internet::activated() {
	uint8_t i = 2;
	do {
		--i;
		gsm.println(F("AT+QIACT?"));
		if(gsm.waitStr(F("+QIACT:"),1000) != "") {
			gsm.waitStr(F("OK"));
			return true;
		}
	}
	while(i > 0);
	return false;
}

bool UC20_internet::deactivate() {
	gsm.print(F("AT+QIDEACT="));
	gsm.println(_contextid);
	return (gsm.waitStr(F("OK"),12000) != "");
}

String UC20_internet::getLocalIP() {
	gsm.println(F("AT+QIACT?"));
	String resp = gsm.waitStr(F("+QIACT:"));
	String IP = "";
	if(resp != "") {
		int8_t index1 = resp.indexOf(F("\""))+1;
		int8_t index2 = resp.lastIndexOf(F("\""));
		IP = resp.substring(index1,index2);
		gsm.waitStr(F("OK"));
	}
	return IP;
}

bool UC20_internet::ping(const char* server) {
	gsm.print(F("AT+QPING="));
	gsm.print(_contextid);
	gsm.print(F(",\""));
	gsm.print(server);
	gsm.println(F("\",1,1"));
	gsm.waitStr(F("OK"),1000);
	String resp = gsm.waitStr(F("+QPING:"),5000);
	if(resp != "") {
		int8_t index = resp.indexOf(F(" "))+1;
		int result = resp.substring(index).toInt();
		gsm.waitStr(F("+QPING:"),5000);
		return (result == 0);
	}
	else 
		return false;
}

void UC20_internet::pingcmd(const char* server) {
	gsm.print(F("AT+QPING="));
	gsm.print(_contextid);
	gsm.print(F(",\""));
	gsm.print(server);
	gsm.println(F("\""));
	gsm.waitStr(F("OK"),1000);
	String resp = "";
	for(uint8_t i=0; i<4; i++) {	// pingnum = 4 (as default)
		resp = gsm.waitStr(F("+QPING:"),5000);
		if(resp != "") {
			int8_t index1 = resp.indexOf(F(" "))+1;
			int result = resp.substring(index1).toInt();
			if(result == 0) {		// received the ping response
				int8_t index2 = resp.indexOf(F("\""))+1;
				int8_t index3 = resp.lastIndexOf(F("\""));
				gsm.debug->print(F("Reply from "));					//##### Serial
				gsm.debug->print(resp.substring(index2,index3));	//##### Serial
				index2 = resp.indexOf(F(","),index3)+1;
				index3 = resp.indexOf(F(","),index2);
				gsm.debug->print(F(": bytes="));					//##### Serial
				gsm.debug->print(resp.substring(index2,index3));	//##### Serial
				index2 = index3+1;
				index3 = resp.indexOf(F(","),index2);
				gsm.debug->print(F(" time="));						//##### Serial
				gsm.debug->print(resp.substring(index2,index3));	//##### Serial
				index2 = index3+1;
				gsm.debug->print(F("ms TTL="));						//##### Serial
				gsm.debug->println(resp.substring(index2));			//##### Serial
			}
			else 
				gsm.debug->println(F("Request timed out."));		//##### Serial
		}
		else 
			return;
	}
	resp = gsm.waitStr(F("+QPING:"),5000);		// finresult
	if(resp != "") {
		int8_t index1 = resp.indexOf(F(" "))+1;
		int result = resp.substring(index1).toInt();
		if(result == 2) {
			int8_t index2 = resp.indexOf(F(","))+1;
			int8_t index3 = resp.indexOf(F(","),index2);
			gsm.debug->print(F("Packets: Sent = "));					//##### Serial
			gsm.debug->print(resp.substring(index2,index3));			//##### Serial
			index2 = index3+1;
			index3 = resp.indexOf(F(","),index2);
			int recv_n = resp.substring(index2,index3).toInt();
			gsm.debug->print(F(", Received = "));						//##### Serial
			gsm.debug->print(resp.substring(index2,index3));			//##### Serial
			index2 = index3+1;
			index3 = resp.indexOf(F(","),index2);
			gsm.debug->print(F(", Lost = "));							//##### Serial
			gsm.debug->println(resp.substring(index2,index3));			//##### Serial
			if(recv_n > 0) {
				index2 = index3+1;
				index3 = resp.indexOf(F(","),index2);
				gsm.debug->print(F("Round trip times: Minimum = "));	//##### Serial
				gsm.debug->print(resp.substring(index2,index3));		//##### Serial
				index2 = index3+1;
				index3 = resp.indexOf(F(","),index2);
				gsm.debug->print(F("ms, Maximum = "));					//##### Serial
				gsm.debug->print(resp.substring(index2,index3));		//##### Serial
				index2 = index3+1;
				index3 = resp.indexOf(F(","),index2);
				gsm.debug->print(F("ms, Average = "));					//##### Serial
				gsm.debug->print(resp.substring(index2,index3));		//##### Serial
				gsm.debug->println(F("ms"));							//##### Serial
			}
		}
	}
}

bool UC20_internet::getNTP(const char* server, unsigned int port, bool settime) {
	gsm.print(F("AT+QNTP="));
	gsm.print(_contextid);
	gsm.print(F(",\""));
	gsm.print(server);
	gsm.print(F("\""));
	if(port != -1) {
		gsm.print(F(","));
		gsm.print(port);
		gsm.print(F(","));
		gsm.print(settime, DEC);
	}
	gsm.println();
	return (gsm.waitStr(F("OK")) != "");
}

String UC20_internet::recvNTP(unsigned long t_out) {
	String resp = gsm.waitStr(F("+QNTP:"), t_out);
	String t = "";
	if(resp != "") {
		if(resp.indexOf(F(" 0,")) != -1) {
			int8_t index1 = resp.indexOf(F("\""))+1;
			int8_t index2 = resp.lastIndexOf(F("\""));
			t = resp.substring(index2,index2);
		}
	}
	return t;
}









