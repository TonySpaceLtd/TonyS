#include "UC20_LineNotify.h"

void UC20_LineNotify::begin(const char* token) {
	_client.initConfig();
	_client.responseHeader(false);
	_client.url(LINEHOST);
	_token = (String)token;
}

void UC20_LineNotify::sticker(long StkPkgID, long StkID) {
	_data[1] = (String)StkPkgID;
	_data[2] = (String)StkID;
	_dataFlag |= 0x06;
}

void UC20_LineNotify::imageurl(String url) {
	_data[3] = url;
	_dataFlag |= 0x08;
}

void UC20_LineNotify::silentNotify(bool enable) {
	_data[4] = enable? "true":"false";
	_dataFlag |= 0x10;
}

bool UC20_LineNotify::submit(String msg) {
	if(msg != "") {
		_data[0] = msg;
		_dataFlag |= 0x01;
	}
	else 
		return false;
	
	uint8_t paramSize[] = {8,18,11,31,22};	// length of param name
	unsigned int data_len = 0;
	uint8_t ptrFlag = 0x01;
	for(uint8_t i=0; i<5; i++) {
		if((_dataFlag&ptrFlag) == ptrFlag) {
			data_len += paramSize[i];
			data_len += _data[i].length();
			if(i==3) 
				data_len += _data[i].length();
		}
		ptrFlag = ptrFlag<<1;
	}
	unsigned int packet_len = 171;
	packet_len += _token.length();
	packet_len += String(data_len).length();
	packet_len += data_len;
	if(_client.post(packet_len, CUSTOM_HEADER)) {
		gsm.println(F("POST /api/notify HTTP/1.1"));						// 27
		gsm.println(F("Host: notify-api.line.me"));							// 26
		gsm.print(F("Authorization: Bearer "));								// 22
		gsm.println(_token);												// 2+
		gsm.println(F("Cache-Control: no-cache"));							// 25
		gsm.println(F("Content-Type: application/x-www-form-urlencoded"));	// 49
		gsm.print(F("Content-Length: "));									// 16
		gsm.println(data_len);												// 2+
		gsm.println();														// 2
		ptrFlag = 0x01;
		for(uint8_t i=0; i<5; i++) {
			if((_dataFlag&ptrFlag) == ptrFlag) {
				switch(ptrFlag) {
					case 0x01:
						gsm.print(F("message="));							// 8
						gsm.print(_data[i]);								// +
						break;
					case 0x02:
						gsm.print(F("&stickerPackageId="));					// 18
						gsm.print(_data[i]);								// +
						break;
					case 0x04:
						gsm.print(F("&stickerId="));						// 11
						gsm.print(_data[i]);								// +
						break;
					case 0x08:
						gsm.print(F("&imageThumbnail="));					// 16
						gsm.print(_data[i]);								// +
						gsm.print(F("&imageFullsize="));					// 15
						gsm.print(_data[i]);								// +
						break;
					case 0x10:
						gsm.print(F("&notificationDisabled="));				// 22
						gsm.print(_data[i]);								// +
						break;
				}
			}
			ptrFlag = ptrFlag<<1;
		}
		gsm.waitStr(F("OK"));
		clear();				// clear msg
		return (_client.wait_responseCode() == 200);
	}
	else 
		return false;
}

void UC20_LineNotify::clear() {
	for(uint8_t i=0; i<4; i++) 
		_data[i] = "";
	_dataFlag = 0x00;
}
 
long UC20_LineNotify::available() {
	return _client.available();
}

int UC20_LineNotify::read() {
	return _client.read();
}









