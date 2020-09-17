#include "display.h"
#include "TonyS_X1_ExternalModule.h"

lv_obj_t * header;
lv_obj_t * clock_ ;

lv_obj_t * cont_Temp;
lv_obj_t * cont_Humi;

lv_obj_t * lmeter;
lv_obj_t * label_gage;
lv_obj_t * lmeter2;
lv_obj_t * label_gage2;

lv_obj_t * chart;
lv_chart_series_t * ser1;
lv_chart_series_t * ser2;

lv_obj_t * label1;
lv_obj_t * mbox1;
lv_obj_t *mbox,*info;
lv_obj_t * sym;
lv_obj_t *cntDown;
void createDisplay()
{
  lv_theme_t *themes = lv_theme_alien_init(20, NULL);
  lv_theme_set_current(themes);
  lv_obj_t * scr = lv_cont_create(NULL, NULL);
  lv_disp_load_scr(scr);
  headerCreate();
  gaugeTemp();
  gaugeHumi();
  charGraph();
 // msgBox();
}
void headerCreate(void)
{
  header = lv_cont_create(lv_disp_get_scr_act(NULL), NULL);
  lv_obj_set_width(header, lv_disp_get_hor_res(NULL));
  
  sym = lv_label_create(header, NULL);
  lv_label_set_text(sym," ");
  lv_obj_align(sym, NULL, LV_ALIGN_IN_RIGHT_MID,-20, 0);

  clock_ = lv_label_create(header, NULL);
  lv_label_set_text(clock_ , "TONY SPACE");
  lv_obj_align(clock_ , NULL, LV_ALIGN_CENTER, 0, 0);
  lv_cont_set_fit2(header, LV_FIT_NONE, LV_FIT_TIGHT);   /*Let the height set automatically*/
  lv_obj_set_pos(header, 0, 0);
}
void gaugeTemp()
{
  cont_Temp = lv_cont_create(lv_disp_get_scr_act(NULL), NULL);
  lv_obj_set_size(cont_Temp, 110, 110);
  lv_obj_align(cont_Temp, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 50);


  static lv_style_t style_lmeter;
  lv_style_copy(&style_lmeter, &lv_style_pretty_color);
  style_lmeter.line.width = 2;
  style_lmeter.line.color = LV_COLOR_SILVER;
  style_lmeter.body.main_color = LV_COLOR_YELLOW;//lv_color_hex(0x500606);         /*Light blue*/
  style_lmeter.body.grad_color = LV_COLOR_RED;//lv_color_hex(0xF20909);         /*Dark blue*/
  style_lmeter.body.padding.left = 14;                           /*Line length*/

  /*Create a line meter */
  lmeter = lv_lmeter_create(cont_Temp, NULL);
  lv_lmeter_set_range(lmeter, 0, 100);                   /*Set the range*/
  lv_lmeter_set_value(lmeter, 0);                       /*Set the current value*/
  lv_lmeter_set_scale(lmeter, 240, 31);                  /*Set the angle and number of lines*/
  lv_lmeter_set_style(lmeter, LV_LMETER_STYLE_MAIN, &style_lmeter);           /*Apply the new style*/
  lv_obj_set_size(lmeter, 100, 100);
  lv_obj_align(lmeter, NULL, LV_ALIGN_CENTER, 0, 0);

  static lv_style_t style_txt;
  lv_style_copy(&style_txt, &lv_style_pretty_color);

  label_gage = lv_label_create(cont_Temp, NULL);
  lv_label_set_style(label_gage, LV_LABEL_STYLE_MAIN, &style_txt);
  lv_label_set_recolor(label_gage, true);
  lv_label_set_text(label_gage, "00.00*C");
  lv_obj_align(label_gage, NULL, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t * label_gage_name = lv_label_create(cont_Temp, NULL);
  lv_label_set_style(label_gage_name, LV_LABEL_STYLE_MAIN, &style_txt);
  lv_label_set_text(label_gage_name, "Temperature");
  lv_obj_align(label_gage_name, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -5);

}
void gaugeHumi()
{
  cont_Humi = lv_cont_create(lv_disp_get_scr_act(NULL), NULL);
  lv_obj_set_size(cont_Humi, 110, 110);
  lv_obj_align(cont_Humi, NULL, LV_ALIGN_IN_TOP_RIGHT, -5, 50);

  static lv_style_t style_lmeter;
  lv_style_copy(&style_lmeter, &lv_style_pretty_color);
  style_lmeter.line.width = 2;
  style_lmeter.line.color = LV_COLOR_SILVER;
  style_lmeter.body.main_color = LV_COLOR_BLUE;//lv_color_hex(0x500606);         /*Light blue*/
  style_lmeter.body.grad_color = LV_COLOR_RED;//lv_color_hex(0xF20909);         /*Dark blue*/
  style_lmeter.body.padding.left = 14;                           /*Line length*/

  /*Create a line meter */
  lmeter2 = lv_lmeter_create(cont_Humi, NULL);
  lv_lmeter_set_range(lmeter2, 0, 100);                   /*Set the range*/
  lv_lmeter_set_value(lmeter2, 0);                       /*Set the current value*/
  lv_lmeter_set_scale(lmeter2, 240, 31);                  /*Set the angle and number of lines*/
  lv_lmeter_set_style(lmeter2, LV_LMETER_STYLE_MAIN, &style_lmeter);           /*Apply the new style*/
  lv_obj_set_size(lmeter2, 100, 100);
  lv_obj_align(lmeter2, NULL, LV_ALIGN_CENTER, 0, 0);

  static lv_style_t style_txt;
  lv_style_copy(&style_txt, &lv_style_pretty_color);

  label_gage2 = lv_label_create(cont_Humi, NULL);
  lv_label_set_style(label_gage2, LV_LABEL_STYLE_MAIN, &style_txt);
  lv_label_set_recolor(label_gage2, true);
  lv_label_set_text(label_gage2, "00.00%");
  lv_obj_align(label_gage2, NULL, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t * label_gage_name = lv_label_create(cont_Humi, NULL);
  lv_label_set_style(label_gage_name, LV_LABEL_STYLE_MAIN, &style_txt);
  lv_label_set_text(label_gage_name, "Humidity");
  lv_obj_align(label_gage_name, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -5);
}
void updateTempHumi(float t, float h)
{
  String tt = String(t, 2) + "*C";
  lv_label_set_text(label_gage, tt.c_str());
  lv_lmeter_set_value(lmeter, t);
  tt = String(h, 2) + "%";
  lv_label_set_text(label_gage2, tt.c_str());
  lv_lmeter_set_value(lmeter2, t);
  updateChart(t,h);
  String str = "#FF0000 Temp "+String(t,2)+" C#  "+"#0000FF Humi "+String(h,2)+"%#";
  setlabel(str);
     
}

void charGraph()
{
  chart = lv_chart_create(lv_scr_act(), NULL);
  lv_obj_set_size(chart, 230, 120);
  lv_obj_align(chart, NULL, LV_ALIGN_CENTER, 0,70);
  lv_chart_set_type(chart, LV_CHART_TYPE_POINT | LV_CHART_TYPE_LINE);   /*Show lines and points too*/
  lv_chart_set_series_opa(chart, LV_OPA_70);                            /*Opacity of the data series*/
  lv_chart_set_series_width(chart, 4);                                  /*Line width and point radious*/

  lv_chart_set_range(chart, 0, 100);

  /*Add two data series*/
  ser1 = lv_chart_add_series(chart, LV_COLOR_RED);
  ser2 = lv_chart_add_series(chart, LV_COLOR_BLUE);

   label1 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_long_mode(label1, LV_LABEL_LONG_BREAK);     /*Break the long lines*/
    lv_label_set_recolor(label1, true);                      /*Enable re-coloring by commands in the text*/
   // lv_label_set_align(label1, LV_LABEL_ALIGN_CENTER);       /*Center aligned lines*/
   // lv_label_set_text(label1, "#000080 Re-color# #0000ff words# #6666ff of a# label "
     //                         "and  wrap long text automatically.");
    lv_obj_set_width(label1, 240);
    lv_obj_align(label1, NULL, LV_ALIGN_IN_BOTTOM_MID, 5,-5);    
    
}
void updateChart(float t,float h)
{
   lv_chart_set_next(chart, ser1, t);
   lv_chart_set_next(chart, ser2, h);
}
void setlabel(String str)
{
    lv_label_set_text(label1,str.c_str());
}

void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        printf("Button: %s\n", lv_mbox_get_active_btn_text(obj));
    }
}

