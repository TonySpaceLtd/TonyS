#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

void setup() 
{
  Serial.begin(115200);
  Serial.println("TonyS_X1 Example");
  
  Tony.begin();  //----  begin Library


  Serial.println();
  Serial.println("//---- Test Read Write FRAM ----//");



  TonyFRAM.clear(); //-------  Clear all data on FRAM
  
  uint16_t sizeFRAM = 0;

  
  //--------------- Write --------------//
  //Write data : Address (0 - 511)  ,  Data (4 Byte) per time
  bool checkWrite = TonyFRAM.write(0 , 1548836325);  // Return 1 when successfully
  
  delay(1000);
  
  //--------------- Read --------------//
  //Read data : Address (0 - 511)  4 Byte per time
  uint32_t readdata = TonyFRAM.read(0);  //4 bytes of data for stored in variable
   

  //------------ Check Size -----------//
  //Check blank Address (It'll return first of blank address , if return = 512 That's mean FRAM full 
  //Check step 4 Byte
  sizeFRAM = TonyFRAM.size(4);
  if(sizeFRAM == 512)  // Full chip = 512 byte , 0-511 byte
  {        
    Serial.println("Data Full");
  }
  else 
  {
    Serial.println(sizeFRAM , HEX);
  }
}

void loop() 
{
	
}