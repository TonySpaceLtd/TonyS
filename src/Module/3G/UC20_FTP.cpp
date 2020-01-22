#include "UC20_FTP.h"

UC20_FTP::UC20_FTP(int8_t sslctxid, int8_t ctxid) : UC20_internet(ctxid), UC20_SSL(sslctxid) {
	
}

void UC20_FTP::initConfig() {
	gsm.print(F("AT+QFTPCFG=\"contextid\","));
	gsm.println(_contextid);
	gsm.waitStr(F("OK"),1000);
	fileType(BINARY);
	disableSSL();
}

void UC20_FTP::account(const char* user, const char* pass) {
	gsm.print(F("AT+QFTPCFG=\"account\",\""));
	gsm.print(user);
	gsm.print(F("\",\""));
	gsm.print(pass);
	gsm.println(F("\""));
	gsm.waitStr(F("OK"),1000);
}

void UC20_FTP::fileType(bool type) {
	gsm.print(F("AT+QFTPCFG=\"filetype\","));
	gsm.println(type, DEC);
	gsm.waitStr(F("OK"),1000);
}

// void UC20_FTP::enableSSL() {
	// gsm.println(F("AT+QFTPCFG=\"ssltype\",1"));
	// gsm.waitStr(F("OK"),1000);
	// gsm.print(F("AT+QFTPCFG=\"sslctxid\","));
	// gsm.println(_sslctxid);
	// gsm.waitStr(F("OK"),1000);
	// sslversion(1);
	// ciphersuite(0xFFFF);
	// seclevel(0);
// }

void UC20_FTP::disableSSL() {
	gsm.println(F("AT+QFTPCFG=\"ssltype\",0"));
	gsm.waitStr(F("OK"));
}

int UC20_FTP::login(const char* server, const char* port) {
	gsm.print(F("AT+QFTPOPEN=\""));
	gsm.print(server);
	gsm.print(F("\","));
	gsm.println(port);
	String resp = gsm.waitStr(F("+QFTPOPEN:"),12000,F("+CME ERROR:"));
	if(resp != "") {
		int8_t index = resp.indexOf(F(" "))+1;
		return resp.substring(index).toInt();
	}
	else 
		return -1;
}

int UC20_FTP::login(const char* server, unsigned int port) {
	String str = (String)port;
	char c[str.length()+1];
	for(int i=0; i<str.length(); i++) {
		c[i] = str[i];
		c[i+1] = 0;
	}
	return login(server, c);
}

bool UC20_FTP::logout() {
	gsm.println(F("AT+QFTPCLOSE"));
	String resp = gsm.waitStr(F("+QFTPCLOSE:"),12000,F("+CME ERROR:"));
	if(resp != "") {
		if(resp.indexOf(F(" 0,0")) != -1) 
			return true;
		else 
			return false;
	}
	else 
		return false;
}

String UC20_FTP::getPath() {
	gsm.println(F("AT+QFTPPWD"));
	gsm.waitStr(F("OK"));
	String resp = gsm.waitStr(F("+QFTPPWD:"),5000);
	if(resp != "") {
		if(resp.indexOf(" 0,") != -1) {
			int8_t index1 = resp.indexOf(F("\""))+1;
			int8_t index2 = resp.lastIndexOf(F("\""));
			return resp.substring(index1,index2);
		}
		else 
			return "";
	}
	else 
		return "";
}

bool UC20_FTP::setPath(String path) {
	gsm.print(F("AT+QFTPCWD=\""));
	gsm.print(path);
	gsm.println(F("\""));
	gsm.waitStr(F("OK"));
	String resp = gsm.waitStr(F("+QFTPCWD:"),5000);
	if(resp != "") {
		if(resp.indexOf(F(" 0,0")) != -1) 
			return true;
		else 
			return false;
	}
	else 
		return false;
}

