
#include "littlegl_esp32.h"

TFT_eSPI *tftObj;
Ticker tick;
lv_disp_t * disp;
void initDisplay(TFT_eSPI *tft)
{
	tftObj = tft;
	tftObj->begin();
	tftObj->init();
	tftObj->setRotation(1); /* Landscape orientation */
	tftObj->fillScreen(TFT_WHITE);
	static lv_disp_buf_t disp_buf_1;
	static lv_color_t buf1_1[LV_HOR_RES_MAX * 10]; 
	static lv_color_t buf1_2[LV_HOR_RES_MAX * 10];                      
	lv_disp_buf_init(&disp_buf_1, buf1_1, buf1_2, LV_HOR_RES_MAX * 10);   
	lv_disp_drv_t disp_drv;                        
	lv_disp_drv_init(&disp_drv); 
	disp_drv.buffer = &disp_buf_1;
	disp_drv.flush_cb = my_flush_cb;
	
	disp = lv_disp_drv_register(&disp_drv);
	
	//xTaskCreate(lvglTask,"lvglTask",10000,NULL,10,NULL);
}
void initTouch()
{
  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
 // indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.type =LV_INDEV_TYPE_NONE;
  indev_drv.read_cb = read_encoder;
  lv_indev_drv_register(&indev_drv);
}
static void lv_tick_handler(void)
{
  lv_tick_inc(LVGL_TICK_PERIOD);
}
void my_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
  uint16_t c;
  tftObj->startWrite(); 
  tftObj->setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); 
  for (int y = area->y1; y <= area->y2; y++) {
    for (int x = area->x1; x <= area->x2; x++) {
      c = color_p->full;
      tftObj->writeColor(c, 1);
      color_p++;
    }
  }
  tftObj->endWrite();
  lv_disp_flush_ready(disp_drv);
}
bool read_encoder(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
/*	uint16_t x, y;
	static uint16_t prev_x, prev_y;
	if (tftObj->getTouch(&x, &y)) 
	{
		data->point.x = x;
		data->point.y = y;
		data->state = LV_INDEV_STATE_PR;
		prev_x = data->point.x;
		prev_y = data->point.y;
	}
	else 
	{
		data->point.x = prev_x;
		data->point.y = prev_y;
		data->state = LV_INDEV_STATE_REL;  
	}
	*/
	static int32_t last_diff = 0;

  int32_t diff = 0; /* Dummy - no movement */
  int btn_state = LV_INDEV_STATE_REL; /* Dummy - no press */

  data->enc_diff = diff - last_diff;;
  data->state = btn_state;

  last_diff = diff;
	return false;
}
void touchCalibrate()
{
 /* uint16_t calibrationData[5];
  uint8_t calDataOK = 0;
   // check file system
  if (!SPIFFS.begin()) {
    Serial.println("formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }
  // check if calibration file exists
  if (SPIFFS.exists(CALIBRATION_FILE)) {
    File f = SPIFFS.open(CALIBRATION_FILE, "r");
    if (f) {
      if (f.readBytes((char *)calibrationData, 14) == 14)
        calDataOK = 1;
      f.close();
    }
  }
  if (calDataOK) {
    // calibration data valid
    tftObj->setTouch(calibrationData);
  } else {
    // data not valid. recalibrate
    tftObj->calibrateTouch(calibrationData, TFT_WHITE, TFT_RED, 15);
    // store data
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calibrationData, 14);
      f.close();
    }
  }
  */
}
void calibrateTouch(unsigned long timeout)
{
/*	unsigned long pvtime = millis();
	uint16_t calibrationData[5];
	uint8_t cnt=10;
	uint16_t x, y;
	lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label, "Calibrate Touch Screen");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_task_handler();
	do
	{
		
		if (tftObj->getTouch(&x, &y)) 
		{
			
				tftObj->calibrateTouch(calibrationData, TFT_WHITE, TFT_RED, 15);
				 File f = SPIFFS.open(CALIBRATION_FILE, "w");
					if (f) 
					{
						f.write((const unsigned char *)calibrationData, 14);
						f.close();
					}	
				tftObj->setTouch(calibrationData);
		}
		
	}
	while((millis()-pvtime)<timeout);
	lv_obj_del(label);
	tftObj->fillScreen(TFT_WHITE);
	*/
}
void lvglTask(void* data)
{
  for(;;)
  {
	lv_task_handler(); /* let the GUI do its work */
	delay(5);
	//vTaskDelay(1);
  }
  vTaskDelete( NULL );
}
void startlvglTask()
{
	tick.attach_ms(LVGL_TICK_PERIOD, lv_tick_handler);
	xTaskCreate(lvglTask,"lvglTask",10000,NULL,10,NULL);
}