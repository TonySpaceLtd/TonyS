#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

void setup() 
{
    Serial.begin(115200);
    Serial.println("TonyS_X1 Example");
    Tony.begin();  //----  begin Library
    
//--------- RS485 Module Config ---------//
    RS485.slot(SLOT1);    //  Select slot such as SLOT1, SLOT1_U, SLOT2, SLOT2_U, SLOT3, SLOT3_U ...
                          //  SLOTx_U = Floor 2
    RS485.begin(9600);  // Select buad rate 
//--------------------------------------//
}

void loop() 
{
  byte getData = 0;
  uint8_t getByteNumber = 0;
  bool statusRequest = 0;
  
                //RS485.requestData(Slave ID, Function, Start Address, Number of Data, Timeout);
  statusRequest = RS485.requestData(1, 3, 0, 5, 1000);      //Return 1 = Sent success , and Return 0 = Not success

                //RS485.get_byteNumber() = Get number of Data
  getByteNumber = RS485.get_byteNumber();

  if(getByteNumber > 1)
  {
    Serial.print("Data = ");
    for(uint8_t i = 0; i<getByteNumber; i++)
    {
              //RS485.readByte(i) = read data from byte (i)
      getData = RS485.readByte(i);
      Serial.print("0x");
      if (getData < 16) {Serial.print("0");}
      Serial.print(getData, HEX);
      Serial.print(" ");
    }
    Serial.println(); 
  }

  //-------- Reading register ---------
  uint16_t read_reg = 0;
  if(getByteNumber > 1) 
  {
    read_reg = RS485.readRegister(2);  // Read data from address  2 (2 Byte)
    Serial.print("Read from address 2");
    Serial.print(" : 0x");
    Serial.println(read_reg,HEX);
	
	read_reg = RS485.readRegister(3);  // Read data from address  3 (2 Byte)
    Serial.print("Read from address 3");
    Serial.print(" : 0x");
    Serial.println(read_reg,HEX);
  }
  Serial.println();
  delay(1000);
}