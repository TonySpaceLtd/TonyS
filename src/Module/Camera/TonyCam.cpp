#include "TonyCam.h"

HardwareSerial camSer(NULL);

camera_jpg img;	


TonyCam::TonyCam()
{
	
}
void TonyCam::begin(uint8_t slot,unsigned long baudrate )
{
	camSer = Tony.SerialBegin(slot,baudrate);
	img.buf = (uint8_t*)ps_malloc(320*240*3);
	
	
}

camera_jpg* TonyCam::capture()
{
	img.width = 0;
	img.height = 0;
	img.len=0;
	img.status=false;
	
	camSer.print("CAPTURE\n");
	unsigned long pvt = millis();
	do
	{
		 while(camSer.available())
		{
			String  str =  camSer.readStringUntil('\n');
			if(str.indexOf("SIZE")!=-1)
			{ 
				uint8_t len[4];
				camSer.readBytes(len,4);
				size_t s=0;
				s =  (len[0]&0xFF); 
				s |= (len[1]&0xFF)<<8;
				s |= (len[2]&0xFF)<<16;
				s |= (len[3]&0xFF)<<24;
				//Serial.println("FileSize-->"+String(s));
				if(s>=320*240*3)
					return &img; 
				camSer.readBytes(img.buf,s);
				for(size_t i=1;i<s;i++)
				{
					if(img.buf[i-1]==0xFF && img.buf[i]==0xC0)
					{
						uint16_t w,h;
						h  =  img.buf[i+4]<<8;
						h |=  img.buf[i+5];
						w  =  img.buf[i+6]<<8;
						w |=  img.buf[i+7];
						//Serial.printf("res = %d,%d\r\n",w,h);
						img.width =  w;
						img.height = h;
						img.len=s;
						img.status=true;
						break;
					}
				}
				return &img;  
			}
		}
	}
	while((millis()-pvt)<1000);
	Serial.println("Capture Time out");
	return &img;  
}
void TonyCam::findQR()
{
	camSer.print("READQR\n");
	unsigned long pvt = millis();
	do
	{
		 while(camSer.available())
		{
			String  str =  camSer.readStringUntil('\n');
			if(str.indexOf("OK")!=-1)
			{ 
				return ;  
			}
		}
	}
	while((millis()-pvt)<5);
	Serial.println("ReadQR Time out");
	
}
String TonyCam::readQR()
{
	String out ;
	out[0] = 0;
	camSer.print("GETQRDATA\n");
	unsigned long pvt = millis();
	do
	{
		 while(camSer.available())
		{
			String  str =  camSer.readStringUntil('\n');
			if(str.indexOf("SIZE")!=-1)
			{ 
				uint8_t len[4];
				camSer.readBytes(len,4);
				size_t s=0;
				s =  (len[0]&0xFF); 
				s |= (len[1]&0xFF)<<8;
				s |= (len[2]&0xFF)<<16;
				s |= (len[3]&0xFF)<<24;
				if(s==0)
					return(out);
				uint8_t dataQR[s+1];
				camSer.readBytes(dataQR,s);
				//Serial.write(dataQR,s);
				dataQR[s]=0;
				String out = (char*)dataQR;
				return(out) ;  
			}
		}
	}
	while((millis()-pvt)<5000);
	Serial.println("QR Time out");
	return(out);
	
}
bool TonyCam::jpg2rgb(camera_jpg *fb, dl_matrix3du_t **image_matrix)
{
	*image_matrix = dl_matrix3du_alloc(1, fb->width, fb->height, 3);
	if (!*image_matrix)
	{
		return false;
	}
	 bool s = fmt2rgb888(fb->buf, fb->len, PIXFORMAT_JPEG, (*image_matrix)->item);
	if (!s)
	{
		dl_matrix3du_free(*image_matrix);
		return false;
	}
	return true;
}
uint16_t TonyCam::color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void TonyCam::rgb888to565(dl_matrix3du_t **image565,dl_matrix3du_t *image, bool RGB, bool swap)
{
  *image565 = dl_matrix3du_alloc(1, image->w, image->h, 2);
  if (!*image565)
  {
    // printDebug("dl_matrix3du_alloc failed");
    return;
  }
  size_t n = 0;
  for (size_t i = 0; i < (image->w * image->h * 3); i += 3)
  {

    uint16_t color;
    if (RGB)
      color = color565(image->item[i], image->item[i + 1], image->item[i + 2]);
    else
      color = color565(image->item[i + 2], image->item[i + 1], image->item[i]);

    uint8_t hi = color >> 8, lo = color;
    if (swap)
    {
      (*image565)->item[n++] = hi;
      (*image565)->item[n++] = lo;
    }
    else
    {
      (*image565)->item[n++] = lo;
      (*image565)->item[n++] = hi;
    }
  }
 
}
void TonyCam::rgb888to565_2(dl_matrix3du_t *image565,dl_matrix3du_t *image, bool RGB, bool swap)
{
  size_t n = 0;
  for (size_t i = 0; i < (image->w * image->h * 3); i += 3)
  {

    uint16_t color;
    if (RGB)
      color = color565(image->item[i], image->item[i + 1], image->item[i + 2]);
    else
      color = color565(image->item[i + 2], image->item[i + 1], image->item[i]);

    uint8_t hi = color >> 8, lo = color;
    if (swap)
    {
      (image565)->item[n++] = hi;
      (image565)->item[n++] = lo;
    }
    else
    {
      (image565)->item[n++] = lo;
      (image565)->item[n++] = hi;
    }
  }
 
}


