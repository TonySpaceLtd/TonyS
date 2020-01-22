#include "UC20_Firebase.h"

void UC20_Firebase::begin(String host, String auth) {
	String modhost = host;
	modhost.toLowerCase();
	int index1 = modhost.indexOf(F("https://"));
	int index2 = modhost.indexOf(F("/"),index1+8);
	if(index2 == -1) {
		fb_host = host.substring(index1+8);
		fb_path = "";
	}
	else {
		fb_host = host.substring(index1+8,index2);
		fb_path = host.substring(index2);
		if(fb_path.endsWith("/")) 
			fb_path.remove(fb_path.lastIndexOf(F("/")));
	}
	fb_auth = auth;
	_client.initConfig();
	_client.responseHeader(false);
	char c_host[host.length()+1];
	for(int8_t i=0; i<host.length(); i++) {
		c_host[i]   = host[i];
		c_host[i+1] = 0;
	}
	_client.url(c_host);
}

bool UC20_Firebase::sendHTTP(String method, String thing, String data) {
	if(!thing.startsWith("/")) 
		thing = "/"+thing;
	unsigned int data_len = data.length();
	unsigned int packet_len = 54;
	packet_len += method.length();
	packet_len += fb_path.length();
	packet_len += thing.length();
	packet_len += fb_auth.length();
	packet_len += fb_host.length();
	if(method != "GET") {
		packet_len += 18;
		packet_len += String(data_len).length();
	}
	packet_len += data_len;
	
	if(_client.post(packet_len, CUSTOM_HEADER)) {
		gsm.print(method);
		gsm.print(F(" "));						// 1
		gsm.print(fb_path);						// +
		gsm.print(thing);						// +
		gsm.print(F(".json?auth="));			// 11
		gsm.print(fb_auth);						// +
		gsm.println(F(" HTTP/1.1"));			// 11
		gsm.print(F("Host: "));					// 6
		gsm.println(fb_host);					// 2+
		gsm.println(F("User-Agent: UC20_3G"));	// 21+
		if(method != "GET") {
			gsm.print(F("Content-Length: "));	// 16
			gsm.println(data_len);				// 2+
		}
		gsm.println();							// 2
		gsm.print(data);						// +
		gsm.waitStr(F("OK"));
		return (_client.wait_responseCode() == 200);
	}
	else 
		return false;
}

bool UC20_Firebase::set(String thing, String JsObj) {
	return sendHTTP("PUT",thing,JsObj);
}

bool UC20_Firebase::setInt(String thing, long data) {
	return sendHTTP("PUT",thing,(String)data);
}

bool UC20_Firebase::setFloat(String thing, double data) {
	return sendHTTP("PUT",thing,(String)data);
}

bool UC20_Firebase::setBool(String thing, bool data) {
	if(data) 
		return sendHTTP("PUT",thing,"true");
	else 
		return sendHTTP("PUT",thing,"false");
}

bool UC20_Firebase::setString(String thing, String data) {
	String strdata = "\"" + data + "\"";
	return sendHTTP("PUT",thing,strdata);
}

String UC20_Firebase::get(String thing) {
	sendHTTP("GET",thing);
	return _client.readString();
}

long UC20_Firebase::getInt(String thing) {
	sendHTTP("GET",thing);
	String data = _client.readString();
	return data.toInt();
}

double UC20_Firebase::getFloat(String thing) {
	sendHTTP("GET",thing);
	String data = _client.readString();
	return data.toFloat();
}

bool UC20_Firebase::getBool(String thing) {
	sendHTTP("GET",thing);
	String data = _client.readString();
	if(data == "true") 
		return true;
	else 
		return false;
}

String UC20_Firebase::getString(String thing) {
	sendHTTP("GET",thing);
	String data = _client.readString();
	int index1 = data.indexOf(F("\""));
	int index2 = data.indexOf(F("\""),index1+1);
	if((index1!=-1)&&(index2!=-1)) 
		return data.substring(index1+1,index2);
	else 
		return "";
}
		
String UC20_Firebase::push(String thing, String JsObj) {
	bool success = sendHTTP("POST",thing,JsObj);
	if(success) {
		String data = _client.readString();
		if(data.indexOf(F("name")) != -1) {
			int index1 = data.indexOf(F(":"))+2;
			int index2 = data.lastIndexOf(F("\""));
			return data.substring(index1,index2);	// return path
		}
	}
	return "";
}

String UC20_Firebase::pushInt(String thing, long data) {
	bool success = sendHTTP("POST",thing,(String)data);
	if(success) {
		String data = _client.readString();
		if(data.indexOf(F("name")) != -1) {
			int index1 = data.indexOf(F(":"))+2;
			int index2 = data.lastIndexOf(F("\""));
			return data.substring(index1,index2);	// return path
		}
	}
	return "";
}

String UC20_Firebase::pushFloat(String thing, double data) {
	bool success = sendHTTP("POST",thing,(String)data);
	if(success) {
		String data = _client.readString();
		if(data.indexOf(F("name")) != -1) {
			int index1 = data.indexOf(F(":"))+2;
			int index2 = data.lastIndexOf(F("\""));
			return data.substring(index1,index2);	// return path
		}
	}
	return "";
}

String UC20_Firebase::pushBool(String thing, bool data) {
	bool success;
	if(data) 
		success = sendHTTP("POST",thing,"true");
	else 
		success = sendHTTP("POST",thing,"false");
	if(success) {
		String data = _client.readString();
		if(data.indexOf(F("name")) != -1) {
			int index1 = data.indexOf(F(":"))+2;
			int index2 = data.lastIndexOf(F("\""));
			return data.substring(index1,index2);	// return path
		}
	}
	return "";
}

String UC20_Firebase::pushString(String thing, String data) {
	String strdata = "\"" + data + "\"";
	bool success = sendHTTP("POST",thing,strdata);
	if(success) {
		String data = _client.readString();
		if(data.indexOf(F("name")) != -1) {
			int index1 = data.indexOf(F(":"))+2;
			int index2 = data.lastIndexOf(F("\""));
			return data.substring(index1,index2);	// return path
		}
	}
	return "";
}

bool UC20_Firebase::remove(String thing) {
	return sendHTTP("DELETE",thing,"0");
}









