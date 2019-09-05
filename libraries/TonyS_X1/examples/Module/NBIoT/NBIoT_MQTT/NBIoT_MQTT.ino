#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"
Adafruit_SHT31 sht31 = Adafruit_SHT31();
NBIoT nb;
NBIoT_MQTT mqtt;
#define SERVER_IP   "m16.cloudmqtt.com"
#define SERVER_PORT 18568
#define USER  "XXXXXXX"
#define PASS  "XXXXXXX"
#define CLIENT_ID "NBIoTNode"

unsigned long previousMillis = 0;
const long interval = 5000;
void setup()
{
  Serial.begin(9600);
  Serial.println("TonyS X1");
  Tony.begin();
  Serial.println("\r\nNBIoT Setting");
  nb.begin(SLOT2);
  Serial.println("NBIoT Ready");
  Serial.print("SIM(S/N) :");
  Serial.println(nb.getSIMSerial());
  Serial.print("IMEI :");
  Serial.println(nb.getIMEI());
  Serial.print("IMSI :");
  Serial.println(nb.getIMSI());
  Serial.print("Network Status:");
  while (!nb.getNetworkStatus())
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("OK");
  Serial.print("Signal :");
  Serial.println(nb.getSignal());
  Serial.print("Signal :");
  Serial.print(nb.getSignaldBm());
  Serial.println("dBm");
  Serial.print("Signal Level :");
  Serial.println(nb.getSignalLevel());
  Serial.print("Device IP :");
  Serial.println(nb.getDeviceIP());
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
  mqtt.callback = mqttCallBack; 
}
uint8_t cnt=0;
void loop()
{
  if(!mqtt.connected())
  {
    connectServer();
  }
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    static uint8_t cnt=0; 
    Serial.print("Publish Topic : ");
    if(mqtt.publish("counter",String(cnt++)))
      Serial.println("OK");
    else
      Serial.println("Error");
    previousMillis = currentMillis;
  }
  mqtt.eventTask();
  
}
void mqttCallBack(String topic ,char *payload,size_t length)
{
  Serial.println("<-----Received messages----->");
  Serial.println("Topic--> "+topic);
  Serial.print("Payload-->");
  Serial.write((uint8_t*)payload,length);
  Serial.println();
  Serial.println("<-------------------------->");
}
void connectServer()
{
  Serial.println("Open Server...");
  if(mqtt.open(SERVER_IP,SERVER_PORT))
  {
    Serial.print("Connect...");
    if(mqtt.connect(CLIENT_ID,USER,PASS))
    {
      Serial.println("OK");
      subscribe();
    }
  }
}
void subscribe()
{
  String topic[3]={"test1","test2","test3"};
  for(uint8_t i=0;i<3;i++)
  {
    Serial.print("Subscribe Topic "+ topic[i]+" : ");
    if(mqtt.subScribe(topic[i]))
      Serial.println("OK");
    else
    {
      Serial.println("Error");
      break;
    }   
  }
}
void sendmqtt()
{
   static uint8_t cnt=0; 
   Serial.print("Publish Topic : ");
   if(mqtt.publish("Serial",String(cnt++)))
   Serial.println("OK");
  else
  Serial.println("Error");
}
