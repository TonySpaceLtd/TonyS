#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

//---------------- Setting Parameter ----------------//
String Syncword = "10";  // Set value 0 to FF
//---------------------------------------------------//

uint16_t timeout = 0;

void setup() {
  Serial.begin(115200);
  Tony.begin();

  TonyLORA.slot(SLOT2);   // Select the slot
  Serial.println();

  /* Set Syncword  */
  timeout = 1000;
  if(TonyLORA.setSyncword(Syncword, timeout)) Serial.println("Set up syncword successfully");
  else Serial.println("Set syncword fail");
  Serial.println();


  /* Please reset the module after set Syncword  */
  TonyLORA.reset();
  Serial.println("Reset the module successfully");
  Serial.println();


  /* Sending brodcast */
  timeout = 2000;
  String testdata = "5008754215"; //Please fill the data in pairs. Such as 10, 1020 or 102030  
  /*                  testdata = data for sending (String)
                      Maximum data size = 255 byte      */
  if(TonyLORA.sendBrodcast(testdata, timeout) == 1) // return 1 = successfully, 0 = fail
  {
    Serial.println("Sending brodcast successfully.");
  }
  else Serial.println("Sending brodcast fail.");
}

void loop() {
}
