#include "ESPino32CAM.h"
#include "ESPino32CAM_QRCode.h"
ESPino32CAM cam;
ESPino32QRCode qr;
#define RXD2 2
#define TXD2 15
#define CAM_SER Serial2
camera_fb_t *fb;
uint8_t len[4];
qrResoult res;
void setup() {
  Serial.begin(2000000);
  Serial.println("Hello 123");
  CAM_SER.begin(1000000, SERIAL_8N1, RXD2, TXD2);
  if (cam.init() != ESP_OK)
  {
    cam.printDebug(F("Init Fail"));
    while (1);
  }
  qr.init(&cam);
  sensor_t *s = cam.sensor();
  s->set_framesize(s, FRAMESIZE_HQVGA);
  //s->set_hmirror (s,false);
  CAM_SER.print("Ready\n");
  Serial.print("Ready\n");
}
void loop() 
{
  
  if(CAM_SER.available())
  {
    String dat = CAM_SER.readStringUntil('\n');
    if(dat.indexOf("CAPTURE")!=-1)
    {
        if (fb)
        { 
          cam.clearMemory(fb);
        }
        fb = cam.capture();
       
       len[0] =  fb->len&0x000000FF;
       len[1] = (fb->len&0x0000FF00)>>8;
       len[2] = (fb->len&0x00FF0000)>>16;
       len[3] = (fb->len&0xFF000000)>>24;
       
        
        size_t s=0;
          s =  (len[0]&0xFF); 
          s |= (len[1]&0xFF)<<8;
          s |= (len[2]&0xFF)<<16;
          s |= (len[3]&0xFF)<<24;
       Serial.print(s);   Serial.print("  ");
       Serial.println(fb->len);
       CAM_SER.print("SIZE\n");
       CAM_SER.write(len[0]);
       CAM_SER.write(len[1]);
       CAM_SER.write(len[2]);
       CAM_SER.write(len[3]);
       CAM_SER.write(fb->buf, fb->len);    
    }
    if(dat.indexOf("READQR")!=-1)
    {
        res.status = false;
        res.length = 0;
        dl_matrix3du_t *image_rgb;
        CAM_SER.print("OK\n");
       if(cam.jpg2rgb(fb,&image_rgb))
       {
          cam.printDebug("\r\nQR Read:");
          res = qr.recognition(image_rgb);
          cam.clearMemory(image_rgb);
       }
    }
    if(dat.indexOf("GETQRDATA")!=-1)
    {
       CAM_SER.print("SIZE\n");
       len[0] = 0;
       len[1] = 0;
       len[2] = 0;
       len[3] = 0;
       
       if(res.status)
       {  
         len[0] =  res.length&0x000000FF;
         len[1] = (res.length&0x0000FF00)>>8;
         len[2] = (res.length&0x00FF0000)>>16;
         len[3] = (res.length&0xFF000000)>>24;

   
        CAM_SER.write(len[0]);
        CAM_SER.write(len[1]);
        CAM_SER.write(len[2]);
        CAM_SER.write(len[3]);
        CAM_SER.write((uint8_t*)res.payload.c_str(),res.length);   
       
       }
       else
       {
         
         CAM_SER.write(len[0]);
         CAM_SER.write(len[1]);
         CAM_SER.write(len[2]);
         CAM_SER.write(len[3]);
       }
    }
    
  }
  
}
