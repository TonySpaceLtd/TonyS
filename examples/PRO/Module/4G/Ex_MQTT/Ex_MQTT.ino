#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"
#define SERVER      "m16.cloudmqtt.com"
#define PORT         18568
#define DeviceID    "4GModule"
#define USER        "xxxxxxx"
#define PASSWORD    "xxxxxxx"
EC25_MQTT mqtt;
unsigned long previousMillis = 0;
const long interval = 2000;

#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif

void setup() {
  Serial.begin(115200);
  Serial.println("Hello World");
  Tony.begin();
  delay(100);
  LTE.initial(SLOT3);
  Serial.println("Power On 4G Module Wait Ready...");
  LTE.powerOn(true);
  LTE.waitModuleReady(20);
  Serial.print("IMEI: ");
  Serial.println(LTE.getIMEI());
  Serial.print("CIMI: ");
  Serial.println(LTE.getCIMI());
  Serial.print("Network :");
  while (!LTE.getNetworkRegis())
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Ready");
  Serial.print("Access Tech: ");
  Serial.println(LTE.getAccessTech());
  Serial.print("Operator: ");
  Serial.println(LTE.getOperator());
  Serial.print("Signal: ");
  Serial.println(String(LTE.getSignaldBm()) + " dBm");
  Serial.print("Connect Internet: ");
  Serial.println(LTE.connectInternet() ? "OK" : "Fail");
  Serial.print("IP Address: ");
  Serial.println(LTE.getIPAddress());
  mqtt.init();
  mqtt.callback = callback;
}

void loop() {
  if (mqtt.connected())
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
      float temp = (temprature_sens_read() - 32) / 1.8;
      Serial.println("publish: " + String(temp, 2));
      mqtt.publish("Temp", String(temp, 2));
      previousMillis = currentMillis;
    }
  }
  else
    connectServer();
  mqtt.task();
}
void connectServer()
{
  Serial.print("\r\nConnect Server: ");
  mqtt.disConnect();
  if (mqtt.connect(SERVER, PORT, DeviceID, USER, PASSWORD))
  {
    Serial.println("OK");
    Serial.print("SubScribe LED1 : ");
    Serial.println(mqtt.subScribe("LED1", 1) ? "OK" : "FAIL");
  }
  else
    Serial.println("Fail");
}
void callback(String topic , uint8_t *payload, size_t length)
{
  Serial.println("#####################################");
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  Serial.write(payload, length);
  Serial.println("\r\n#####################################"); 
}
