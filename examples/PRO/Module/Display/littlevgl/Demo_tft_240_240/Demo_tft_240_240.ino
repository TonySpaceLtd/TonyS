#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"
#include "display.h"

float t;
float h;

Adafruit_ST7789 tft = Adafruit_ST7789(SLOT1,TFT_240_240);
unsigned long previousMillis = 0; 
const long interval = 1; //Interval for update the display

void setup() 
{
 Serial.begin(115200); 
 Tony.begin();

  lvglInit(&tft);
  createDisplay();
}
void loop() 
{
 unsigned long currentMillis = millis();
 if (currentMillis - previousMillis >= interval) 
  {
     simulateData();
     previousMillis = millis();
  }     
}
void simulateData()
{
  //simulate data, t = temperature and h = humidity
  t = random(30, 100);
  h = t-(random(0, t-2));
      
   if (! isnan(t)) 
   {
      Serial.print("Temp *C = "); Serial.println(t);
   } 

   if (! isnan(h)) 
   { 
        Serial.print("Hum. % = "); Serial.println(h);
   } 
 
   updateTempHumi(t,h);
   lv_task_handler(); /* let the GUI do its work */
}