void msgBox()
{
   static lv_style_t style_shadow;
   lv_style_copy(&style_shadow, &lv_style_pretty);
   style_shadow.body.shadow.width = 6;
   style_shadow.body.radius = LV_RADIUS_CIRCLE;
   style_shadow.body.opa = LV_OPA_70;
    
  mbox1 = lv_mbox_create(lv_scr_act(), NULL);
  lv_obj_set_style(mbox1, &style_shadow);
  lv_mbox_set_text(mbox1, "A message box with two buttons.");
  lv_obj_set_width(mbox1, 200);
  lv_obj_set_event_cb(mbox1, event_handler);
  lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0); /*Align to the corner*/
}
void showMsgBox(String msg)
{
  static lv_style_t style_shadow;
  lv_style_copy(&style_shadow, &lv_style_pretty_color);
  style_shadow.body.shadow.width = 6;
 // style_shadow.body.radius = LV_RADIUS_CIRCLE;
  style_shadow.body.opa = LV_OPA_40;
   
  mbox1 = lv_mbox_create(lv_scr_act(), NULL);
  lv_obj_set_style(mbox1, &style_shadow);
  lv_mbox_set_text(mbox1, msg.c_str());
  lv_obj_set_width(mbox1, 200);
  lv_obj_set_event_cb(mbox1, event_handler);
  lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0); /*Align to the corner*/
  
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_time(&a, 500, 0);
  lv_anim_set_values(&a, LV_OPA_TRANSP, LV_OPA_COVER);
  lv_anim_set_exec_cb(&a, lv_scr_act(), (lv_anim_exec_xcb_t)lv_obj_set_opa_scale);
  lv_anim_create(&a);

  static lv_style_t style;
  lv_style_copy(&style, &lv_style_plain);
  style.line.width = 10;                         /*10 px thick arc*/
  style.line.color = LV_COLOR_YELLOW;//lv_color_hex3(0x258);       /*Blueish arc color*/

  style.body.border.color = lv_color_hex3(0xBBB); /*Gray background color*/
  style.body.border.width = 10;
  style.body.padding.left = 0;

    /*Create a Preloader object*/
  lv_obj_t * preload = lv_preload_create(mbox1, NULL);
  lv_obj_set_size(preload, 50, 50);
  lv_obj_align(preload, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_preload_set_style(preload, LV_PRELOAD_STYLE_MAIN, &style);

  cntDown = lv_label_create(mbox1, NULL);
  lv_label_set_text(cntDown," ");
  lv_obj_align(cntDown, NULL,LV_ALIGN_CENTER,0, 0);
  
    
    
}
void closeMsgBox()
{
  lv_mbox_start_auto_close(mbox1, 0);
}
void headText(String t)
{
  lv_label_set_text(clock_ ,t.c_str());
}
void wifisymbol(uint8_t b)
{
  if(b==0)
  lv_label_set_text(sym," ");
  else
  lv_label_set_text(sym,LV_SYMBOL_WIFI);
}
void updateCntDown(uint8_t cnt)
{
   lv_label_set_text(cntDown ,String(cnt).c_str());
}
void updateCntDown(String cnt)
{
   lv_label_set_text(cntDown ,cnt.c_str());
}
