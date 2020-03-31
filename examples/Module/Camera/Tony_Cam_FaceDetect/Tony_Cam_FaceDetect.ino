#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"
Adafruit_ST7789 tft = Adafruit_ST7789(SLOT1,TFT_240_240_OLD);
TonyCam cam;

static lv_img_dsc_t dsc_Img_1 ;
static lv_obj_t * Img_1;
static mtmn_config_t mtmn_config = {0};
void setup() 
{
  Serial.begin(115200); 
  Tony.begin();
  Serial.println(F("Initialized"));
  cam.begin(SLOT5);
  lvglInit(&tft);
  mtmn_config.type = FAST;
  mtmn_config.min_face = 80;
  mtmn_config.pyramid = 0.707;
  mtmn_config.pyramid_times = 4;
  mtmn_config.p_threshold.score = 0.6;
  mtmn_config.p_threshold.nms = 0.7;
  mtmn_config.p_threshold.candidate_number = 20;
  mtmn_config.r_threshold.score = 0.7;
  mtmn_config.r_threshold.nms = 0.7;
  mtmn_config.r_threshold.candidate_number = 10;
  mtmn_config.o_threshold.score = 0.7;
  mtmn_config.o_threshold.nms = 0.7;
  mtmn_config.o_threshold.candidate_number = 1;


  
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
    box_array_t *net_boxes = cam.faceDetect(rgb888, &mtmn_config);
    if (net_boxes)
    {
       cam.drawFaceBoxes(rgb888, net_boxes, true);
       cam.clearMemory(net_boxes);
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
