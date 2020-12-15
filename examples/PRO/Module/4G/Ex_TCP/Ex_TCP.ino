#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"
#define SERVER  "xxx.xxx.xxx.xxx"
#define PORT    1234
#define CONTEXTID 1
EC25_TCP myClient(CONTEXTID);

void setup() {
  Serial.begin(115200);
  Serial.println("Hello World");
  Tony.begin();
  delay(100);
  LTE.initial(SLOT3);
  Serial.println("Power On 4G Module Wait Ready...");
  LTE.powerOn(true);
  LTE.waitModuleReady(20);
  Serial.print("IMEI: ");
  Serial.println(LTE.getIMEI());
  Serial.print("CIMI: ");
  Serial.println(LTE.getCIMI());
  Serial.print("Network :");
  while (!LTE.getNetworkRegis())
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Ready");
  Serial.print("Access Tech: ");
  Serial.println(LTE.getAccessTech());
  Serial.print("Operator: ");
  Serial.println(LTE.getOperator());
  Serial.print("Signal: ");
  Serial.println(String(LTE.getSignaldBm())+" dBm");
  Serial.print("Connect Internet: ");
  Serial.println(LTE.connectInternet() ? "OK" : "Fail");
  Serial.print("IP Address: ");
  Serial.println(LTE.getIPAddress());
  connectServer();
}

void loop() {
  if(myClient.connected())
  {
    if(myClient.available())
    {
      int dataSize = myClient.available();
      uint8_t buff[dataSize];
      myClient.readBytes(buff,dataSize);
      Serial.println("Receive: ");
      Serial.write(buff,dataSize); 
      Serial.println();
    }
    if(Serial.available())
    {
      String str = Serial.readStringUntil('\n');
      Serial.println("Send: "+str);
      if(myClient.writeBytes((uint8_t*)str.c_str(),str.length()))
        Serial.println("OK");
      else
        Serial.println("Fail");
       myClient.println("println:"+str);
    }
  }
  else
    connectServer();
}
void connectServer()
{
  Serial.print("\r\nConnect Server: ");
  myClient.close();
  if(myClient.connect(SERVER,PORT))
    Serial.println("OK");
  else
    Serial.println("Fail");  
}
