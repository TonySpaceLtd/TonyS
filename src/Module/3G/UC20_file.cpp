#include "UC20_file.h"

void UC20_file::list(String fname) {
	if(fname == "ALL") {
		list(F("RAM:*"));
		list(F("*"));
	}
	else {
		gsm.print(F("AT+QFLST=\""));
		gsm.print(fname);
		gsm.println(F("\""));
	}
	String resp = "";
	do {
		resp = gsm.waitStr(F("+QFLST:"),1000);
		if(resp != "") {
			int8_t index1 = resp.indexOf(F("\""))+1;
			int8_t index2 = resp.lastIndexOf(F("\""));
			int8_t index3 = resp.indexOf(F(","))+1;
			gsm.debug->print(F("\""));								//##### Serial
			gsm.debug->print(resp.substring(index1,index2)); 		//##### Serial
			gsm.debug->print(F("\", ")); 							//##### Serial
			gsm.debug->print(resp.substring(index3));				//##### Serial
			gsm.debug->println(F(" bytes")); 						//##### Serial
		}
	}
	while(resp != "");
}

long UC20_file::filesize(String fname) {
	gsm.print(F("AT+QFLST=\""));
	gsm.print(fname);
	gsm.println(F("\""));
	String modelcheck = "+QFLST: \"" + fname;
	String resp = gsm.waitStr(modelcheck);
	if(resp != "") {
		int8_t index = resp.indexOf(F(","))+1;
		gsm.waitStr(F("OK"));
		return resp.substring(index).toInt();
	}
	return -1;
}

bool UC20_file::del(String fname) {
	gsm.print(F("AT+QFDEL=\""));
	gsm.print(fname);
	gsm.println(F("\""));
	return (gsm.waitStr(F("OK")) != "");
}

bool UC20_file::mov(String srcPath, String destPath, bool copy, bool overwrite) {
	gsm.print(F("AT+QFMOV=\""));
	gsm.print(srcPath);
	gsm.print(F("\",\""));
	gsm.print(destPath);
	gsm.print(F("\","));
	gsm.print(copy, DEC);
	gsm.print(F(","));
	gsm.println(overwrite, DEC);
	return (gsm.waitStr(F("OK")) != "");
}

int UC20_file::open(String fname, uint8_t mode) {
	if(mode > 2) 
		return -1;
	if(_handle != -1) 
		close();
	else {
		int prev_h = opened(fname);
		if(prev_h != -1) 
			close(prev_h);
	}
	gsm.print(F("AT+QFOPEN=\""));
	gsm.print(fname);
	gsm.print(F("\","));
	gsm.println(mode);
	String resp = gsm.waitStr(F("+QFOPEN:"));
	if(resp != "") {
		int8_t index = resp.indexOf(F(" "))+1;
		int handle = resp.substring(index).toInt();
		gsm.waitStr(F("OK"));
		_handle = handle;
		_userid = handle+1000;
		return handle;
	}
	else 
		return -1;
}

int UC20_file::opened(String fname) {
	gsm.println(F("AT+QFOPEN?"));
	String resp = "";
	do {
		resp = gsm.waitStr(F("+QFOPEN:"),1000);
		if(resp != "") {
			int8_t index1 = resp.indexOf(F("\""))+1;
			int8_t index2 = resp.lastIndexOf(F("\""));
			int8_t index3 = resp.indexOf(F(","))+1;
			String getname = resp.substring(index1,index2);
			if(getname == fname) {
				gsm.waitStr(F("OK"));
				return resp.substring(index3).toInt();
			}
		}
	}
	while(resp != "");
	return -1;
}

bool UC20_file::close(int handle) {
	gsm.print(F("AT+QFCLOSE="));
	if(handle == -1) 
		gsm.println(_handle);
	else 
		gsm.println(handle);
	_handle = -1;
	_userid = -1;
	return (gsm.waitStr(F("OK")) != "");
}