void TonyCam::clearMemory(camera_jpg *memory)
{
	img.width = 0;
	img.height = 0;
	img.len=0;
	img.status=false;
	free(img.buf);
}
void TonyCam::clearMemory(dl_matrix3du_t *memory)
{
	if(memory != NULL)
	{
		dl_matrix3du_free(memory);
	}
	else
		Serial.println("clear fail");
}
void TonyCam::clearMemory(uint8_t *memory)
{
	free(memory);
}
void TonyCam::clearMemory(box_array_t *memory)
{
	free(memory->score);
    free(memory->box);
    free(memory->landmark);
    free(memory);
}
box_array_t  * TonyCam::faceDetect(dl_matrix3du_t *image_matrix,mtmn_config_t *config)
{
	return face_detect(image_matrix,config);
}
void TonyCam:: drawFaceBoxes(dl_matrix3du_t *image_matrix, box_array_t *boxes,bool landmask)
{
    int x, y, w, h, i;
    uint32_t color = FACE_COLOR_YELLOW;
    fb_data_t fb;
    fb.width = image_matrix->w;
    fb.height = image_matrix->h;
    fb.data = image_matrix->item;
    fb.bytes_per_pixel = 3;
    fb.format = FB_BGR888;
    for (i = 0; i < boxes->len; i++){
        // rectangle box
        x = (int)boxes->box[i].box_p[0];
        y = (int)boxes->box[i].box_p[1];
        w = (int)boxes->box[i].box_p[2] - x + 1;
        h = (int)boxes->box[i].box_p[3] - y + 1;
	    // Add draw Box  >width and height
     	if((x+w)>fb.width)
			w = (fb.width - x);
		if((y+h)>fb.height)
			h = (fb.height - y);
		rbgDrawBox(&fb,color,x,y,w,h);
		if(landmask)
		{
			int x0, y0, j;
			for (j = 0; j < 10; j+=2) {
				x0 = (int)boxes->landmark[i].landmark_p[j];
				y0 = (int)boxes->landmark[i].landmark_p[j+1];
				fb_gfx_fillRect(&fb, x0, y0, 3, 3, color);
			}
		}
	}
}
void TonyCam:: rbgDrawBox(fb_data_t *fb,uint32_t color,int x,int y,int w,int h)
{
	
	 fb_gfx_drawFastHLine(fb, x, y, w, color);
     fb_gfx_drawFastHLine(fb, x, y+h-1, w, color);
     fb_gfx_drawFastVLine(fb, x, y, h, color);
     fb_gfx_drawFastVLine(fb, x+w-1, y, h, color);
}
int8_t  TonyCam::alignFace(dl_matrix3du_t *image_matrix, box_array_t *net_boxes, dl_matrix3du_t **aligned_face)
{
	*aligned_face = dl_matrix3du_alloc(1, FACE_WIDTH, FACE_HEIGHT, 3);
	if(!aligned_face)
	{
        return ESP_FAIL;
    }
	return align_face(net_boxes, image_matrix, *aligned_face);
}
int8_t TonyCam::recognizeFace(face_id_list *l,dl_matrix3du_t *algined_face)
{
	return recognize_face(l,algined_face);
}
void  TonyCam::faceIDInit(face_id_list *l, uint8_t size, uint8_t confirm_times)
{
	face_id_init(l, size, confirm_times);
}
void  TonyCam::faceIDInitFlash(face_id_list *l, uint8_t size, uint8_t confirm_times)
{
	initFatFS();
	
	if(availableFile(FACE_LIST_PATH))
	{
		size_t s = getFileSize(FACE_LIST_PATH);
		if(s==0)
		{	
			deleteFaceLisFlash();
			face_id_init(l, size, confirm_times);
			return;
		}
		
		readFaceListFromFlash(l);
	}
	else
		face_id_init(l, size, confirm_times);
	
}
bool TonyCam::initFatFS()
{
	if(!FFat.begin())
	{
       Serial.println("FFat Mount Failed");
       Serial.println("FFat Format");
       FFat.format();
       Serial.println("Reset");
       ESP.restart();
	}
    Serial.println("FFat Mount OK");
	initialFile();
	return true;
}
void TonyCam::initialFile()
{
	Serial.print("Face list File: ");
	if(availableFile(FACE_LIST_PATH))
		Serial.println("Found");
	else
	{
		Serial.println("Not Found");
		Serial.println("Create File: "+String(FACE_LIST_PATH));
		File newfile = FFat.open(FACE_LIST_PATH, FILE_WRITE);
		if(!newfile)
		{
			Serial.println("- failed to open file for writing");
			return;
		}
		newfile.close();
    }
}
size_t TonyCam::getFileSize(const char * filename)
{
	 File file = FFat.open(filename);
	 if(!file || file.isDirectory())
	 {
        Serial.println("- failed to open file for reading");
        return 0 ;
     }
	 size_t s = file.size();
	 return s;
	
	
}
bool TonyCam::availableFile(const char * filename)
{
	bool found = false;
	File root = FFat.open("/");
	if(!root){
        Serial.println("- failed to open directory");
        return false;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return false;
    }
	File file = root.openNextFile();
	while(file)
	{
       /* Serial.print("  FILE: ");
        Serial.print(file.name());
        Serial.print("\tSIZE: ");
        Serial.println(file.size());	
		*/
		if(String(file.name())==FACE_LIST_PATH)
		{	
			found = true;
			break;
		}
        file = root.openNextFile();
    }
	file.close();
	return found;
	
}
bool TonyCam::writeFaceListToFlash(face_id_list *l)
{
	File file = FFat.open(FACE_LIST_PATH, FILE_WRITE);
	if(!file)
	{		
        Serial.println("- failed to open file for writing");
		return false;
    }
	file.write(l->head); 
	file.write(l->tail); 
	file.write(l->count); 
	file.write(l->size); 
	file.write(l->confirm_times);
	const int block_len = FACE_ID_SIZE * sizeof(float);
	for(uint8_t i =0;i<l->count;i++)
   {
     file.write((uint8_t*)l->id_list[i]->item,block_len);           
   }
	 file.close();
	 return true;
}
static inline void *dl_lib_calloc(int cnt, int size, int align)
{
    int total_size = cnt * size + align + sizeof(void *);
    void *res = malloc(total_size);
    if (NULL == res)
    {
#if CONFIG_SPIRAM_SUPPORT
        res = heap_caps_malloc(total_size, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
    }
    if (NULL == res)
    {
        printf("Item psram alloc failed. Size: %d x %d\n", cnt, size);
#else
        printf("Item alloc failed. Size: %d x %d\n", cnt, size);
#endif
        return NULL;
    }
    bzero(res, total_size);
    void **data = (void **)res + 1;
    void **aligned;
    if (align)
        aligned = (void **)(((size_t)data + (align - 1)) & -align);
    else
        aligned = data;

    aligned[-1] = res;
    return (void *)aligned;
}
bool TonyCam::readFaceListFromFlash(face_id_list *l)
{
	
	 File file = FFat.open(FACE_LIST_PATH);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return false;
    }
	
	l->head = 	file.read();
	l->tail = 	file.read();
    l->count =  file.read();
	l->size = 	file.read();
	l->confirm_times = file.read();
	l->id_list = (dl_matrix3d_t **)dl_lib_calloc(l->size, sizeof(dl_matrix3d_t *), 0);
	const int block_len = FACE_ID_SIZE * sizeof(float);
	 for(uint8_t i =0;i<l->count;i++)
	{
		l->id_list[i] = dl_matrix3d_alloc(1, 1, 1, FACE_ID_SIZE);
		file.read((uint8_t*)l->id_list[i]->item,block_len);           
	}
	file.close();
	return true;
}
bool TonyCam::deleteFaceLisFlash()
{
	if(FFat.remove(FACE_LIST_PATH))
	{
        Serial.println("- file deleted");
    } else {
         return false;
    }
	return true;
}
size_t TonyCam::grayScale(dl_matrix3du_t *image_888,dl_matrix3du_t **gray)
{
	return (grayScale_(image_888,gray));
}

size_t grayScale_(dl_matrix3du_t *image_888,dl_matrix3du_t **gray)
{
	*gray = dl_matrix3du_alloc(1,image_888->w,image_888->h,1);
	if (!*gray)
		return(0);
	(*gray)->w = image_888->w;
	(*gray)->h = image_888->h;
	(*gray)->c = 1;
	size_t len = image_888->w * image_888->h * image_888->c;
	size_t lengray=0;
	for(size_t i=0;i<len;i+=3)
	{
		uint8_t r = image_888->item[i];
		uint8_t g = image_888->item[i+1]; 
		uint8_t b = image_888->item[i+2];
		uint8_t m = max(r, g);
		m = max(m,b);
		uint8_t n = min(r, g);
		n = min(n,b);
		uint8_t gr = (m+n)/2;
		
		//uint8_t gr = (r*0.3)+(g*0.59)+(b*0.11);
		(*gray)->item[lengray]=gr;
		lengray++;	
	}
	return(lengray);
}

