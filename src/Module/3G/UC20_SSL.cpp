#include "UC20_SSL.h"

UC20_SSL::UC20_SSL(int8_t sslctxid) {
	_sslctxid = sslctxid;
}

bool UC20_SSL::sslversion(int8_t version) {
	gsm.print(F("AT+QSSLCFG=\"sslversion\","));
	gsm.print(_sslctxid);
	gsm.print(F(","));
	gsm.println(version);
	return (gsm.waitStr(F("OK")) != "");
}

bool UC20_SSL::ciphersuite(unsigned int type) {
	unsigned int cipherType[] = {0x0035,0x002F,0x0005,0x0004,0x000A,0x003D,0xFFFF};
	uint8_t i = 0;
	while(cipherType[i] != type) {
		++i;
		if(i >= 7) 
			return false;
	}
	gsm.print(F("AT+QSSLCFG=\"ciphersuite\","));
	gsm.print(_sslctxid);
	gsm.print(F(",0X"));
	if(type < 0x100) 
		gsm.print(F("00"));
	if(type < 0x10) 
		gsm.print(F("0"));
	gsm.println(type,HEX);
	return (gsm.waitStr(F("OK")) != "");
}

bool UC20_SSL::seclevel(int8_t level) {
	if(level < 0 || 2 < level) 
		return false;
	gsm.print(F("AT+QSSLCFG=\"seclevel\","));
	gsm.print(_sslctxid);
	gsm.print(F(","));
	gsm.println(level);
	return (gsm.waitStr(F("OK")) != "");
}









