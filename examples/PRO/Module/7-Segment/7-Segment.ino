#include <TonyS_X1.h>
#include "TonyS_X1_ExternalModule.h"


uint32_t start, stop;

void setup()
{
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  Tony7Seg.begin(); //----  begin 7-Segment 

  Serial.println("\nTest 7-Segment");
  Tony7Seg.displayOn();
  Serial.println("Display Testing");
  Tony7Seg.displayTest(100);
  Tony7Seg.displayOff();
  delay(1000);
  Tony7Seg.displayOn();
}

void loop()
{
  //----- Print integer ------//
  Serial.println("Example print integer");
  Tony7Seg.displayInt(9999);
  delay(1000);


  //----- Print Hexadecimal ------//
  Serial.println("Example print hexadecimal");
  Tony7Seg.displayHex(0xABCF);
  delay(1000);


  //----- Dim the display's light ------//
  Serial.println("Example dim the display's light");
  for (int i = 0; i < 16; i++)
  {
    Tony7Seg.displayHex(0xABC0 + i);
    Tony7Seg.brightness(i);
    delay(500);
  }
  for (int i = 15; i >= 0; i--)
  {
    Tony7Seg.displayHex(0xABC0 + i);
    Tony7Seg.brightness(i);
    delay(500);
  }
  delay(1000);
  Tony7Seg.brightness(2);


  //----- Clear display ------//
  Serial.println("Example clear the display");
  Tony7Seg.displayClear();
  delay(1000);


  //----- Print time ------//
  Serial.println("Example show time");
  Tony7Seg.displayTime(13, 25);
  for (int i = 50; i < 60; i++)
  {
    Tony7Seg.displayTime(13, i);
    delay(500);
    Tony7Seg.displayColon(true);
    delay(500);
  }


  //----- Print date ------//
  Serial.println("Example print date");
  uint8_t dpm[12] = { 31, 28, 31, 30,   31, 30, 31, 31,   30, 32, 30, 31 };
  for (uint8_t mo = 0; mo < 12; mo++)
  {
    for (uint8_t da = 1; da <= dpm[mo]; da++)
    {
      Tony7Seg.displayDate(mo + 1, da);
      delay(200);
    }
  }


  Tony7Seg.displayTime(13, 25);
  for (int i = 50; i < 60; i++)
  {
    Tony7Seg.displayTime(13, i);
    delay(500);
    Tony7Seg.displayColon(true);
    delay(500);
  }


  //----- Print float ------//
  Serial.println("Example Float");
  Tony7Seg.displayClear();
  delay(500);
  Tony7Seg.displayFloat(3.14159265);
  delay(500);
  Tony7Seg.displayFloat(99999);
  delay(500);
  Tony7Seg.displayFloat(9999);
  delay(500);
  Tony7Seg.displayFloat(31.4159265);
  delay(500);
  Tony7Seg.displayFloat(314.159265);
  delay(500);
  Tony7Seg.displayFloat(3141.59265);
  delay(500);
  Tony7Seg.displayFloat(0.314159265);
  delay(500);
  Tony7Seg.displayFloat(0.0314159265);
  delay(500);
  Tony7Seg.displayFloat(0.00314159265);
  delay(500);
  Tony7Seg.displayFloat(0.000314159265);
  delay(500);


  //----- Print digits ------//
  Serial.print("SET DIGITS TEST :  ");
  for (uint8_t dig = 0; dig < 5; dig++)
  {
    Serial.print(dig);
    Tony7Seg.setDigits(dig);
    Tony7Seg.displayInt(0);
    delay(1000);
  }
  Tony7Seg.setDigits(1);


  //----- Example voltage meter ------//
  Serial.println();
  Serial.println("Voltmeter");
  for (int i = 0; i < 100; i++)
  {
    float voltage = analogRead(A0) * 5.0 / 1023;
    Tony7Seg.displayFloat(voltage);
    delay(100);
  }


  //----- Example blink ------//
  Serial.println("Test blinking");
  for (uint8_t i = 0; i < 3; i++)
  {
    Tony7Seg.displayHex(0xABC0 + i);
    Tony7Seg.blink(i);
    delay(4000);
  }
  Tony7Seg.blink(0);


  //----- Example counter ------//
  Serial.print("Integer counting test :  ");
  start = millis();
  for (int16_t counter = -999; counter < 9999; counter++)
  {
    Tony7Seg.displayInt(counter);
  }
  stop = millis();
  Serial.println(stop - start);


  Serial.print("Hexadecimal counting test :  ");
  start = millis();
  for (uint16_t counter = 0; counter < 0xFFFF; counter++)
  {
    Tony7Seg.displayHex(counter);
  }
  stop = millis();
  Serial.println(stop - start);

  Serial.println("\r\n");
  delay(2000);
}