int UC20_file::read() {
	if(_handle == -1) 
		return -1;
	if(!(gsm.available_buf() > 0) || (gsm.getUser_buf() != _userid)) {
		gsm.print(F("AT+QFREAD="));
		gsm.print(_handle);
		gsm.print(F(","));
		gsm.println(BUFFER_SIZE);
		String resp = gsm.waitStr(F("CONNECT"));
		if(resp != "") {
			int8_t index = resp.indexOf(F(" "))+1;
			unsigned int data_size = resp.substring(index).toInt();
			gsm.readKeepFor(data_size,_userid);	// keep data to buf
			gsm.waitStr(F("OK"),1000);
		}
	}
	return gsm.read_buf();
}

String UC20_file::readString() {
	if(_handle == -1) 
		return "";
	if(!(gsm.available_buf() > 0) || (gsm.getUser_buf() != _userid)) {
		gsm.print(F("AT+QFREAD="));
		gsm.print(_handle);
		gsm.print(F(","));
		gsm.println(BUFFER_SIZE);
		String resp = gsm.waitStr(F("CONNECT"));
		if(resp != "") {
			int8_t index = resp.indexOf(F(" "))+1;
			unsigned int data_size = resp.substring(index).toInt();
			gsm.readKeepFor(data_size,_userid);		// keep data to buf
			gsm.waitStr(F("OK"),1000);
		}
		else
			return "";
	}
	return gsm.readString_buf();
}

int UC20_file::readbufAvailable() {
	if(gsm.getUser_buf() == _userid) 
		return gsm.available_buf();
	else 
		return 0;
}

bool UC20_file::beginWrite(long len ) {
	if(_handle == -1) 
		return false;
	gsm.print(F("AT+QFWRITE="));
	gsm.print(_handle);
	if(len != -1) {
		gsm.print(F(","));
		gsm.print(len);
	}
	gsm.println();
	return (gsm.waitStr(F("CONNECT")) != "");
}

bool UC20_file::endWrite() {
	if(gsm.waitStr(F("+QFWRITE:"),6000) == "") 
		return false;
	unsigned long prev_t = millis();
	do {
		String resp = gsm.readStringUntil('\n');
		if(resp.indexOf(F("+CME ERROR: 421")) != -1) 	// input size is less than detemined len
			break;
		else if(resp.indexOf(F("OK")) != -1) 			// input success
			break;
	}
	while(millis()-prev_t < 1000);
	return true;
}

long UC20_file::getPointerPos() {
	if(_handle == -1) 
		return -1;
	gsm.print(F("AT+QFPOSITION="));
	gsm.println(_handle);
	String resp = gsm.waitStr(F("+QFPOSITION:"));
	if(resp != "") {
		int8_t index = resp.indexOf(F(" "))+1;
		long position = resp.substring(index).toInt();
		gsm.waitStr(F("OK"));
		return position;
	}
	else 
		return -1;
}

long UC20_file::movPointer(long pos) {
	if(_handle == -1) 
		return -1;
	gsm.print(F("AT+QFSEEK="));
	gsm.print(_handle);
	gsm.print(F(","));
	gsm.println(pos);
	if(gsm.waitStr(F("OK"),1000) != "") 
		return getPointerPos();
	else 
		return -1;
}

long UC20_file::movPointerRight(long offset) {
	if(_handle == -1) 
		return -1;
	gsm.print(F("AT+QFSEEK="));
	gsm.print(_handle);
	gsm.print(F(","));
	gsm.print(offset);
	gsm.println(F(",1"));
	if(gsm.waitStr(F("OK"),1000) != "") 
		return getPointerPos();
	else 
		return -1;
}

long UC20_file::movPointerEnd() {
	if(_handle == -1) 
		return -1;
	gsm.print(F("AT+QFSEEK="));
	gsm.print(_handle);
	gsm.println(F(",0,2"));
	if(gsm.waitStr(F("OK"),1000) != "") 
		return getPointerPos();
	else 
		return -1;
}









