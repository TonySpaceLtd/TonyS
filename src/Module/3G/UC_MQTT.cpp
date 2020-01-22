#include "UC_MQTT.h"

UC_MQTT::UC_MQTT(int8_t conid, int8_t sslctxid , int8_t ctxid){
	mq_tcp = new UC20_TCP(conid,sslctxid,ctxid);
}

void UC_MQTT::enableSSL() {
	mq_tcp->enableSSL();
}

void UC_MQTT::disableSSL() {
	mq_tcp->disableSSL();
}

bool UC_MQTT::brokerConnect(const char* server, const char* port) {
	return mq_tcp->connect(server,port);
}

bool UC_MQTT::brokerConnect(const char* server, unsigned int port) {
	return mq_tcp->connect(server,port);
}

bool UC_MQTT::brokerConnected() {
	return mq_tcp->connected();
}

bool UC_MQTT::brokerDisconnect() {
	return mq_tcp->disconnect();
}

int8_t UC_MQTT::connect(const char* id, const char* user, const char* pass, const char* willTopic, uint8_t willQos, bool willRetain, const char* willMessage, bool cleanSession) {
	unsigned int id_len, wTopic_len, wMsg_len, user_len, pass_len;
	uint8_t length_packet = 0x00;
	uint8_t header_ver[] = {0x00,0x04,'M','Q','T','T',0x04};
	length_packet += sizeof(header_ver);
	uint8_t conn_flags = 0x00;
	if(willTopic)
		conn_flags |= 0x04|(willQos<<3)|(willRetain<<5);
	if(cleanSession)
		conn_flags |= 0x02;
	if(user != NULL) {
		conn_flags |= 0x80;
		if(pass != NULL) 
			conn_flags |= 0x40;
	}
	length_packet += 1;						// conn_flags length
	length_packet += 2;						// KEEPALIVE length
	id_len = strlen(id);
	length_packet += id_len+2;				// clientID length
	if(willTopic) {
		wTopic_len = strlen(willTopic);
		length_packet += wTopic_len+2;		// willTopic length
		wMsg_len = strlen(willMessage);
		length_packet += wMsg_len+2;		// willMessage length
	}
	if(user != NULL) {
		user_len = strlen(user);
		length_packet += user_len+2;		// username length
		if(pass != NULL) {
			pass_len = strlen(pass);
			length_packet += pass_len+2;	// password length
		}
	}
	if(length_packet < 126) {
		mq_tcp->beginSend(length_packet+2);
		gsm.write(0x10);								// connect
		gsm.write(length_packet);
		gsm.write(header_ver, sizeof(header_ver));
		gsm.write(conn_flags);
		gsm.write((MQTT_KEEPALIVE&0xFF00) >> 8);
		gsm.write(MQTT_KEEPALIVE&0xFF);
		gsm.write((id_len&0xFF00) >> 8);				// clientID
		gsm.write(id_len&0xFF);
		gsm.write(id, id_len);
		if(willTopic) {
			gsm.write((wTopic_len&0xFF00) >> 8);		// willTopic
			gsm.write(wTopic_len&0xFF);
			gsm.write(willTopic, wTopic_len);
			gsm.write((wMsg_len&0xFF00) >> 8);			// willMessage
			gsm.write(wMsg_len&0xFF);
			gsm.write(willMessage, wMsg_len);
		}
		if(user != NULL) {
			gsm.write((user_len&0xFF00) >> 8);		// username
			gsm.write(user_len&0xFF);
			gsm.write(user, user_len);
			if(pass != NULL) {
				gsm.write((pass_len&0xFF00) >> 8);	// password
				gsm.write(pass_len&0xFF);
				gsm.write(pass, pass_len);
			}
		}
		if(gsm.waitStr(F("SEND OK")) != "") {
			if(mq_tcp->waitrecv() != -1) {
				// connack : 20 02 00 00
				mq_tcp->read();
				mq_tcp->read();
				mq_tcp->read();
				// Serial.print(mq_tcp->read(),HEX);
				// Serial.print(" ");
				// Serial.print(mq_tcp->read(),HEX);
				// Serial.print(" ");
				// Serial.print(mq_tcp->read(),HEX);
				// Serial.print(" ");
				return mq_tcp->read()&0xFF;	// 0x00 is success, otherwise is fail
			}
			else 
				return -1;			// no incoming message
		}
		else
			return -1;				// send error
	}
	else
		return -1;					// packet too long
}

int8_t UC_MQTT::connect(const char* id, const char* user, const char* pass) {
	return connect(id,user,pass,0,0,0,0,1);
}

int8_t UC_MQTT::connect(const char* id) {
	return connect(id,NULL,NULL,0,0,0,0,1);
}

bool UC_MQTT::publish(const char* topic, const char* msg, bool retain) {
	unsigned int topic_len, msg_len;
	uint8_t header = 0x30;
	if(retain)
		header |= 0x01;
	uint8_t length_packet = 0x00;
	topic_len = strlen(topic);
	length_packet += 2+topic_len;		// topic_length
	msg_len = strlen(msg);
	length_packet += msg_len;			// message_length
	if(length_packet < 126) {
		mq_tcp->beginSend(length_packet+2);
		gsm.write(header);
		gsm.write(length_packet);
		gsm.write((topic_len&0xFF00) >> 8);
		gsm.write(topic_len&0xFF);
		gsm.write(topic, topic_len);
		gsm.write(msg, msg_len);
		return (gsm.waitStr(F("SEND OK")) != "");
	}
	else 
		return false;
}

