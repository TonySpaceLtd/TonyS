#include "EC25_OTA.h"

EC25_OTA::EC25_OTA()
{
	
}
bool EC25_OTA::updateFirmware(String url)
{
	Serial.println("########## OTA Process ##########");
	Serial.print("DownloadFirmware: ");
	int ret = httpOAT.setURL(url);
	if(ret==false)
	{
		Serial.println("Fail");
		return false;
	}
	Serial.println("Loading...");
	ret = httpOAT.get();
	Serial.print("DownloadFirmware: ");
	if(ret==200)
	{
		Serial.println("Finish");
		Serial.print("Firware Size: ");
		size_t updateSize = httpOAT.respSize();
		Serial.print(updateSize);
		Serial.println(" Bytes");
		bool rturn = updateFlash(updateSize);
		Serial.println("####################");
		return(rturn);
	}
	else
		Serial.println("Fail");
	return false;
}
bool EC25_OTA::updateFlash(size_t updateSize)
{
  if (Update.begin(updateSize)) 
  {      
    uint8_t buff[OTA_BUFFER_SIZE];
    size_t s,s2=0;
	uint8_t  pv=0xFF,sv;
    while(s = httpOAT.read(buff,OTA_BUFFER_SIZE))
    {
        sv = map(s2,0,updateSize,0,100);
		if(pv!=sv)
		{
			Serial.print("Process: ");
			Serial.print(sv);
			Serial.println(" %");
			pv=sv;
		}
        s2 += Update.write(buff,s);
    } 
    //Serial.println("\r\nFinish!!!");
	if (Update.end()) 
	{
			Serial.println("OTA done!");
			if (Update.isFinished()) 
			{
				Serial.println("Update successfully completed.");
				httpOAT.clearMemory();
				return true;
				// ESP.restart();
			}
			else 
			{
				Serial.println("Update not finished? Something went wrong!");
			}
    }
    else 
	{
         Serial.println("Error Occurred. Error #: " + String(Update.getError()));
	}

   }
   else
   {
      Serial.println("Not enough space to begin OTA");
   }
   return false;
}





