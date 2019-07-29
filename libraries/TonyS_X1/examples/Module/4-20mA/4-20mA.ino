//---------- Select Slot for plug-in breakout board -----------//
#define  T4_20mA_Floor1     //-----  Select T4_20mA_Floor1(Floor 1) or T4_20mA_Floor2(Floor 2)    
#define  T4_20mA_Slot    1 // ----  Selec Slot 1-6
//-------------------------------------------------------------//

#include <TonyS1.h>
#include <TonyS1_Breakout.h>

TONY_S1 Tony;   //  Call Library's class
#define  Power   2   //  Pin 2 for control ON-OFF power

void setup() 
{
  Serial.begin(115200);
  Serial.println("TonyS1 Example");
  Wire.begin();
  
  Tony.begin();  //----  begin Library
  delay(10);

  pinMode(Power, OUTPUT); // ------- Set to OUTPUT
  digitalWrite(Power, HIGH);  //---- HIGH for ON Board's power
}

void loop() 
{
  float dataADC = 0;
  dataADC = Tony.analogRead(T4_20mA_Port);   //---- Read analog from pin 0 (Range 0-10V)
  Serial.print("Value = ");
  Serial.println(dataADC,3);     
  delay(1000); 
}