bool UC_MQTT::publish(String topic, String msg, bool retain) {
	int length = topic.length() + msg.length() + 4;
	if(length < 128) {
		char cTop[topic.length()+1];
		char cMsg[msg.length()+1];
		for(uint8_t i=0; i<topic.length(); i++) {
			cTop[i]   = topic[i];
			cTop[i+1] = '\0';
		}
		for(uint8_t i=0; i<msg.length(); i++) {
			cMsg[i]   = msg[i];
			cMsg[i+1] = '\0';
		}
		return publish(cTop, cMsg, retain);
	}
	else
		return false;
}

bool UC_MQTT::subscribe(const char* topic, unsigned int packetID, uint8_t QoS) {
	unsigned int topic_len;
	uint8_t length_packet = 0x00;
	length_packet += 2;					// packetid_length
	topic_len = strlen(topic);
	length_packet += topic_len+2;		// topic_length
	length_packet += 1;					// QoS_length
	if(length_packet < 126) {
		mq_tcp->beginSend(length_packet+2);
		gsm.write(0x82);
		gsm.write(length_packet);
		gsm.write((packetID&0xFF00) >> 8);
		gsm.write(packetID&0xFF);
		gsm.write((topic_len&0xFF00) >> 8);
		gsm.write(topic_len&0xFF);
		gsm.write(topic, topic_len);
		gsm.write(QoS);
		if(gsm.waitStr(F("SEND OK")) != "") {
			if(mq_tcp->waitrecv() != -1) {
				// suback : 90 03 00xx 00
				mq_tcp->read();
				mq_tcp->read();
				mq_tcp->read();
				mq_tcp->read();
				// Serial.print(mq_tcp->read(),HEX);
				// Serial.print(" ");
				// Serial.print(mq_tcp->read(),HEX);
				// Serial.print(" ");
				// Serial.print(mq_tcp->read(),HEX);
				// Serial.print(" ");
				// Serial.print(mq_tcp->read(),HEX);
				// Serial.print(" ?? ");
				if(mq_tcp->read() != 0x80) 
					return true;
				else 
					return false;		// return code : FAIL
			}
		}
	}
	return false;					// packet too long
}

bool UC_MQTT::unsubscribe(const char* topic, unsigned int packetID) {
	unsigned int topic_len;
	uint8_t length_packet = 0x00;
	length_packet += 2;						// packetid_length
	topic_len = strlen(topic);
	length_packet += topic_len+2;			// topic_length
	if(length_packet < 126) {
		mq_tcp->beginSend(length_packet+2);
		gsm.write(0xa2);
		gsm.write(length_packet);
		gsm.write((packetID&0xFF00) >> 8);
		gsm.write(packetID&0xFF);
		gsm.write((topic_len&0xFF00) >> 8);
		gsm.write(topic_len&0xFF);
		gsm.write(topic, topic_len);
		if(gsm.waitStr(F("SEND OK")) != "") {
			if(mq_tcp->waitrecv() != -1) {
				// unsuback : b0 02 00xx
				mq_tcp->read();
				mq_tcp->read();
				mq_tcp->read();
				mq_tcp->read();
				return true;
			}
		}
	}
	return false;
}

bool UC_MQTT::pingreq() {
	mq_tcp->beginSend(2);
	gsm.write(0xc0);
	gsm.write(0x00);
	if(gsm.waitStr(F("SEND OK")) != "") {
		if(mq_tcp->waitrecv() != -1) {
			// pingresp : d0 00
			mq_tcp->read();
			mq_tcp->read();
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool UC_MQTT::disconnect() {
	mq_tcp->beginSend(2);
	gsm.write(0xe0);
	gsm.write(0x00);
	return (gsm.waitStr(F("SEND OK")) != "");
}

bool UC_MQTT::recvPublish() {
	if(gsm.available() > 0) {
		if(mq_tcp->waitrecv(1) != -1) {
			uint8_t fixed_h = mq_tcp->read();
			uint8_t len_remain = mq_tcp->read();
			if((fixed_h&0xF0) == 0x30) {				// receive publish packet
				_topic = "";
				_message = "";
				mq_tcp->read();							// len MSB
				uint8_t t_len = mq_tcp->read();			// len LSB
				for(uint8_t i=0; i<t_len; i++) {
					_topic += char(mq_tcp->read());		// get topic
				}
				if((fixed_h&0x06) != 0x00) {			// Qos > 0
					mq_tcp->read();
					mq_tcp->read();
				}
				while(mq_tcp->available() > 0) {
					_message += char(mq_tcp->read());	// get message
				}
				return true;
			}
		}
	}
	return false;
}

String UC_MQTT::getTopic() {
	return _topic;
}

String UC_MQTT::getMessage() {
	return _message;
}









