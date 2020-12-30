#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"
/* 
  TONY_GPS = Class
  TonyGPS = User defined name
*/

uint16_t checkSleep = 500;

void setup()
{
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  //--------- GPS Module Config ---------//
  TonyGPS.slot(SLOT1);    //  Select slot such as SLOT1, SLOT1_U, SLOT2, SLOT2_U, SLOT3, SLOT3_U ...
                          //  SLOTx_U = Floor 2
  TonyGPS.begin(9600);    //  Select buad rate 
  TonyGPS.GPS_ForceOn(1); // 1 = Enable GPS  , 0 = Disable GPS
//--------------------------------------//
  Serial.print("Simple TinyGPS library v. "); Serial.println(TONY_GPS::library_version());
  Serial.println();
  readGPS();
}

void loop()
{
  Serial.println("\r\n\r\n");
  Serial.println("Go to Standby mode");
  while (!TonyGPS);
  TonyGPS.print( F("$PMTK161,0*28\r\n") );
  delay(100);
  for(uint8_t i = 0; i<10; i++)
  {
    readGPS();
  }

  delay(100);
  Serial.println("\r\n\r\n");
  Serial.println("Go to Full mode");
  TonyGPS.println("Wake-up please");
  delay(1000);
  for(uint8_t i = 0; i<10; i++)
  {
    readGPS();
  }
  delay(1000);
}

void readGPS()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (TonyGPS.available())
    {
      char c = TonyGPS.read();
       Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (TonyGPS.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    TonyGPS.f_get_position(&flat, &flon, &age);
    
    Serial.print("LAT=");
    if(flat == TONY_GPS::GPS_INVALID_F_ANGLE)Serial.print(0.0);
    else Serial.print(flat, 6);

    Serial.print(" LON=");
    if(flon == TONY_GPS::GPS_INVALID_F_ANGLE)Serial.print(0.0);
    else Serial.print(flon, 6);

    Serial.print(" SAT=");
    if(TonyGPS.satellites() == TONY_GPS::GPS_INVALID_SATELLITES)Serial.print(0);
    else Serial.print(TonyGPS.satellites());
  
    Serial.print(" PREC=");
    if(TonyGPS.hdop() == TONY_GPS::GPS_INVALID_HDOP)Serial.print(0);
    else Serial.print(TonyGPS.hdop());
  }

  TonyGPS.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
  {
    Serial.println("** No characters received from GPS **");
  }
  checkSleep = chars;
}