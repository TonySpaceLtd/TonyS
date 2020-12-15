#ifndef ECXX_MQTT_H
#define ECXX_MQTT_H

#include "Arduino.h"
#include <Stream.h>
#include "TonyS_X1.h"
#include "EC25_lib.h"


class EC25_MQTT
{
	public:
		EC25_MQTT();
		void init(uint8_t client_idx=5);
		bool connect(String ip,int port,String deviceID,String user,String password);
		bool disConnect();
		bool close();
		bool subScribe(String topic[],uint8_t qos[],uint8_t size);
		bool subScribe(String topic,uint8_t qos=0);
		bool subScribe(String topic[],uint8_t qos,uint8_t size);
		
		bool unSubScribe(String topic[],uint8_t size);
		bool unSubScribe(String topic);
		void task();
		uint8_t connected();
		void (*callback)(String topic ,uint8_t *payload,size_t length);
		bool publish(String topic,uint8_t *payload,size_t msglen,uint8_t msgID=0,uint8_t qos=0,uint8_t retain=0);
		bool publish(String topic,String payload,uint8_t msgID=0,uint8_t qos=0,uint8_t retain=0);
		

	private:
		uint8_t _client_idx; // set 0-5
		uint8_t _connected;
		
};
















#endif