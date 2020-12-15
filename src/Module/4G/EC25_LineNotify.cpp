#include "EC25_LineNotify.h"
  
 
 
EC25_LINE_NOTIFY::EC25_LINE_NOTIFY()
{
	_clientID = LINE_SOCKET_ID;
}
void EC25_LINE_NOTIFY::setSocketID(uint8_t socketID)
{
	_clientID = socketID;
}
bool EC25_LINE_NOTIFY::send(String token,String msg,uint8_t stickerPacket,uint8_t stickerID)
{
	
	line_ssl = new EC25_SSL(_clientID);
	bool ret=false;
	response="";
	if(line_ssl->connect("notify-api.line.me",443,CON_MODE_BUFFER))
	{
		if(line_ssl->startSend())
		{
			String str;
			LTE.ECser.println(F("POST /api/notify HTTP/1.1"));
			str = "Authorization: Bearer " + token;
			LTE.ECser.println(str);
			LTE.ECser.println(F("Content-Type: application/x-www-form-urlencoded"));
			LTE.ECser.println(F("Cache-Control: no-cache"));
			LTE.ECser.println(F("Host: notify-api.line.me"));
			
			String _msg = "message="+msg;
			String _pgid = "&stickerPackageId="+String(stickerPacket);
			String _id = "&stickerId="+String(stickerID);
			
			size_t len = _msg.length();
			if(stickerPacket>0&&stickerID>0)
			{
				len += _pgid.length()+_id.length();
			}
			LTE.ECser.print(F("Content-Length: "));
			LTE.ECser.println(len);
			//Serial.println(len);
			LTE.ECser.println();
			LTE.ECser.print(_msg+_pgid+_id);
			if(line_ssl->stopSend())
			{
				if(line_ssl->waitData(10000));
				{
					uint8_t buffer[500];
					size_t len = line_ssl->readBytes(buffer,500);
					bool flag = false;
					response = "";
					for(size_t i=0;i<len;i++)
					{
						char c = buffer[i];
						if(c=='{')
							flag = true;
						if(flag)
							response += c;
						if(c=='}')
							break;
					}	
					if(response.indexOf("\"status\":200")!=-1)
						ret = true;
					//Serial.write(buffer,len);
				}	
			}
		}
	}
	else
		response = "Connect Server: Fail";
	line_ssl->close();
	return ret;
}
































