#ifndef UC20_FIREBASE_H
#define UC20_FIREBASE_H

#include <Arduino.h>
#include "UC20_lib.h"
#include "UC20_HTTP.h"

class UC20_Firebase {
	public:
		void begin(String host, String auth);
		bool set(String thing, String JsObj);
		bool setInt(String thing, long data);
		bool setFloat(String thing, double data);
		bool setBool(String thing, bool data);
		bool setString(String thing, String data);
		
		String get(String thing);
		long   getInt(String thing);
		double getFloat(String thing);
		bool   getBool(String thing);
		String getString(String thing);
		
		String push(String thing, String JsObj);
		String pushInt(String thing, long data);
		String pushFloat(String thing, double data);
		String pushBool(String thing, bool data);
		String pushString(String thing, String data);
		
		bool remove(String thing);
		
	private:
		bool sendHTTP(String method, String thing, String data = "");
		UC20_HTTP _client;
		String fb_host;
		String fb_path;
		String fb_auth;
};

#endif
