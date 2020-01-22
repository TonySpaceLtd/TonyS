#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

#define DEST_NO "PHONE_NUMBER"  // SMS destination

UC20_SMS s;

void setup() {
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);
  gsm.initial(SLOT2);

  Serial.println("########## UC20 SMS ##########");
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

  // read sms
  Serial.println("- Read all SMS in storage");
  s.list("ALL");      // available param
                      // "ALL","REC UNREAD","REC READ"
  Serial.println();

  // send sms
//  Serial.println("send sms");
//  s.beginsend(DEST_NO);
//  gsm.printlnUnicode(F("แอบมองเธออยู่นะจ๊ะ~"));
//  gsm.printUnicode  (F("แต่เธอไม่รู้บ้างเลย~"));
//  bool send_ok = s.endsend();
//  Serial.println(send_ok? "Send OK":"Send Fail");
  
  Serial.println("\n...");
}

void loop() {
  int recvIndex = s.listen();   // wait incoming SMS
  if(recvIndex != -1) {
    Serial.print("Receive SMS, index ");
    Serial.println(recvIndex);
    Serial.println("Reading...");
    Serial.println(s.readHeadIndex(recvIndex));
    Serial.println(s.readIndex(recvIndex));
    Serial.println("<----------------------->");
  }
}
