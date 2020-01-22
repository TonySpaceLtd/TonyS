#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

UC20_GNSS gps;
unsigned long prev_t = 0;
unsigned long interval = 20000;

String topic[] = {"Date","Time","Latitude,Longtitude","HDOP","Altitude",
                  "Fix","COG","speed(km/h)","speed(knots)","nsat"};
int indent[] = {10,21,43,50,61,67,76,90,105,112};

void setup() {
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);
  gsm.initial(SLOT2);

  Serial.println(F("########### UC20 GNSS ###########"));
  Serial.print("Power on module...");
  while(!gsm.powerOn()) 
    Serial.print(".");
  Serial.println(" READY");
  Serial.println("Start GPS Session");
  gps.begin();                                    // Start GPS session
  Serial.print("Connection to Satellites...");
  while(!gps.getPosition()) {                     // get data to check connection
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.println("Connected!");
  Serial.println();
  printTopic();                                  // print GPS topic header
}

void loop() {
  unsigned long curr_t = millis();
  if(curr_t-prev_t > interval || prev_t == 0) {
    if(gps.getPosition()) {                       // get new data
      prev_t = curr_t;
      printInfo();                                // print GPS info
    }
    else {
      delay(1000);
    }
  }
}

void printTopic() {
  int i = 0;
  for(uint8_t j=0; j<sizeof(indent)/sizeof(int); j++) {
    i += Serial.write(' ');
    i += Serial.print(topic[j]);
    while(i<indent[j]+1) {
      i += Serial.write(' ');
    }
  }
  Serial.println();
  i = 0;
  for(uint8_t j=0; j<sizeof(indent)/sizeof(int); j++) {
    while(i<indent[j]) 
      i += Serial.write('-');
    i += Serial.write('+');
  }
  Serial.println();
}

void printInfo() {
  int i = 0;
  i += Serial.write(' ');
  i += Serial.print(gps.date());           // string
  i += printCharFor(' ',indent[0]-i+2);
  i += Serial.print(gps.time());           // string
  i += printCharFor(' ',indent[1]-i+2);
  i += Serial.print(gps.latitude(),5);     // float (5 decimal points precision)
  i += Serial.write(',');
  i += Serial.print(gps.longtitude(),5);   // float (5 decimal points precision)
  i += printCharFor(' ',indent[2]-i+2);
  i += Serial.print(gps.hdop(),1);         // float (1 decimal points precision)
  i += printCharFor(' ',indent[3]-i+2);
  i += Serial.print(gps.altitude(),1);     // float (1 decimal points precision)
  i += printCharFor(' ',indent[4]-i+2);
  i += Serial.print(gps.fix());            // integer
  i += printCharFor(' ',indent[5]-i+2);
  i += Serial.print(gps.cog());            // string
  i += printCharFor(' ',indent[6]-i+2);
  i += Serial.print(gps.spkm(),1);         // float (1 decimal points precision)
  i += printCharFor(' ',indent[7]-i+2);
  i += Serial.print(gps.spkn(),1);         // float (1 decimal points precision)
  i += printCharFor(' ',indent[8]-i+2);
  i += Serial.print(gps.nsat());           // integer
  Serial.println();
}

int printCharFor(char c, int n) {
  for(int i=0; i<n; i++) 
    Serial.write(c);
  return n;
}
