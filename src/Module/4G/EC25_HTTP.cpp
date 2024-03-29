#include "EC25_HTTP.h"

void http_null_(uint8_t *data,size_t size,bool finish){}
EC25_HTTP::EC25_HTTP()
{
	callback = http_null_;
}
bool EC25_HTTP::setURL(String url,bool respHeader)
{
	String str;
	EC_Resp res;
	str = "AT+QHTTPCFG=\"responseheader\","+String(respHeader);
	LTE.ECser.sendAT(str);
	res = LTE.ECser.waitString("OK",2000);
	if(res.id==0)
	{
		str = "AT+QHTTPURL="+String(url.length())+",80";
		LTE.ECser.sendAT(str);
		res = LTE.ECser.waitString("CONNECT",2000);
		if(res.id==0)
		{
			LTE.ECser.print(url);
			res = LTE.ECser.waitString("OK",2000);
			if(res.id==0)
				return true;
		}
		
	}

	return false;
}
int EC25_HTTP::get()
{
	String storage_str[3]={"UFS","RAM","SD"};
	String str;
	EC_Resp res;
	str = "AT+QHTTPGET=80";
	LTE.ECser.sendAT(str);
	res = LTE.ECser.waitString("OK",80000);
	if(res.id==0)
	{
		res = LTE.ECser.waitString("+QHTTPGET",8000);
		if(res.id==0)
		{
			int index = res.data.indexOf(",");
			int index2 = res.data.indexOf(",",index+1);
			if(index2==-1)
				index2=res.data.length();
			int resp = res.data.substring(index+1,index2).toInt();
			if(resp==200)
			{
				//Serial.print("File size: ");
				//Serial.println(res.data.substring(index2+1));
				httpFile.del(HTTP_STORAGE,HTTP_PATH);
				str = "AT+QHTTPREADFILE=\""+ String(storage_str[HTTP_STORAGE])+ ":"+ String(HTTP_PATH)+"\",80";
				LTE.ECser.sendAT(str);
				res = LTE.ECser.waitString("OK",2000);
				if(res.id==0)
				{
					readTail = 0;
					res = LTE.ECser.waitString("+QHTTPREADFILE: 0",(1000*60)*3);
					if(res.id==0)
						return resp;
				}
				else
				{
					Serial.println("Noresp!!");
				}
			}
			return resp;
		}
	}
	return 0;
}
size_t EC25_HTTP::read(uint8_t *data,size_t buffSize)
{
	if(readTail >= httpFile.fileSize(HTTP_STORAGE,HTTP_PATH))
		return 0 ;
	httphandle = httpFile.open(HTTP_STORAGE,HTTP_PATH);
	httpFile.seek(httphandle,readTail);
	size_t r = httpFile.read(httphandle,data,buffSize);
	readTail += r; 
	httpFile.close(httphandle);
	return r;
}
size_t EC25_HTTP::respSize()
{
	return httpFile.fileSize(HTTP_STORAGE,HTTP_PATH);
}
bool EC25_HTTP::clearMemory()
{
	return httpFile.del(HTTP_STORAGE,HTTP_PATH);
}

int EC25_HTTP::post(uint8_t *data,size_t len)
{
	String storage_str[3]={"UFS","RAM","SD"};
	String str;
	EC_Resp res;
	str = "AT+QHTTPPOST="+String(len)+",80,80"; //AT+QHTTPPOST=20,80,80
	LTE.ECser.sendAT(str);
	res = LTE.ECser.waitString("CONNECT",80000);
	if(res.id==0)
	{
		LTE.ECser.write(data,len);
		res = LTE.ECser.waitString("OK",80000);
		if(res.id==0)
		{
			
			res = LTE.ECser.waitString("+QHTTPPOST:",80000);//+QHTTPPOST: 0,200,445
			if(res.id==0)
			{
				
				int index = res.data.indexOf(",");
				int index2 = res.data.indexOf(",",index+1);
				if(index2==-1)
					index2=res.data.length();
				int resp = res.data.substring(index+1,index2).toInt();
				if(resp==200)
				{
					
					httpFile.del(HTTP_STORAGE,HTTP_PATH);
					str = "AT+QHTTPREADFILE=\""+ String(storage_str[HTTP_STORAGE])+ ":"+ String(HTTP_PATH)+"\",80";
					LTE.ECser.sendAT(str);
					res = LTE.ECser.waitString("OK",2000);
					if(res.id==0)
					{
						readTail = 0;
						res = LTE.ECser.waitString("+QHTTPREADFILE: 0",(1000*60)*3); 
						if(res.id==0)
							return resp;
					}
					else
					{
						Serial.println("Noresp!!");
					}
				}
				return resp;
			}
			
		}
		

	}
	
	
	
	return(0);
}


