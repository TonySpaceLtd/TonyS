#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

UC20_HTTP client;

void setup() {
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);
  gsm.initial(SLOT2);

  Serial.println("########## UC20 HTTP ##########");
  Serial.print("Power on module...");
  while(!gsm.powerOn()) 
    Serial.print(".");
  Serial.println(" READY");
  Serial.print  ("Network registration...");
  Serial.println(gsm.getNetworkStatus()? " READY":" NOT READY");
  Serial.println("IMEI     : " + gsm.getIMEI());
  Serial.println("IMSI     : " + gsm.getIMSI());
  Serial.println("Operator : " + gsm.getOperator());
  Serial.print  ("RSSI     : ");
  Serial.println(gsm.getSignal());
  Serial.println();
  
  client.initConfig();
  
  // To use Secure connection (HTTPs)
  // Start the <HOST> name with https://
  client.url("https://www.google.com");               // Set URL here
  Serial.println("- URL = https://www.google.com");

  // GET Request
  Serial.println("- GET (AUTO-HEADER)");
  client.get();
  client.wait_responseCode();
  Serial.println("- READ");
  while(client.available() > 0) {
    Serial.write(client.read());
  }
  Serial.println();

  // Edit header GET Request
  Serial.println();
  Serial.println("- GET (CUSTOM-HEADER)");
  client.get(40, CUSTOM_HEADER);        // 40 is length of data below
  gsm.println("GET / HTTP/1.1");        // 16 bytes here
  gsm.println("Host: www.google.com");  // 22 bytes here
  gsm.println();                        //  2 bytes here ; 16+22+2 = 40
  client.wait_responseCode();
  Serial.println("- READ");
  while(client.available() > 0) {
    Serial.write(client.read());
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
