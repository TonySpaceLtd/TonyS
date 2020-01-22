#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

#define DEST_NO "PHONE_NUMBER"  // call destination (08xxxxxxxx)

UC20_call cell;

void setup() {
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);
  gsm.initial(SLOT2);

  Serial.println("########## UC20 CALL ##########");
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
  Serial.println("Type 'c' to call the determine phone number.");
  Serial.println("     'a' to answer incoming call.");
  Serial.println("     'd' to terminate call.");
  Serial.println();
}

void loop() {
  if(cell.listen() == RING) {
    Serial.print("# ");
    Serial.print(cell.currentCallME());
    Serial.println(" is calling YOU");
  }
  else if(Serial.available() > 0) {
    char c = Serial.read();
    switch(c) {
      case 'c':
        Serial.print("calling to ");
        Serial.println(DEST_NO);
        cell.call(DEST_NO);
        break;
      case 'a':
        Serial.println("answer");
        cell.answer();
        break;
      case 'd':
        Serial.println("disconnect");
        cell.disconnect();
        break;
    }
  }
}
