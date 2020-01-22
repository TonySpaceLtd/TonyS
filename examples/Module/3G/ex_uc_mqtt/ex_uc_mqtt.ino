#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

#define HOST       "YOUR_BROKER"
#define PORT       "YOUR_PORT"
#define USER       "YOUR_USERNAME"
#define PASS       "YOUR_PASSWORD"
#define CLIENT_ID  "UC20_NODE"
#define TOPIC_SEND "count"
#define TOPIC_SUB  "count"

UC_MQTT mq(0);
unsigned long prev_t = 0;
unsigned long interval = 20000;   // time interval between each packet (ms)
unsigned long pack_no = 0;        // "data" of this example

void setup() {
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);
  gsm.initial(SLOT2);

  Serial.println("########## UC20 MQTT ##########");
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
  mq.enableSSL();
}

void loop() {
  unsigned long curr_t = millis();
  if(curr_t-prev_t > interval || prev_t == 0) {
    prev_t = curr_t;
    if(!mq.brokerConnected()) {
      Serial.println("Connection lost. Reconnecting...");
      mq.brokerDisconnect();
      if(mq.brokerConnect(HOST,PORT)) {
        Serial.print("MQTT connect   : ");
        Serial.println(mq.connect(CLIENT_ID,USER,PASS)==0? "Connection Accepted":"Fail");
        Serial.print("MQTT subscribe : ");
        Serial.println(mq.subscribe(TOPIC_SUB)? "Success":"Fail");
        Serial.println();
      }
    }
    if(mq.brokerConnected()) {
      String topic = TOPIC_SEND;
      String message = String(pack_no++);
      mq.publish(topic, message);
    }
  }

  if(mq.recvPublish()) {
    Serial.println("<---- Received message ---->");
    Serial.println("topic   : " + mq.getTopic());
    Serial.println("message : " + mq.getMessage());
    Serial.println("<-------------------------->");
  }
}
