#include "UC20_HTTP.h"

UC20_HTTP::UC20_HTTP(int8_t sslctxid, int8_t ctxid) : UC20_internet(ctxid), UC20_SSL(sslctxid) {
	
}

void UC20_HTTP::initConfig() {
	gsm.print(F("AT+QHTTPCFG=\"contextid\","));
	gsm.println(_contextid);
	gsm.waitStr(F("OK"),1000);						// init pdpcontext
	gsm.print(F("AT+QHTTPCFG=\"sslctxid\","));
	gsm.println(_sslctxid);
	gsm.waitStr(F("OK"),1000);						// init sslcontext
	responseHeader(true);							// response header is outputted
	sslversion(1);									// ssl config
	ciphersuite(0xFFFF);							// ssl config
	seclevel(0);									// ssl config
}

void UC20_HTTP::responseHeader(bool enable) {
	gsm.print(F("AT+QHTTPCFG=\"responseheader\","));
	gsm.println(enable,DEC);
	gsm.waitStr(F("OK"),1000);
}

void UC20_HTTP::requestHeader(bool enable) {
	gsm.print(F("AT+QHTTPCFG=\"requestheader\","));
	gsm.println(enable,DEC);
	gsm.waitStr(F("OK"),1000);
}

bool UC20_HTTP::url(const char* server) {
	String protocol = "http://";
	String protocols = "https://";
	String inputs = (String)server;
	inputs.toLowerCase();
	int len = inputs.length();
	bool addstr = !(inputs.startsWith(protocol) || inputs.startsWith(protocols));
	if(addstr) 
		len += protocol.length();
	gsm.print(F("AT+QHTTPURL="));
	gsm.print(len);
	gsm.println(F(",3"));
	if(gsm.waitStr(F("CONNECT")) != "") {
		if(addstr) {
			gsm.print(protocol);
			gsm.print(server);
		}
		else 
			gsm.print(server);
		return (gsm.waitStr(F("OK")) != "");
	}
	else 
		return false;
}

bool UC20_HTTP::get(int len, int8_t mode) {
	recentAct = 1;		// 1 - define as GET
	saved = false;
	if(mode == CUSTOM_HEADER) {
		requestHeader(true);
		gsm.print(F("AT+QHTTPGET=20,"));
		gsm.println(len);
		return (gsm.waitStr(F("CONNECT"),15000) != "");  	// go to print header
	}
	else if(mode == AUTOMATIC_HEADER) {
		requestHeader(false);
		gsm.println(F("AT+QHTTPGET=20"));
		return true;										// wait response code
	}
	else 
		return false;
}	// left "\r\nOK\r\n" after input done

bool UC20_HTTP::post(int len, int8_t mode) {
	recentAct = 2;		// 2 - define as POST
	saved = false;
	if(mode == CUSTOM_HEADER) 
		requestHeader(true);
	else if(mode == AUTOMATIC_HEADER) 
		requestHeader(false);
	else 
		return false;
	gsm.print(F("AT+QHTTPPOST="));
	gsm.print(len);
	gsm.println(F(",60,25"));
	return (gsm.waitStr(F("CONNECT"),15000) != "");
}	// left "\r\nOK\r\n" after input done

int UC20_HTTP::wait_responseCode() {
	String resp = "";
	if(recentAct == 1) 
		resp = gsm.waitStr(F("+QHTTPGET:"),21000);
	else if(recentAct == 2) 
		resp = gsm.waitStr(F("+QHTTPPOST:"),26000);
	if(resp != "") {
		recentAct = 0;
		int8_t index = resp.indexOf(F(","));
		if(index != -1) {
			saveable = true;
			return resp.substring(index+1).toInt();
		}
	}
	return -1;
}

bool UC20_HTTP::save(String fname) {
	int old_handle = httpfile.opened(fname);
	if(old_handle != -1) 
		httpfile.close(old_handle);
	gsm.print(F("AT+QHTTPREADFILE=\""));
	gsm.print(fname);
	gsm.println(F("\""));
	gsm.waitStr(F("OK"));
	saveable = false;
	if(gsm.waitStr(F("+QHTTPREADFILE: 0"),15000) != "") {
		if(fname == HTTPREAD_FILENAME) {
			_fsize = httpfile.filesize(HTTPREAD_FILENAME);
			_fpos = 0;
		}
		return true;
	}
	else 
		return false;
}

long UC20_HTTP::available() {
	if(!saved) {
		if(saveable) {
			if(save(HTTPREAD_FILENAME)) {
				saved = true;
				httpfile.open(HTTPREAD_FILENAME,FMODE_READONLY);
			}
			else 
				return 0;
		}
		else 
			return 0;
	}
	if(_fsize > _fpos) 
		return _fsize - _fpos;
	else 
		return 0;
}

int UC20_HTTP::read() {
	if(!saved && saveable) {
		if(save(HTTPREAD_FILENAME)) {
			saved = true;
			httpfile.open(HTTPREAD_FILENAME,FMODE_READONLY);
		}
	}
	++_fpos;
	return httpfile.read();
}

String UC20_HTTP::readString() {
	if(!saved && saveable) {
		if(save(HTTPREAD_FILENAME)) {
			saved = true;
			httpfile.open(HTTPREAD_FILENAME,FMODE_READONLY);
		}
	}
	_fpos += BUFFER_SIZE;
	return httpfile.readString();
}









