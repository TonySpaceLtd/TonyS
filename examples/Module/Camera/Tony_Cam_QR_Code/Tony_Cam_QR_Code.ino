#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"
Adafruit_ST7789 tft = Adafruit_ST7789(SLOT1,TFT_240_240_OLD);
TonyCam cam;
TonyQRCode qr;
#define BUTTON_QR 0

static lv_img_dsc_t dsc_Img_1 ;
static lv_obj_t * Img_1;
static lv_obj_t * label1; 
void setup() 
{
  Serial.begin(115200); 
  Tony.begin();
  Serial.println(F("Initialized"));
  cam.begin(SLOT5);
  lvglInit(&tft);
  qr.init(&cam);

  label1 = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_long_mode(label1, LV_LABEL_LONG_BREAK);    
  lv_label_set_recolor(label1, true);                     
  lv_label_set_align(label1, LV_LABEL_ALIGN_CENTER);   
  lv_obj_set_width(label1, 240);
  lv_obj_align(label1, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0,-10);    
}
void loop()
{
  camera_jpg *fb = cam.capture();
  if(fb->status==false)
  {
     Serial.println("Capture Error");
     return;
  }
  dl_matrix3du_t *rgb888;
  dl_matrix3du_t *rgb565;
  if (cam.jpg2rgb(fb, &rgb888))
  {
        Serial.printf("\r\nQR Read:");
        qrResoult res = qr.recognition(rgb888);
       if(res.status)
       {
          Serial.printf("");
          Serial.printf("Version: %d\r\n", res.version);
          Serial.printf("ECC level: %c\r\n",res.eccLevel);
          Serial.printf("Mask: %d\r\n", res.mask);
          Serial.println("Data type: "+ qr.dataType(res.dataType));
          Serial.printf("Length: %d\r\n",res.length);
          Serial.println("Payload: "+res.payload);
          String str = "Payload: "+res.payload;
          printtxt(str);
       }
       else
       {
          Serial.println("FAIL");
           printtxt("QR Code not Found");
       }
    cam.rgb888to565(&rgb565,rgb888);
    cam.clearMemory(rgb888);
    showPic(rgb565);
  }
  lv_task_handler();
  delay(5);
  cam.clearMemory(rgb565);
}
void showPic(dl_matrix3du_t *pic)
{
   dsc_Img_1.header.always_zero = 0;
   dsc_Img_1.header.w = pic->w;
   dsc_Img_1.header.h = pic->h;
   dsc_Img_1.data_size = (pic->w * pic->h) * (LV_COLOR_SIZE / 8);
   dsc_Img_1.header.cf = LV_IMG_CF_TRUE_COLOR;
   dsc_Img_1.data = pic->item;
 
  if (Img_1 == NULL)
  {
    Img_1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(Img_1, &dsc_Img_1);
    lv_obj_align(Img_1, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_auto_size(Img_1,false);
  }
  lv_img_set_src(Img_1, &dsc_Img_1);
}
void printtxt(String str)
{
  lv_label_set_text(label1,str.c_str());
}
