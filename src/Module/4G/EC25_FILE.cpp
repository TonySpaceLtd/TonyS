#include "EC25_FILE.h"

const String storage_str[3]={"UFS","RAM","SD"};

EC25_FILE::EC25_FILE()
{
	//LTE.ECser.print(url);
}


size_t EC25_FILE::getTotalSpace(uint8_t storage)
{
	return spaceInfo(storage,false);
}
size_t EC25_FILE::getFreeSpace(uint8_t storage)
{
	return spaceInfo(storage,true);
}
size_t EC25_FILE::spaceInfo(uint8_t storage,bool mode) // false = Totalspace,true = free spaceInfo
{
	String str;
	EC_Resp res;
	str = "AT+QFLDS=\""+storage_str[storage]+"\"";
	LTE.ECser.sendAT(str);
	res = LTE.ECser.waitString("+QFLDS:",5000);
	if(res.id==0)
	{
		int indx1 = res.data.indexOf(" ");
		int indx2 = res.data.indexOf(",");
		LTE.ECser.waitString("OK",5000);
		if(mode) // free space
			return res.data.substring(indx1+1,indx2).toInt();
		else // total space
			return res.data.substring(indx2+1).toInt();
	}
	return 0;
}
String EC25_FILE::getfileInfo(uint8_t storage,String path)
{
	//AT+QFLST="RAM:http.temp"
	String str;
	EC_Resp res;
	str = "AT+QFLST=\"" + storage_str[storage] + ":" + path + "\"";
	LTE.ECser.sendAT(str);
	String respStr[3] = {"OK","+QFLST:","+CME ERROR: 417"};
	str = "";
	do
	{
		res = LTE.ECser.waitString(respStr,3,5000);
		if(res.id==1)
		{
			int indx1 = res.data.indexOf("\"");
			int indx2 = res.data.indexOf("\"",indx1+1);
			int indx3 = res.data.indexOf(",");
			str += res.data.substring(indx1+1,indx2);
			str += ",";
			str += res.data.substring(indx3+1);
			str += "\r\n";
			//Serial.println(res.data);
		}
		if(res.id==2)
			return "File not found";
	}
	while(res.id!=0);
	return str;
	
}
size_t EC25_FILE::fileSize(uint8_t storage,String path)
{
	String str = getfileInfo(storage, path);
	int indx = str.indexOf(",");
	if(indx!=-1)
	{
		return str.substring(indx+1).toInt();
	}
	return 0;
}
bool EC25_FILE::del(uint8_t storage,String path)
{
	//AT+QFDEL="RAM:*"
	String str;
	EC_Resp res;
	str = "AT+QFDEL=\"" + storage_str[storage] + ":" + path + "\"";
	LTE.ECser.sendAT(str);
	res = LTE.ECser.waitString("+QFLDS:",5000);
	if(res.id==0)
	{
		return true;
	}
	return false;
}
int EC25_FILE::open(uint8_t storage,String path,uint8_t mode)
{
	int handle=0;
	String str;
	EC_Resp res;
	str = "AT+QFOPEN=\"" + storage_str[storage] + ":" + path + "\","+String(mode);
	LTE.ECser.sendAT(str);
	res = LTE.ECser.waitString("+QFOPEN:",2000);
		if(res.id==0)
		{
			int index = res.data.indexOf(" ");
			handle = res.data.substring(index+1).toInt(); 
		}
	LTE.ECser.waitString("OK",2000);
	return handle;
}
size_t EC25_FILE::read(int handle,uint8_t *buf,size_t length)
{
	String str;
	EC_Resp res;
	size_t s=0;
	str = "AT+QFREAD="+String(handle)+","+String(length);
	LTE.ECser.sendAT(str);
	res = LTE.ECser.waitString("CONNECT",2000);
	if(res.id==0)
	{
		int indx = res.data.indexOf(" ");
		s	= res.data.substring(indx).toInt();
		LTE.ECser.read(); //clear \r\n 
		LTE.ECser.readBytes(buf,s);
	}
	LTE.ECser.waitString("OK",2000);
	return s;
}
bool EC25_FILE::close(int handle)
{
	String str;
	EC_Resp res;
	str = "AT+QFCLOSE="+String(handle);
	LTE.ECser.sendAT(str);
	res = LTE.ECser.waitString("OK",2000);
	if(res.id==0)
	{
		return true;
	}
	return false;
}
bool EC25_FILE::seek(int handle,size_t offset,uint8_t position)
{
	String str;
	EC_Resp res;
	str = "AT+QFSEEK="+String(handle)+","+String(offset)+","+String(position);
	LTE.ECser.sendAT(str);
	res = LTE.ECser.waitString("OK",2000);
	if(res.id==0)
	{
		return true;
	}
	return false;
}



















