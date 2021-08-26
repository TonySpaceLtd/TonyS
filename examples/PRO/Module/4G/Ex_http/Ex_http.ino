#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

EC25_HTTP http;
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
}
void loop() 
{
   httpget();
   delay(500); 
}
void httpget()
{
  Serial.print("Set URL :");
  int ret = http.setURL("http://covid19.ddc.moph.go.th/api/Cases/today-cases-all"); //URL must begin with “http://"
  if(ret)
  {
    Serial.println("OK");
    Serial.print("GET: ");
    ret = http.get();
    Serial.println(ret);
    if(ret==200)
    {
        uint8_t buff[101];
        size_t s;
        while(s = http.read(buff,100))
        {
          Serial.write(buff,s);
        }
        Serial.println("\r\n####Finish####\r\n"); 
    }
  }
  else
    Serial.print("Error");

}
