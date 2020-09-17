#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

#define powerPin 4

void setup() {
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);
  gsm.initial(SLOT2);

  Serial.println("BEGIN");
  Serial.println("- Power On UC20 module (SOFTWARE START)");
  while(!gsm.powerOn(powerPin));   // default is 4, if not input param.
  Serial.print("- Firmware Version      : ");
  Serial.println(gsm.getFirmwareVersion());
  Serial.print("- Time                  : ");
  Serial.println(gsm.moduleTime());
  Serial.print("- SIM                   : ");
  Serial.println(gsm.simReady()? "READY":"NOT AVAILABLE");
  Serial.print("- Network status        : ");
  Serial.println(gsm.getNetworkStatus()? "READY":"NOT READY");
  Serial.print("- RSSI                  : ");
  Serial.println(gsm.getSignal());
  Serial.print("- IMEI                  : ");
  Serial.println(gsm.getIMEI());
  Serial.print("- IMSI                  : ");
  Serial.println(gsm.getIMSI());
  Serial.print("- ICCID                 : ");
  Serial.println(gsm.getICCID());
  Serial.print("- Operator              : ");
  Serial.println(gsm.getOperator());
  Serial.println("++++++++++++++++++++++++++++++++++++++++++++++");
}

void loop() {
  if(Serial.available() > 0) {
    gsm.write(Serial.read());
  }
  if(gsm.available() > 0) {
    Serial.write(gsm.read());
  }
}
