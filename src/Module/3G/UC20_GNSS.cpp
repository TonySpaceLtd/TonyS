#include "UC20_GNSS.h"

bool UC20_GNSS::begin() {
	gsm.println(F("AT+QGPS=1"));
	return (gsm.waitStr(F("OK")) != "");
}

bool UC20_GNSS::end() {
	gsm.println(F("AT+QGPSEND"));
	return (gsm.waitStr(F("OK")) != "");
}

bool UC20_GNSS::setNMEA(bool enable) {
	gsm.print(F("AT+QGPSCFG=\"nmeasrc\","));
	gsm.println(enable, DEC);
	return (gsm.waitStr(F("OK")) != "");
}

String UC20_GNSS::getNMEA(const char* type) {
	gsm.print(F("AT+QGPSGNMEA=\""));
	gsm.print(type);
	gsm.println(F("\""));
	String nm = "";
	String resp = "";
	do {
		resp = gsm.waitStr(F("+QGPSGNMEA:"));
		if(resp != "") {
			if(nm != "") {
				nm += "\r\n";
			}
			int index = resp.indexOf(F(" "))+1;
			nm += resp.substring(index);
		}
	}
	while(resp != "");
	return nm;
}

bool UC20_GNSS::getPosition() {
	gsm.println(F("AT+QGPSLOC=2"));
	String resp = gsm.waitStr(F("+QGPSLOC:"),3000,F("+CME ERROR: 5"));
	if(resp != "") {
		int8_t index = resp.indexOf(F(" "))+1;
		gpsInfo = resp.substring(index);
		gsm.waitStr(F("OK"));
		return true;
	}
	else 
		return false;
}

String UC20_GNSS::getInfo() {
	return gpsInfo;
}

String UC20_GNSS::date() {
	int8_t index1 = 0;
	int8_t index2;
	for(int8_t i=0; i<9; i++) 
		index1 = gpsInfo.indexOf(F(","),index1)+1;
	index2 = gpsInfo.indexOf(F(","),index1);
	if(index2 != -1) {
		String d = gpsInfo.substring(index1,index2);
		return d.substring(0,2)+":"+d.substring(2,4)+":"+d.substring(4);
	}
	else 
		return "";
}

String UC20_GNSS::time(int8_t timezone) {
	int8_t index = gpsInfo.indexOf(F("."));
	if(index != -1) {
		String t = gpsInfo.substring(0,index);
		int h = (t.substring(0,2).toInt()) + timezone;
		if(h >= 24) 
			h = h - 24;
		else if(h < 0) 
			h = h + 24;
		String strh = "";
		if(h<10) 
			strh += "0";
		strh += String(h);
		return strh+"/"+t.substring(2,4)+"/"+t.substring(4);
	}
	else 
		return "";
}

float UC20_GNSS::latitude() {
	int8_t index1 = gpsInfo.indexOf(F(","))+1;
	int8_t index2 = gpsInfo.indexOf(F(","),index1);
	if(index2 != -1) 
		return gpsInfo.substring(index1,index2).toFloat();
	else 
		return 0;
}

float UC20_GNSS::longtitude() {
	int8_t index1 = 0;
	int8_t index2;
	for(int8_t i=0; i<2; i++) 
		index1 = gpsInfo.indexOf(F(","),index1)+1;
	index2 = gpsInfo.indexOf(F(","),index1);
	if(index2 != -1) 
		return gpsInfo.substring(index1,index2).toFloat();
	else 
		return 0;
}

float UC20_GNSS::hdop() {
	int8_t index1 = 0;
	int8_t index2;
	for(int8_t i=0; i<3; i++) 
		index1 = gpsInfo.indexOf(F(","),index1)+1;
	index2 = gpsInfo.indexOf(F(","),index1);
	if(index2 != -1) 
		return gpsInfo.substring(index1,index2).toFloat();
	else 
		return 0;
}

float UC20_GNSS::altitude() {
	int8_t index1 = 0;
	int8_t index2;
	for(int8_t i=0; i<4; i++) 
		index1 = gpsInfo.indexOf(F(","),index1)+1;
	index2 = gpsInfo.indexOf(F(","),index1);
	if(index2 != -1) 
		return gpsInfo.substring(index1,index2).toFloat();
	else 
		return 0;
}

int UC20_GNSS::fix() {
	int8_t index1 = 0;
	int8_t index2;
	for(int8_t i=0; i<5; i++) 
		index1 = gpsInfo.indexOf(F(","),index1)+1;
	index2 = gpsInfo.indexOf(F(","),index1);
	if(index2 != -1) 
		return gpsInfo.substring(index1,index2).toInt();
	else 
		return 0;
}

String UC20_GNSS::cog() {
	int8_t index1 = 0;
	int8_t index2;
	for(int8_t i=0; i<6; i++) 
		index1 = gpsInfo.indexOf(F(","),index1)+1;
	index2 = gpsInfo.indexOf(F(","),index1);
	if(index2 != -1) 
		return gpsInfo.substring(index1,index2);
	else 
		return "";
}

float UC20_GNSS::spkm() {
	int8_t index1 = 0;
	int8_t index2;
	for(int8_t i=0; i<7; i++) 
		index1 = gpsInfo.indexOf(F(","),index1)+1;
	index2 = gpsInfo.indexOf(F(","),index1);
	if(index2 != -1) 
		return gpsInfo.substring(index1,index2).toFloat();
	else 
		return 0;
}

float UC20_GNSS::spkn() {
	int8_t index1 = 0;
	int8_t index2;
	for(int8_t i=0; i<8; i++) 
		index1 = gpsInfo.indexOf(F(","),index1)+1;
	index2 = gpsInfo.indexOf(F(","),index1);
	if(index2 != -1) 
		return gpsInfo.substring(index1,index2).toFloat();
	else 
		return 0;
}

int UC20_GNSS::nsat() {
	int8_t index = gpsInfo.lastIndexOf(F(","))+1;
	if(index != -1) 
		return gpsInfo.substring(index).toInt();
	else 
		return 0;
}









