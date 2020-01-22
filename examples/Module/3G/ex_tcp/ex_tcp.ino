#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

#define HOST "YOUR_SERVER"
#define PORT "YOUR_PORT"

UC20_TCP client(0);       // 0 is client_ID (range 0-11)
unsigned long prev_t = 0;
unsigned long interval = 20000;   // time interval between each packet (ms)
unsigned int pack_no = 1;         // data example

void setup() {
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);
  gsm.initial(SLOT2);

  Serial.println("########### UC20 TCP ###########");
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

//  client.enableSSL();   // Uncomment to use SSL/TLS connection
}

void loop() {
  // listen to URC-Receive
  if(client.waitrecv() != -1) {
    Serial.print("response message: ");
    while(client.available() > 0) {
      Serial.write(client.read());
    }
  }
  // tcp send process
  unsigned long curr_t = millis();
  if(curr_t-prev_t > interval || prev_t == 0) {
    prev_t = curr_t;
    
    // check connection before send data
    if(!client.connected()) {
      if(!client.activated()) {
        client.activate();
        Serial.print("reconnect - IP address: ");
        Serial.println(client.getLocalIP());
      }
      client.disconnect();
      client.connect(HOST,PORT);
    }
    client.beginSend();
    gsm.print(pack_no++);
    client.endSend();
  }
}
