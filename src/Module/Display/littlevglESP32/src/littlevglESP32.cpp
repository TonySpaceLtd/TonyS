#include "littlevglESP32.h"

Adafruit_ST7789 *mytft;
JOYSTICK joy_lvgl;
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

#include <Ticker.h>
#define LVGL_TICK_PERIOD 20
Ticker tick; /* timer for interrupt handler */


void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
static void lv_tick_handler(void);
bool read_encoder(lv_indev_drv_t * indev, lv_indev_data_t * data);
bool buttons_read(lv_indev_drv_t * indev, lv_indev_data_t * data);

void lvglInit(Adafruit_ST7789 *display)
{
	size_t w,h;
	mytft = display;
	if(mytft->_tfttype == TFT_240_240)
	{
		w = 240;
		h = 240;
	}
	else
	{
		w = 240;
		h = 320;
	}
	lvglInit(w,h);
	
}

void lvglInit(size_t w,size_t h)
{
	lv_init();
	mytft->init(w,h); 
	if(h == 320)	
		mytft->setRotation(2);
	mytft->fillScreen(ST77XX_BLACK);
	mytft->fillScreen(ST77XX_WHITE);
	
	
	
	lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);
	
	
	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.hor_res = w;
	disp_drv.ver_res = h;
	disp_drv.flush_cb = my_disp_flush;
	disp_drv.buffer = &disp_buf;
	lv_disp_drv_register(&disp_drv);
  
	lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);
	//if(joy_lvgl.begin())
	//{
		//indev_drv.type = LV_INDEV_TYPE_KEYPAD;
		//indev_drv.read_cb = buttons_read;
		//emulated_kp_indev = lv_indev_drv_register(&kp_drv);
	//}
	//else	
	//{
		indev_drv.type = LV_INDEV_TYPE_ENCODER;
		indev_drv.read_cb = read_encoder;
		lv_indev_drv_register(&indev_drv);
	//}
	
	
   /*Initialize the graphics library's tick*/
    tick.attach_ms(LVGL_TICK_PERIOD, lv_tick_handler);
	
}
void lv_handler(void *)
{
	for(;;)
	{
		lv_task_handler(); /* let the GUI do its work */
		delay(5);
	}
	vTaskDelete( NULL );
}
void startLvglHandle()
{
	 xTaskCreate( lv_handler,"lv_handler",10000,NULL,1,NULL);      
}
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint16_t c;

  mytft->startWrite(); /* Start new TFT transaction */
  mytft->setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); /* set the working window */
  for (int y = area->y1; y <= area->y2; y++) {
    for (int x = area->x1; x <= area->x2; x++) {
      c = color_p->full;
      mytft->writeColor(c, 1);
      color_p++;
    }
  }
  mytft->endWrite(); /* terminate TFT transaction */
  lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}

static void lv_tick_handler(void)
{

  lv_tick_inc(LVGL_TICK_PERIOD);
}

bool read_encoder(lv_indev_drv_t * indev, lv_indev_data_t * data)
{
  static int32_t last_diff = 0;
  int32_t diff = 0; /* Dummy - no movement */
  int btn_state = LV_INDEV_STATE_REL; /* Dummy - no press */

  data->enc_diff = diff - last_diff;;
  data->state = btn_state;

  last_diff = diff;

  return false;
}
bool buttons_read(lv_indev_drv_t * indev, lv_indev_data_t * data)
{
	uint8_t pressKey = joy_lvgl.getCursor();
	if(pressKey == PRESS_UP)
	{
		data->key = 2;
		data->state = LV_INDEV_STATE_PR;
	}
	else
	{
		data->key = 2;
		data->state = LV_INDEV_STATE_REL;
	}
	if(pressKey == PRESS_DOWN)
	{
		data->key = 1;
		data->state = LV_INDEV_STATE_PR;
	}
	else
	{
		data->key = 1;
		data->state = LV_INDEV_STATE_REL;
	}
	if(pressKey == PRESS_LEFT)
	{
		data->key = 3;
		data->state = LV_INDEV_STATE_PR;
	}
	else
	{
		data->key = 3;
		data->state = LV_INDEV_STATE_REL;
	}
	if(pressKey == PRESS_RIGHT)
	{
		data->key = 4;
		data->state = LV_INDEV_STATE_PR;
	}
	else
	{
		data->key = 4;
		data->state = LV_INDEV_STATE_REL;
	}
	if(pressKey == PRESS_ENTER)
	{
		data->key = 5;
		data->state = LV_INDEV_STATE_PR;
	}
	else
	{
		data->key = 5;
		data->state = LV_INDEV_STATE_REL;
	}
	
  return false;
	
}