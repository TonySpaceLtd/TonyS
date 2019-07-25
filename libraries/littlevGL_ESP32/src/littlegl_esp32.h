#ifndef _LVGL_ESP32_H_
#define _LVGL_ESP32_H_


#include "Arduino.h"
#include "FS.h"
#include "SPI.h"
#include "lvgl/lvgl.h"
#include <TFT_eSPI.h>
#include <Ticker.h>


#define LVGL_TICK_PERIOD 20
#define CALIBRATION_FILE "/calibrationData"

void initDisplay(TFT_eSPI *tft);
void initTouch();
void my_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
static void lv_tick_handler(void);
bool read_encoder(lv_indev_drv_t * drv, lv_indev_data_t*data);
void calibrateTouch(unsigned long timeout);
void lvglTask(void* data);
void startlvglTask();
		






































#endif