
#ifndef LVGL_ESP32_H
#define LVGL_ESP32_H

#include "TonyS_X1_ExternalModule.h"
#include <SPI.h>

#include "lvgl/lvgl.h"

void lvglInit(Adafruit_ST7789 *display,size_t w=240,size_t h=240);
void startLvglHandle();

#endif





