void UC20_FTP::list(String path, bool detail) {
	if(detail) 
		gsm.print(F("AT+QFTPLIST=\""));
	else 
		gsm.print(F("AT+QFTPNLST=\""));
	gsm.print(path);
	gsm.println(F("\""));
	if(gsm.waitStr(F("CONNECT"),10000) != "") {
		gsm.debugReadUntil("\r\nOK\r\n");
		gsm.waitStr(F("+QFTPLIST:"));
	}
}

bool UC20_FTP::mkDir(String dirname) {
	gsm.print(F("AT+QFTPMKDIR=\""));
	gsm.print(dirname);
	gsm.println(F("\""));
	gsm.waitStr(F("OK"));
	String resp = gsm.waitStr(F("+QFTPMKDIR:"),5000);
	if(resp != "") {
		if(resp.indexOf(F(" 0,0")) != -1) 
			return true;
		else 
			return false;
	}
	else 
		return false;
}

bool UC20_FTP::delDir(String dirname) {
	gsm.print(F("AT+QFTPRMDIR=\""));
	gsm.print(dirname);
	gsm.println(F("\""));
	gsm.waitStr(F("OK"));
	String resp = gsm.waitStr(F("+QFTPRMDIR:"),5000);
	if(resp != "") {
		if(resp.indexOf(F(" 0,0")) != -1) 
			return true;
		else 
			return false;
	}
	else 
		return false;
}

bool UC20_FTP::del(String fn) {
	gsm.print(F("AT+QFTPDEL=\""));
	gsm.print(fn);
	gsm.println(F("\""));
	gsm.waitStr(F("OK"));
	String resp = gsm.waitStr(F("+QFTPDEL:"),5000);
	if(resp != "") {
		if(resp.indexOf(F(" 0,0")) != -1) 
			return true;
		else 
			return false;
	}
	else 
		return false;
}

bool UC20_FTP::rename(String old_name, String new_name) {
	gsm.print(F("AT+QFTPRENAME=\""));
	gsm.print(old_name);
	gsm.print(F("\",\""));
	gsm.print(new_name);
	gsm.println(F("\""));
	gsm.waitStr(F("OK"));
	String resp = gsm.waitStr(F("+QFTPRENAME:"),5000);
	if(resp != "") {
		if(resp.indexOf(F(" 0,0")) != -1) 
			return true;
		else 
			return false;
	}
	else 
		return false;
}

bool UC20_FTP::put(String fn, String localname, int startpos) {
	gsm.print(F("AT+QFTPPUT=\""));
	gsm.print(fn);
	gsm.print(F("\",\""));
	gsm.print(localname);
	gsm.print(F("\""));
	if(startpos != -1) {
		gsm.print(F(","));
		gsm.print(startpos);
	}
	gsm.println();
	if(localname != "COM:") 
		return (gsm.waitStr(F("OK")) != "");
	else 
		return (gsm.waitStr(F("CONNECT")) != "");
}

bool UC20_FTP::wait_putdone() {
	String resp = gsm.waitStr(F("+QFTPPUT:"),12000,F("+CME ERROR:"));
	if(resp != "") 
		return (resp.indexOf(F(" 0,")) != -1);
	else 
		return false;
}

bool UC20_FTP::get(String fn, String localname, int startpos) {
	gsm.print(F("AT+QFTPGET=\""));
	gsm.print(fn);
	gsm.print(F("\",\""));
	gsm.print(localname);
	gsm.print(F("\""));
	if(startpos != -1) {
		gsm.print(F(","));
		gsm.print(startpos);
	}
	gsm.println();
	if(localname == "COM:") {
		if(gsm.waitStr(F("CONNECT"),12000) != "") 
			gsm.debugReadUntil("\r\nOK\r\n");
		else 
			return false;
	}
	else 
		gsm.waitStr(F("OK"));
	String resp = gsm.waitStr(F("+QFTPGET:"),12000);
	if(resp != "") {
		int8_t index = resp.indexOf(F(" "))+1;
		int err = resp.substring(index).toInt();
		if(err == 0) 
			return true;
		else 
			return false;
	}
	else 
		return false;
}









