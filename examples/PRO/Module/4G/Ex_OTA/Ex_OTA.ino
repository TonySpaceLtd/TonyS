#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

EC25_OTA ota;

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
  bool ret = ota.updateFirmware("https://downloads.thaieasyelec.com/support/LED_Blink_200ms.bin");
  if(ret)
  {
    Serial.println("Reboot...");
    ESP.restart();
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
