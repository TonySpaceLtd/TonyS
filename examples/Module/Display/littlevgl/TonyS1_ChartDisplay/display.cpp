#include "display.h"

static void header_create(void);
static lv_obj_t * header;
static lv_obj_t * gauge;
static lv_obj_t * clock_ ;
static lv_obj_t * label1; 
lv_obj_t * chart;
lv_chart_series_t * tempSHT ;
lv_chart_series_t * tempRTC ;
lv_chart_series_t * humidity;

void createDisplay(void)
{
   lv_theme_t *themes = lv_theme_alien_init(20, NULL);
   lv_theme_set_current(themes);
   lv_obj_t * scr = lv_cont_create(NULL, NULL);
   lv_disp_load_scr(scr);
   headerCreate();
   chartCreate();
   labelCreate();
}
void headerCreate(void)
{
    header = lv_cont_create(lv_disp_get_scr_act(NULL), NULL);
    lv_obj_set_width(header, lv_disp_get_hor_res(NULL));
    lv_obj_t * sym = lv_label_create(header, NULL);
    lv_label_set_text(sym, LV_SYMBOL_GPS LV_SYMBOL_WIFI LV_SYMBOL_BLUETOOTH LV_SYMBOL_VOLUME_MAX);
    lv_obj_align(sym, NULL, LV_ALIGN_IN_RIGHT_MID, -LV_DPI/10, 0);
    
    clock_ = lv_label_create(header, NULL);
    lv_label_set_text(clock_ , "0:0");
    lv_obj_align(clock_ , NULL, LV_ALIGN_IN_LEFT_MID, LV_DPI/10, 0);
    lv_cont_set_fit2(header, LV_FIT_NONE, LV_FIT_TIGHT);   /*Let the height set automatically*/
    lv_obj_set_pos(header, 0, 0);
}
void showTime(String t)
{
  lv_label_set_text(clock_ ,t.c_str());
}
void chartCreate(void)
{
    /*Create a chart*/
    chart = lv_chart_create(lv_scr_act(), NULL);
    lv_obj_set_size(chart, 200, 120);
    lv_obj_align(chart, NULL, LV_ALIGN_CENTER, 0, -10);
    lv_chart_set_type(chart, LV_CHART_TYPE_POINT | LV_CHART_TYPE_LINE);   /*Show lines and points too*/
    lv_chart_set_series_opa(chart, LV_OPA_70);                            /*Opacity of the data series*/
    lv_chart_set_series_width(chart, 4);                                  /*Line width and point radious*/

    lv_chart_set_range(chart, 0, 100);

   tempSHT = lv_chart_add_series(chart, LV_COLOR_RED);
   tempRTC = lv_chart_add_series(chart, LV_COLOR_YELLOW);
   humidity = lv_chart_add_series(chart, LV_COLOR_BLUE);

    /*Set the next points on 'dl1'*/
  //  lv_chart_set_next(chart, ser1, 10);
  

    /*Directly set points on 'dl2'*/
   // ser2->points[0] = 90;
    lv_chart_refresh(chart); /*Required after direct set*/
}
void setTempRTC(float t)
{
   lv_chart_set_next(chart, tempRTC,t);
}
void setTempSHT(float t)
{
   lv_chart_set_next(chart, tempSHT,t);
}
void setHumidity(float h)
{
   lv_chart_set_next(chart, humidity,h);
}
void updatechart(void)
{
   lv_chart_refresh(chart);
}
void labelCreate(void)
{
    label1 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_long_mode(label1, LV_LABEL_LONG_BREAK);     /*Break the long lines*/
    lv_label_set_recolor(label1, true);                      /*Enable re-coloring by commands in the text*/
    lv_label_set_align(label1, LV_LABEL_ALIGN_CENTER);       /*Center aligned lines*/
   // lv_label_set_text(label1, "#000080 Re-color# #0000ff words# #6666ff of a# label "
     //                         "and  wrap long text automatically.");
    lv_obj_set_width(label1, 240);
    lv_obj_align(label1, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0,-40);    
}
void setlabel(String str)
{
    lv_label_set_text(label1,str.c_str());
}
