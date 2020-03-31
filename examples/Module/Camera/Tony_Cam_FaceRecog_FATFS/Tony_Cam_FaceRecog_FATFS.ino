#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"
Adafruit_ST7789 tft = Adafruit_ST7789(SLOT1,TFT_240_240_OLD);
TonyCam cam;

static lv_img_dsc_t dsc_Img_1 ;
static lv_obj_t * Img_1;
static lv_obj_t * label1; 

#define BUTTON 0
static mtmn_config_t mtmn_config = {0};
static face_id_list id_list = {0};
bool is_enrolling = false;

dl_matrix3du_t *rgb565;

String nameID[20]={"Name1","Name2","Name3"};

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
  
  //cam.faceIDInit(&id_list, 100, ENROLL_CONFIRM_TIMES);
  //deleteFaceLisFlash();
  cam.faceIDInitFlash(&id_list, 100, ENROLL_CONFIRM_TIMES);
  
  label1 = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_long_mode(label1, LV_LABEL_LONG_BREAK);    
  lv_label_set_recolor(label1, true);                     
  lv_label_set_align(label1, LV_LABEL_ALIGN_CENTER);   
  lv_obj_set_width(label1, 240);
  lv_obj_align(label1, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0,-10);   
 

  rgb565 = dl_matrix3du_alloc(1,240,176, 2);
 // while(1){}
}
void loop()
{
  String str;
  if (digitalRead(BUTTON) == 0)
  {
    is_enrolling = true;
  }
  camera_jpg *fb = cam.capture();
  if(fb->status==false)
  {
     Serial.println("Capture Error");
     return;
  }
  dl_matrix3du_t *rgb888;
  if (cam.jpg2rgb(fb, &rgb888))
  {
    box_array_t *net_boxes = cam.faceDetect(rgb888, &mtmn_config);
    if (net_boxes)
    {
       printtxt("Face Dettected");
       cam.drawFaceBoxes(rgb888, net_boxes, true);
       dl_matrix3du_t *aligned_face;
        if ( cam.alignFace(rgb888, net_boxes, &aligned_face) == ESP_OK)
        {
          if (is_enrolling)
          {
              int8_t left_sample_face = enroll_face(&id_list, aligned_face);
              str = "ID " + String(id_list.tail)+" Sample"+ String(ENROLL_CONFIRM_TIMES - left_sample_face);
              printtxt(str);
              if (left_sample_face == 0)
              {
                  is_enrolling = false;
                  str = "Enrolled Face ID: " + String(id_list.tail);
                  printtxt(str);
                  cam.writeFaceListToFlash(&id_list);
              }
              if (left_sample_face == -2)
              {
                  is_enrolling = false;
                  str = "Enrolled Fail" + String(id_list.tail);
                  printtxt(str);
              }
          }
          else
          {
              int matched_id = cam.recognizeFace(&id_list, aligned_face);
              if (matched_id >= 0)
              {
                  str = "Hello " + nameID[matched_id];
                  printtxt(str);
              }
              else
              {
                  str = "Intruder Alert!";
                  printtxt(str);
              }
          }
          cam.clearMemory(aligned_face);
          //////////////////////////////////
        }
        else
          printtxt(" ");
       cam.clearMemory(net_boxes);
    }
    else
    {
      printtxt("Not Detect");
    }
    cam.rgb888to565_2(rgb565,rgb888);
    cam.clearMemory(rgb888);
    showPic(rgb565);
  }
  lv_task_handler();
  delay(5);
  //cam.clearMemory(rgb565);
}
void showPic(dl_matrix3du_t *pic)
{
  if((pic->w!=240)||(pic->h!=176))
  {
    Serial.println("Error size");
    return;
  }
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
