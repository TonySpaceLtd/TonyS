#ifndef _DIAPLAY_TONY_
#define _DIAPLAY_TONY_
#include <Arduino.h>
void createDisplay();
void headerCreate(void);
void gaugeTemp();
void gaugeHumi();
void updateTempHumi(float t,float h);
void charGraph();
void updateChart(float t,float h);
void setlabel(String str);
void msgBox();
void showMsgBox(String msg);
void closeMsgBox();
void headText(String t);
void wifisymbol(uint8_t b);
void updateCntDown(uint8_t cnt);
void updateCntDown(String cnt);










#endif 
