#ifndef TonySpace_NBIoT_MQTT_h
#define TonySpace_NBIoT_MQTT_h

#include "Arduino.h"
#include <Stream.h>
#include "TonyS_X1.h"
#include "TonySpace_NBIoT.h"

#define REQ_LENGTH 1358

class NBIoT_MQTT : public Stream 
{
	public:
	NBIoT_MQTT();
	bool open(String serverIP,int port);
	bool connect(String clientID,String user="",String password="");
	bool subScribe(String topic,uint8_t qos=0);
	bool unSubScribe(String topic);
	bool publish(String topic,uint8_t *data,size_t length,uint8_t qos=0,uint8_t retain=0);
	bool publish(String topic,String data,uint8_t qos=0,uint8_t retain=0);
	void eventTask();
	void qmtrecv();
	void qmtstat();
	bool connected();
	void (*callback)(String topic ,char *payload,size_t length);
	//Stream Class 
		virtual int read(){};	
		virtual size_t write(uint8_t c){};
		virtual int available(){};
		virtual int peek(){}; 
		virtual void flush(){};
		using Stream::write; // make the other overloads of write visible

	private:
		char *reciveBuf;
		size_t rxCnt;
		bool status;
};






























#endif






