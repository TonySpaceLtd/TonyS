#include "UCXX_SER.h"

HardwareSerial Ser3G(NULL);

void UCXX_SER::initial(uint8_t slot,unsigned long baud){
	Ser3G = Tony.SerialBegin(slot,baud);	
	initSerial(Ser3G);
	powerPin = getPinIO[slot];
	Serial.println ("powetPin "+String(powerPin));
}
void UCXX_SER::initSerial(Stream &serial) {
	_ucSerial = &serial; 
}

void UCXX_SER::setDebugSerial(Stream &serial) {
	debug = &serial;
}

size_t UCXX_SER::println() {
	return _ucSerial->print(F("\r\n"));
}

int UCXX_SER::available() {
	return _ucSerial->available();
}

int UCXX_SER::read() {
	return _ucSerial->read();
}

int UCXX_SER::peek() {
	return _ucSerial->peek();
}

String UCXX_SER::readStringUntil(const char terminate) {
	return _ucSerial->readStringUntil(terminate);
}

String UCXX_SER::waitStr(String str, unsigned long t_out, String strNot, bool clrLF) {
	unsigned long prev_t = millis();
	do {
		String temp = readStringUntil('\r');
		// String dbg_temp = String(temp); 						//##### debug
		// dbg_temp.replace("\r","<CR>");  						//##### debug
		// dbg_temp.replace("\n","<LF>");  						//##### debug
		// debug->print(">  ");									//##### debug
		// debug->println(dbg_temp);   							//##### debug
		if(temp.indexOf(str) != -1) {
			if(clrLF) 
				readStringUntil('\n');
			return temp;
		}
		else if((strNot != "") && (temp.indexOf(strNot) != -1)) {
			if(clrLF) 
				readStringUntil('\n');
			return "";
		}
		else {}
	}
	while(millis()-prev_t < t_out);
	return "";
}

long UCXX_SER::debugReadUntil(const char* terminate) {
	long data_size = 0;				// return value
	int tlen = strlen(terminate);
	int cindex = 0;
	char c[tlen+1];
	unsigned long prev_t;
	while(1) {
		prev_t = millis();
		while(!(available() > 0)) {
			if(millis()-prev_t > 1000) {
				for(int i=0; i<cindex; i++) {
					debug->write(c[i]);				//##### Serial
					++data_size;
				}
				return data_size;
			}
		}
		c[cindex] = read();
		if(c[cindex] == terminate[cindex]) {
			++cindex;
			if(cindex == tlen) 
				return data_size;
		}
		else {
			if(cindex == 0) {
				debug->write(c[0]);					//##### Serial
				++data_size;
			}
			else {
				for(int i=0; i<cindex; i++) {
					debug->write(c[i]);				//##### Serial
					++data_size;
				}
				if(c[cindex] == terminate[0]) {
					c[0] = c[cindex];
					cindex = 1;
				}
				else {
					debug->write(c[cindex]);		//##### Serial
					++data_size;
					cindex = 0;
				}
			}
		}
	}
}

void UCXX_SER::readKeepUntil(const char* terminate, int userid) {
	clear_buf();
	buf_owner_id = userid;
	int tlen = strlen(terminate);
	int cindex = 0;
	char c[tlen+1];
	readUntilEnd = false;
	unsigned long prev_t;
	while(1) {
		prev_t = millis();
		while(!(available() > 0)) {
			if(millis()-prev_t > 1000) {				// timeout
				for(int i=0; i<cindex; i++) 
					charT[buf_len++] = c[i];
				readUntilEnd = true;
				return;
			}
		}
		c[cindex] = read();
		if(c[cindex] == terminate[cindex]) {
			++cindex;
			if(cindex == tlen) {						// success
				readUntilEnd = true;
				return;
			}
		}
		else {
			if(cindex == 0) {
				charT[buf_len++] = c[0];
			}
			else {
				for(int i=0; i<cindex; i++) 
					charT[buf_len++] = c[i];
				if(c[cindex] == terminate[0]) {
					c[0] = c[cindex];
					cindex = 1;
				}
				else {
					charT[buf_len++] = c[cindex];
					cindex = 0;
				}
			}
		}
		if(buf_len+cindex > BUFFER_SIZE-(tlen-1) || buf_len >= BUFFER_SIZE) {	// out of range
			while(!(available() > 0)) {
				if(millis()-prev_t > 1000) 
					break;
			}
			if(cindex == 0) 
				return;
			else if(peek() != terminate[cindex]) {
				for(int i=0; i<cindex; i++) 
					charT[buf_len++] = c[i];
				return;
			}
		}
	}
}

