#ifndef ECXX_LINE_NOTIF_H
#define ECXX_LINE_NOTIF_H

#include "Arduino.h"
#include <Stream.h>
#include "TonyS_X1.h"
#include "EC25_lib.h"
#include "EC25_SSL.h"

#define LINE_SOCKET_ID 10
class EC25_LINE_NOTIFY
{
	public:
	EC25_LINE_NOTIFY();
	void setSocketID(uint8_t socketID);
	bool send(String token,String msg,uint8_t stickerPacket=0,uint8_t stickerID=0);
	String response;
	
	private:
	uint8_t _clientID;	
	EC25_SSL *line_ssl;
	
	
};








































#endif