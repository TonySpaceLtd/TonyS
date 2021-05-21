#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

//---------------- Setting Parameter ----------------//
String Syncword = "10";  // Set value 0 to FF
//---------------------------------------------------//

uint16_t timeout = 0;
char charbuff[40];   // char array buffer
String stringbuff;   // string buffer

void setup() {
  Serial.begin(115200);
  Tony.begin();

  TonyLORA.slot(SLOT2);   // Select the slot
  Serial.println();

  /* Set Syncword  */
  timeout = 1000;
  if(TonyLORA.setSyncword(Syncword, timeout)) Serial.println("Set up syncword successfully");
  else Serial.println("Set up syncword fail");
  Serial.println();


  /* Please reset the module after set Syncword  */
  TonyLORA.reset();
  Serial.println("Reset the module successfully");
  Serial.println();


  /* receiving brodcast */
  /*  Maximum data size = 255 byte  */
  timeout = 20000; 
  if(TonyLORA.receiveBrodcast(charbuff, timeout) == 1) // return 1 = successfully, 0 = fail
  {
    Serial.print("Data : ");
    stringbuff = charbuff;  // Convert char to string
    Serial.println(stringbuff);
  }
  else Serial.println("Receiving brodcast fail.");
  charbuff[0] = '\0';  // Clear char array buffer
}

void loop() {
}
