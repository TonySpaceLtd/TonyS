#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

#define HOST "YOUR_SERVER_HTTPS_URL"
#define AUTH "YOUR_SECRET"

UC20_Firebase fbase;

unsigned long prev_t = 0;
unsigned long interval = 20000;   // time interval between each packet (ms)
unsigned long pack_no = 1;        // data example
String pathA = "/ex_push";
String pathB = "/ex_Set";

void setup() {
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);
  gsm.initial(SLOT2);
  Serial.println("###### UC20 with FIREBASE ######");
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
  
  fbase.begin(HOST,AUTH);
}

void loop() {
  unsigned long curr_t = millis();
  if(curr_t-prev_t > interval || prev_t == 0) {
    prev_t = curr_t;

    // push data to firebase
    String a = String(pack_no++);                                 // dataA
    String b = "\"myText\"";                                      // dataB
    String jsObj = "{\"dataA\":" + a + ",\"dataB\":" + b + "}";   // a group of data in json format
    String addpth = fbase.push(pathA,jsObj);                      // push a group of data
    Serial.println("push : "+ pathA + "/" + addpth); 

    // set and get data on firebase
    String c = fbase.getString(pathB);                            // get existed data
    Serial.println("get : " + pathB + " = " + c);
    if(c == "ON") 
      fbase.setString(pathB,"OFF");                               // set new data (case1)
    else 
      fbase.setString(pathB,"ON");                                // set new data (case2)
    Serial.println("set : " + pathB);
    
    c = fbase.getString(pathB);                                   // get new update data
    Serial.println("get : " + pathB + " = " + c);
    Serial.println("<----------------------------------->");
  }

  // remove data on firebase
  if(pack_no > 10) {
    if(fbase.remove(pathA)) {                                     // remove object that pathA point to
      pack_no = 1;
      Serial.println(pathA + " has been removed");
      Serial.println("<----------------------------------->");
    }
  }
}
