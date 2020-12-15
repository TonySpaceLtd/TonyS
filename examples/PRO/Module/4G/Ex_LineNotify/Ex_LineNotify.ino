#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"
#define TOKEN "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
EC25_LINE_NOTIFY line;

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
  Serial.println(String(LTE.getSignaldBm()) + " dBm");
  Serial.print("Connect Internet: ");
  Serial.println(LTE.connectInternet() ? "OK" : "Fail");
  Serial.print("IP Address: ");
  Serial.println(LTE.getIPAddress());
  Serial.print("Send LineNotify: ");
  Serial.println(line.send(TOKEN,"Hello Line Notify",1,2)?"OK":"\r\nFail-->"+line.response);
  //line.send(TOKEN,"Hello Line Notify",2,2)      //arg1: token
                                                  //arg2: message
                                                  //arg3: StickerPacket default = 0 (not send sticker) 
                                                  //arg4: StickerID     default = 0 (not send sticker)
}
void loop() 
{
  if(Serial.available())
  {
    String str = Serial.readStringUntil('\n');
    Serial.print("Send LineNotify: "+str);
    Serial.println(line.send(TOKEN,str)?" OK":"\r\nFail-->"+line.response);
  }
}
