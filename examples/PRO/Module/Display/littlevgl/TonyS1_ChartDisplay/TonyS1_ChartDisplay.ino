#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"
#include "display.h"
#include <SPI.h>
#include <Wire.h>
Adafruit_ST7789 tft = Adafruit_ST7789(SLOT1);
RTC_DS3231 rtc;
Adafruit_SHT31 sht31 = Adafruit_SHT31();
unsigned long previousClock = 0;  
const long interval = 500;

void setup() 
{
  Serial.begin(115200); 
  Tony.begin();
  Serial.println(F("Initialized"));
  rtc.begin();
   if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
  }
  lvglInit(&tft);
  createDisplay();
  startLvglHandle();
}

void loop() 
{ 
   unsigned long currentMillis = millis();
  if (currentMillis - previousClock >= interval) 
  {
     DateTime now = rtc.now();
     String str_t = String(now.hour())+":"+String(now.minute())+":"+String(now.second());
     showTime(str_t);
     float t_rtc = rtc.getTemperature();
     float t = sht31.readTemperature();
     float h = sht31.readHumidity();
     setTempRTC(t_rtc);
     setTempSHT(t);
     setHumidity(h);
     updatechart();
     String str = "#FF0000 Temp(SHT30) "+String(t,2)+" C#\n"+"#FFFF00 Temp(RTC) "+String(t_rtc,2)+" C#\n"+"#0000FF Humidity "+String(h,2)+"%#\n";
     setlabel(str);
     previousClock = millis();
  }
}