void UCXX_SER::readKeepFor(unsigned int times, int userid) {
	clear_buf();
	buf_owner_id = userid;
	if(times > BUFFER_SIZE) 
		times = BUFFER_SIZE;
	unsigned long prev_t;
	for(unsigned int i=0; i<times; i++) {
		prev_t = millis();
		while(!(available() > 0)) {
			if(millis()-prev_t > 1000) 
				return;
		}
		charT[buf_len++] = read();
	}
}

bool UCXX_SER::getKeepUntilDone() {
	return readUntilEnd;
}

int UCXX_SER::available_buf() {
	return (buf_len - buf_cur);
}

int UCXX_SER::read_buf() {
	if(buf_cur >= buf_len) 
		return -1;
	else 
		return (int)buf[buf_cur++] & 0xFF;
}

String UCXX_SER::readString_buf() {
	String str = (String)buf;
	str.remove(0,buf_cur);
	buf_cur = buf_len;
	return str;
}

void UCXX_SER::clear_buf() {
	buf_cur = 0;
	buf_len = 0;
	buf = charT;
	for(int i=0; i<BUFFER_SIZE+1; i++) {
		charT[i] = 0;
	}
}

int UCXX_SER::getUser_buf() {
	return buf_owner_id;
}

void UCXX_SER::printUnicode(String str) {
	print(utf8ToUcode(str));
}

void UCXX_SER::printlnUnicode(String str) {
	print(utf8ToUcode(str));
	print(utf8ToUcode(F("\r\n")));
}

void UCXX_SER::printlnUnicode() {
	print(utf8ToUcode(F("\r\n")));
}

uint8_t UCXX_SER::strToHex(String str) {
	const char hex_chr[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	const uint8_t hex_val[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	str.toUpperCase();
	uint8_t val = 0;
	for(uint8_t i=0; i<2; i++) {
		for(uint8_t j=0; j<sizeof(hex_chr); j++) {
			if(str[i] == hex_chr[j]) {
				val = (val<<4) | hex_val[j];
				break;
			}
		}
	}
	return val;
}

String UCXX_SER::ucodeToUtf8(String ucode, int len_byte) {
	int len = len_byte * 2;
	String utf8 = "";
	unsigned long unic = 0;
	if(ucode.length() >= len) {
		for(uint8_t i=0; i<len/2; i++) {
			unic = (unic<<8) ^ strToHex(ucode.substring(0,2));
			ucode.remove(0,2);
		}
		if(unic < 0x80) {
			utf8 += char(unic>>0 & 0x7F | 0x00);
		}
		else if(unic < 0x0800) {
			utf8 += char(unic>>6 & 0x1F | 0xC0);
			utf8 += char(unic>>0 & 0x3F | 0x80);
		}
		else if(unic < 0x010000) {
			utf8 += char(unic>>12 & 0x0F | 0xE0);
			utf8 += char(unic>>6  & 0x3F | 0x80);
			utf8 += char(unic>>0  & 0x3F | 0x80);
		}
		else if(unic < 0x110000) {
			utf8 += char(unic>>18 & 0x07 | 0xF0);
			utf8 += char(unic>>12 & 0x3F | 0x80);
			utf8 += char(unic>>6  & 0x3F | 0x80);
			utf8 += char(unic>>0  & 0x4F | 0x80);
		}
	}
	return utf8;
}

String UCXX_SER::utf8ToUcode(String utf8) {
	String ucode = "";
	int i = 0;
	while(i < utf8.length()) {
		uint8_t n = utf8[i++];
		unsigned long unic = 0;
		if(n < 0xC0) {
			ucode += "00";
			if(n < 0x10) 
				ucode += "0";
			ucode += String(n,HEX);
		}
		else if(n < 0xE0) {
			ucode += "0";
			unic = unic<<0 | (n & 0x3F);
			n = utf8[i++];
			unic = unic<<6 | (n & 0x7F);
			if(unic < 0x100) 
				ucode += "0";
			ucode += String(unic,HEX);
		}
		else if(n < 0xF0) {
			unic = unic<<0 | (n & 0x1F);
			n = utf8[i++];
			unic = unic<<6 | (n & 0x7F);
			n = utf8[i++];
			unic = unic<<6 | (n & 0x7F);
			if(unic < 0x1000) 
				ucode += "0";
			ucode += String(unic,HEX);
		}
		else if(n < 0xF5) {
			unic = unic<<0 | (n & 0x0F);
			n = utf8[i++];
			unic = unic<<6 | (n & 0x7F);
			n = utf8[i++];
			unic = unic<<6 | (n & 0x7F);
			n = utf8[i++];
			unic = unic<<6 | (n & 0x7F);
			ucode += String(unic,HEX);
		}
	}
	ucode.toUpperCase();
	return ucode;
}









