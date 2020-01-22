#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"
#include "display.h"
#include <SPI.h>
#include <Wire.h>

float t;
float h;
Adafruit_ST7789 tft = Adafruit_ST7789(SLOT1,TFT_240_320);
Adafruit_SHT31 sht31 = Adafruit_SHT31();
unsigned long previousMillis = 0; 
const long interval = 1000;

void setup() 
{
 Serial.begin(115200); 
 Tony.begin();
 if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
 }
  lvglInit(&tft);
  createDisplay();
  startLvglHandle();
}
void loop() 
{
 unsigned long currentMillis = millis();
 if (currentMillis - previousMillis >= interval) 
  {
     readSHT3x();
     previousMillis = millis();
  }     
}
void readSHT3x()
{
   t = sht31.readTemperature();
   h = sht31.readHumidity();
      
   if (! isnan(t)) 
   {
      Serial.print("Temp *C = "); Serial.println(t);
   } 
   else 
   { 
      Serial.println("Failed to read temperature");
   }
   if (! isnan(h)) 
   { 
        Serial.print("Hum. % = "); Serial.println(h);
   } 
   else 
   { 
    Serial.println("Failed to read humidity");
   }
   updateTempHumi(t,h);
 
}
