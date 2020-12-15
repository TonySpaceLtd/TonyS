#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

EC25_FILE file;

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
  Serial.print("Total Space: ");
  Serial.println(file.getTotalSpace(RAM));
  Serial.print("Free Space: ");
  Serial.println(file.getFreeSpace(RAM));
  Serial.print( file.getfileInfo(UFS,"*"));
  Serial.print("get file size: ");
  size_t fs = file.fileSize(UFS,"http.temp");
  Serial.println(fs);
  int f = file.open(UFS,"http.temp");
  uint8_t buf[fs];
  size_t s = file.read(f,buf,fs);
  Serial.println(s);
  Serial.write(buf,s);
  file.close(f);
}
void loop() 
{
  if(LTE.ECser.available())
    Serial.write(LTE.ECser.read());
  if(Serial.available())
    LTE.ECser.write(Serial.read());
}
