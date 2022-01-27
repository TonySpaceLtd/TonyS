#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"


/*
 Please enter information appeui , appkey (Not recommended for deveui)
 */
//---------------- Setting Parameter ----------------//
String deveui = "";                                  // Such as "9c65f9fffe3f4a33" ,  "" = Not edit (no change from the current value)
String appeui = "0000000000000000";                  // Such as "0000000000000000" , "" = Not edit (no change from the current value)
String appkey = "8124BB216C38DBA9B511613F1F508799";  // Such as "8124BB216C38DBA9B511613F1F508799" , "" = Not edit (no change from the current value)
//--------------------------------------------------//

uint16_t timeout = 0;
char charbuff[18];   // char array buffer
String stringbuff;   // string buffer
String mydeveui = "9c65f9fffe3f4a33";  // for test compare string data
  
void setup() 
{
  Serial.begin(115200);
  Tony.begin();
  
  TonyLORA.slot(SLOT2);   // Select the slot
  Serial.println();
  
  
  /* Begin and setting up all parameter 
     You can skip this function If no parameter for setting to the module */
  if(TonyLORA.begin(deveui, appeui, appkey) == 1)  // return 1 = successfully, 0 = fail
  {
    Serial.println("The LoRaWAN module setup was successfully");
  }
  else Serial.println("The LoRaWAN module setup was fail");
  Serial.println();


  /*  Test get value from module  */
  Serial.print("deveui : ");
  timeout = 1000;
  TonyLORA.getDeveui(charbuff, timeout); // Print the current deveui value
  stringbuff = charbuff;  // Convert char to string
  Serial.println(stringbuff);
  Serial.println("Testing compare data between output from function and string buffer...");
  if(stringbuff == mydeveui) // compare between output(stringbuff) and deveui(mydeveui)
  {
    Serial.println("---> The deveui is correct !");  
    Serial.println();
  }
  else
  {
    Serial.println("---> The deveui is incorrect !");  
    Serial.println();
  }
  charbuff[0] = '\0';  // Clear char array buffer

  Serial.print("appeui : ");
  timeout = 1000;
  TonyLORA.getAppeui(charbuff, timeout); // Print the current appeui value
  stringbuff = charbuff;  // Convert char to string
  Serial.println(stringbuff);
  Serial.println();
  charbuff[0] = '\0';  // Clear char array buffer

  Serial.print("appkey : ");
  timeout = 1000;
  TonyLORA.getAppkey(charbuff, timeout); // Print the current appkey value
  stringbuff = charbuff;  // Convert char to string
  Serial.println(stringbuff);
  Serial.println();
  charbuff[0] = '\0';  // Clear char array buffer


  /* Test join OTAA 
     Sending the request join to OTAA  */
  timeout = 1000;
  if(TonyLORA.joinOTAA(timeout)) Serial.println("The request join to OTAA Successfully");
  else Serial.println("The request join to OTAA Fail");


  /*   Check status after sending the request join to OTAA   */
  timeout = 15000;
  Serial.print("OTAA : ");
  if(TonyLORA.joinStatus(timeout)) Serial.println("Joined");
  else Serial.println("Unjoined");
  Serial.println();
  delay(1000);

  
  /*   Test get the respond from gateway  */
  // Example sending the request to join otaa and read the respond from gateway
  // Sending
  timeout = 1000;
  Serial.println("Testing get the the respond from gateway...");
  if(TonyLORA.joinOTAA(timeout)) Serial.println("The request join to OTAA Successfully");
  else Serial.println("The request join to OTAA Fail");
  // Read respond
  timeout = 15000;
  Serial.print("The respond : ");
  TonyLORA.getRespond(charbuff, timeout);
  stringbuff = charbuff;  // Convert char to string
  Serial.println(stringbuff);
  Serial.println();
  charbuff[0] = '\0';  // Clear char array buffer

  
  /* Write the data to Gateway */
  Serial.println("Testing send the data to gateway...");
  timeout = 15000;
  String testdata = "6500224455667756"; //Please fill the data in pairs. Such as 10, 1020 or 102030  
  /*                  (5 = Port, testdata = data for sending(String)) 
                      Port 0-223 , Maximum data size = 64 byte      */
  if(TonyLORA.stringWrite(5, testdata, timeout) == 1) // return 1 = successfully, 0 = fail
  {
    Serial.println("The data was sent to the gateway successfully");
  }
  else Serial.println("The data was sent to the gateway fail");
}

void loop() 
{
}