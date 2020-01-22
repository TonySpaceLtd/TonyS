// LINE Sticker list
// https://devdocs.line.me/files/sticker_list.pdf

#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

#define TOKEN "YOUR_LINE_TOKEN"

UC20_LineNotify line;

void setup() {
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);
  gsm.initial(SLOT2);

  Serial.println("######## UC20 with LINE ########");
  Serial.print("Power on module...");
  while(!gsm.powerOn()) 
    Serial.print(".");
  Serial.println(" READY");
  Serial.print  ("Network registration...");
  Serial.println(gsm.getNetworkStatus()? " READY":" NOT READY");
  Serial.println("IMEI     : " + gsm.getIMEI());
  Serial.println("IMSI     : " + gsm.getIMSI());
  Serial.println("ICCID    : " + gsm.getICCID());
  Serial.println("Operator : " + gsm.getOperator());
  Serial.print  ("RSSI     : ");
  Serial.println(gsm.getSignal());
  Serial.println();
  
  line.begin(TOKEN);
  Serial.println("- send");
  line.sticker(2,159);                // ex: packageID=2, stickerID=159
  line.imageurl(F("https://store.kidbright.info/upload/cover-image/1544265083-nDP3ez.png"));
//  line.silentNotify(true);          // uncomment to not push notification to your device
  line.submit("LINE NOTIFY TEST");    // Send
  while(line.available() > 0) {
    Serial.write(line.read());
  }
  Serial.println("\n...");
}

void loop() {
  if(Serial.available() > 0) {
    gsm.write(Serial.read());
  }
  if(gsm.available() > 0) {
    Serial.write(gsm.read());
  }
}
