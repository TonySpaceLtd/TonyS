#ifndef TonyS_X1_TonyCam_h
#define TonyS_X1_TonyCam_h


#include "Arduino.h"
#include "TonyS_X1.h"
#include "esp_camera.h" // camera
#include "fd_forward.h" // face Detect
#include "fr_forward.h" // face Recognition
#include "fb_gfx.h" 
#include "image_util.h"
#include "dl_lib_matrix3d.h"

#include "FS.h"
#include "FFat.h"


#define DEFAULT_BAUD 1000000

#define FACE_COLOR_WHITE  0x00FFFFFF
#define FACE_COLOR_BLACK  0x00000000
#define FACE_COLOR_RED    0x000000FF
#define FACE_COLOR_GREEN  0x0000FF00
#define FACE_COLOR_BLUE   0x00FF0000
#define FACE_COLOR_YELLOW (FACE_COLOR_RED | FACE_COLOR_GREEN)
#define FACE_COLOR_CYAN   (FACE_COLOR_BLUE | FACE_COLOR_GREEN)
#define FACE_COLOR_PURPLE (FACE_COLOR_BLUE | FACE_COLOR_RED)

#define ENROLL_CONFIRM_TIMES 5
#define FACE_ID_SAVE_NUMBER 7

#define FACE_LIST_PATH "/faceList.dat"

typedef struct {
    uint8_t * buf;              
    size_t len;                 
    size_t width;               
    size_t height;  
	bool status; 	
 } camera_jpg;


class TonyCam
{
public:
	TonyCam();
	void begin(uint8_t slot,unsigned long baudrate = DEFAULT_BAUD);
	camera_jpg* capture();
	bool jpg2rgb(camera_jpg *fb, dl_matrix3du_t **image_matrix);
	uint16_t color565(uint8_t r, uint8_t g, uint8_t b) ;
	void rgb888to565(dl_matrix3du_t **image565,dl_matrix3du_t *image, bool RGB=false, bool swap=false);
	void rgb888to565_2(dl_matrix3du_t *image565,dl_matrix3du_t *image, bool RGB=false, bool swap=false);
	void clearMemory(dl_matrix3du_t *memory);
	void clearMemory(uint8_t *memory);
	void clearMemory(box_array_t *memory);
	void clearMemory(camera_jpg *memory);
	box_array_t *faceDetect(dl_matrix3du_t *image_matrix,mtmn_config_t *config);
	void drawFaceBoxes(dl_matrix3du_t *image_matrix, box_array_t *boxes,bool landmask);
	void rbgDrawBox(fb_data_t *fb,uint32_t color,int x,int y,int w,int h);
	int8_t alignFace(dl_matrix3du_t *image_matrix, box_array_t *net_boxes, dl_matrix3du_t **aligned_face);
	int8_t recognizeFace(face_id_list *l,dl_matrix3du_t *algined_face);
	void  faceIDInit(face_id_list *l, uint8_t size, uint8_t confirm_times);
	void  faceIDInitFlash(face_id_list *l, uint8_t size, uint8_t confirm_times);
	
	bool initFatFS();
	void initialFile();
	bool availableFile(const char * filename);
	bool writeFaceListToFlash(face_id_list *l);
	bool readFaceListFromFlash(face_id_list *l);
	bool deleteFaceLisFlash();
	
private:
		
};




















#endif






