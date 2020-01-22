#ifndef UC_MQTT_H
#define UC_MQTT_H

#include <Arduino.h>
#include "UC20_lib.h"
#include "UC20_TCP.h"

#define MQTT_KEEPALIVE 60

class UC_MQTT {
	public:
		UC_MQTT(int8_t conid, int8_t sslctxid = 1, int8_t ctxid = 1);
		void enableSSL();
		void disableSSL();
		bool brokerConnect(const char* server, const char* port);
		bool brokerConnect(const char* server, unsigned int port);
		bool brokerConnected();
		bool brokerDisconnect();
		int8_t connect(const char* id, const char* user, const char* pass, const char* willTopic, uint8_t willQos, bool willRetain, const char* willMessage, bool cleanSession);
		int8_t connect(const char* id, const char* user, const char* pass);
		int8_t connect(const char* id);
		bool publish(const char* topic, const char* msg, bool retain = false);
		bool publish(String topic, String msg, bool retain = false);
		bool subscribe(const char* topic, unsigned int packetID = 0x0a, uint8_t QoS = 0x00);
		bool unsubscribe(const char* topic, unsigned int packetID = 0x0b);
		bool pingreq();
		bool disconnect();
		bool recvPublish();
		String getTopic();
		String getMessage();
		 
	private:
		UC20_TCP* mq_tcp;
		String _topic   = "";
		String _message = "";
};

#endif
