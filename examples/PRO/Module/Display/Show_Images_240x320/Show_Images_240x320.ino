#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"
#include "MyBitmap.h"  // Include Image file
#include "MyLogo.h"    // Include Image file

                                    //SLOT
Adafruit_ST7789 tft = Adafruit_ST7789(SLOT1,TFT_240_320); // Call Library's Class and Select SLOT

void setup(void) 
{
  Serial.begin(115200);
  Tony.begin();  //----  begin Library  
  Serial.println("Test Show Image");
  tft.init(240, 320);  // Init the display
  tft.setRotation(2);  // Set Rotation of image
}

void loop() 
{
  //Start from 0, 0, The name of Image's buffer, Image's width, Image's height
  tft.drawRGBBitmap(0, 0, (uint16_t *)MyLogo, IMAGE_WIDTH, IMAGE_HEIGHT);
  delay(2000);
  tft.drawRGBBitmap(0, 0, (uint16_t *)MyBitmap, IMAGE_WIDTH, IMAGE_HEIGHT);
  delay(2000);
}