
#ifndef LVGL_ESP32_H
#define LVGL_ESP32_H

#include <Adafruit_GFX.h>    
#include <Adafruit_ST7789.h>
#include <SPI.h>

#include "lvgl/lvgl.h"

void lvglInit(Adafruit_ST7789 *display,size_t w=240,size_t h=240);
void startLvglHandle();






#endif





















