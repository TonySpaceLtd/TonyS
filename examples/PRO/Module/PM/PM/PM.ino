#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

void setup()
{
  Serial.begin(115200);
  Tony.begin();       //-----  begin Main Library
  TonyPM.slot(SLOT1); //-----  Select Slot
  TonyPM.begin();     //-----  Begin PM's Library
}

void loop()
{
  // read the PM sensor
  TonyPM.read();
  if (TonyPM)
  { // successfull read
    // print formatted results
    Serial.printf("PM1.0 %2d, PM2.5 %2d, PM10 %2d [ug/m3]\n",
                  TonyPM.pm01, TonyPM.pm25, TonyPM.pm10);

    if (TonyPM.has_number_concentration())
      Serial.printf("N0.3 %4d, N0.5 %3d, N1.0 %2d, N2.5 %2d, N5.0 %2d, N10 %2d [#/100cc]\n",
                    TonyPM.n0p3, TonyPM.n0p5, TonyPM.n1p0, TonyPM.n2p5, TonyPM.n5p0, TonyPM.n10p0);

    if (TonyPM.has_temperature_humidity() || TonyPM.has_formaldehyde())
      Serial.printf("%5.1f °C, %5.1f %%rh, %5.2f mg/m3 HCHO\n",
                    TonyPM.temp, TonyPM.rhum, TonyPM.hcho);
  }
  else
  { // something went wrong
    switch (TonyPM.status)
    {
    case TonyPM.OK: // should never come here
      break;     // included to compile without warnings
    case TonyPM.ERROR_TIMEOUT:
      Serial.println(F(PMS_ERROR_TIMEOUT));
      break;
    case TonyPM.ERROR_MSG_UNKNOWN:
      Serial.println(F(PMS_ERROR_MSG_UNKNOWN));
      break;
    case TonyPM.ERROR_MSG_HEADER:
      Serial.println(F(PMS_ERROR_MSG_HEADER));
      break;
    case TonyPM.ERROR_MSG_BODY:
      Serial.println(F(PMS_ERROR_MSG_BODY));
      break;
    case TonyPM.ERROR_MSG_START:
      Serial.println(F(PMS_ERROR_MSG_START));
      break;
    case TonyPM.ERROR_MSG_LENGTH:
      Serial.println(F(PMS_ERROR_MSG_LENGTH));
      break;
    case TonyPM.ERROR_MSG_CKSUM:
      Serial.println(F(PMS_ERROR_MSG_CKSUM));
      break;
    case TonyPM.ERROR_PMS_TYPE:
      Serial.println(F(PMS_ERROR_PMS_TYPE));
      break;
    }
  }
  Serial.println();
  // wait for 10 seconds
  delay(10000);
}