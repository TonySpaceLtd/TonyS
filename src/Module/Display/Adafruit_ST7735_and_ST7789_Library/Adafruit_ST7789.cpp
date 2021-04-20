#include "Adafruit_ST77xx.h"
#include "Adafruit_ST7789.h"
#include <TonyS_X1.h>
TonyS_X1 Tony_Display;
// CONSTRUCTORS ************************************************************

/*!
    @brief  Instantiate Adafruit ST7789 driver with software SPI
    @param  cs    Chip select pin #
    @param  dc    Data/Command pin #
    @param  mosi  SPI MOSI pin #
    @param  sclk  SPI Clock pin #
    @param  rst   Reset pin # (optional, pass -1 if unused)
*/
Adafruit_ST7789::Adafruit_ST7789(int8_t cs, int8_t dc, int8_t mosi,
  int8_t sclk, int8_t rst) : Adafruit_ST77xx(320, 240, cs, dc, mosi, sclk,
  rst) {
}

/*!
    @brief  Instantiate Adafruit ST7789 driver with hardware SPI
    @param  cs   Chip select pin #
    @param  dc   Data/Command pin #
    @param  rst  Reset pin # (optional, pass -1 if unused)
*/
Adafruit_ST7789::Adafruit_ST7789(int8_t cs, int8_t dc, int8_t rst) :
  Adafruit_ST77xx(320, 240, cs, dc, rst) {
}

uint8_t find_cs(uint8_t slot)
{
	return IO_CS;
}
uint8_t find_dc(uint8_t slot,uint8_t type)
{
	if(type == TFT_240_240)
	{
		switch(slot)
		{
			case  SLOT1 :
				return IO0;
			case  SLOT2 :
				return IO2;
			case  SLOT3 :
				return IO4;
			case  SLOT4 :
				return IO6;
			case  SLOT5 :
				return IO8;
			case  SLOT6 :
				return IO10;
			case  SLOT1_U :
				return IO1;
			case  SLOT2_U :
				return IO3;
			case  SLOT3_U :
				return IO5;
			case  SLOT4_U :
				return IO7;
			case  SLOT5_U :
				return IO9;
			case  SLOT6_U :
				return IO11;
		}
	}
	if(type == TFT_240_320)
	{
		switch(slot)
		{
			case  SLOT1 :
				return IO10;
			case  SLOT2 :
				return IO8;
			case  SLOT4 :
				return IO4;
			case  SLOT5 :
				return IO2;
			case  SLOT1_U :
				return IO11;
			case  SLOT2_U :
				return IO9;
			case  SLOT4_U :
				return IO5;
			case  SLOT5_U :
				return IO3;
		}
	}
	return -1;
	
}
uint8_t find_rst(uint8_t slot,uint8_t type)
{
	if(type == TFT_240_240)
	{
		switch(slot)
		{
			case  SLOT1 :
				return AIO0;
			case  SLOT2 :
				return AIO2;
			case  SLOT3 :
				return AIO4;
			case  SLOT4 :
				return AIO6;
			case  SLOT5 :
				return AIO8;
			case  SLOT6 :
				return AIO10;
			case  SLOT1_U :
				return AIO1;
			case  SLOT2_U :
				return AIO3;
			case  SLOT3_U :
				return AIO5;
			case  SLOT4_U :
				return AIO7;
			case  SLOT5_U :
				return AIO9;
			case  SLOT6_U :
				return AIO11;
		}
	}
	if(type == TFT_240_320)
	{
		switch(slot)
		{
			case  SLOT1 :
				return IO2;
			case  SLOT2 :
				return IO4; //IO4
			case  SLOT4 :
				return IO8;
			case  SLOT5 :
				return IO10;
			case  SLOT1_U :
				return IO3;
			case  SLOT2_U :
				return IO5;
			case  SLOT4_U :
				return IO9;
			case  SLOT5_U :
				return IO11;
		}
	}
	
	return -1;
}


Adafruit_ST7789::Adafruit_ST7789(int8_t slot) :
Adafruit_ST77xx(320, 240,find_cs(slot),find_dc(slot,0),find_rst(slot,0)) 
{
	
}

Adafruit_ST7789::Adafruit_ST7789(int8_t slot,uint8_t tfttype = TFT_240_240) :
Adafruit_ST77xx(320, 240,find_cs(slot),find_dc(slot,tfttype),find_rst(slot,tfttype)) 
{
	_tfttype = tfttype;
	if(tfttype == TFT_240_240)
	{
		switch(slot)
		{
			case  SLOT1 : 
				Tony_Display.pinMode(IO10, OUTPUT);    // Select CS
				Tony_Display.digitalWrite(IO10, LOW); 
				Tony_Display.pinMode(AIO10, OUTPUT);   // BLK
				Tony_Display.digitalWrite(AIO10, LOW);
				break;
			case  SLOT2 : 
				Tony_Display.pinMode(IO8, OUTPUT);
				Tony_Display.digitalWrite(IO8, LOW);
				Tony_Display.pinMode(AIO8, OUTPUT);
				Tony_Display.digitalWrite(AIO8, LOW);
				break;
			case  SLOT3 : 
				Tony_Display.pinMode(IO6, OUTPUT);
				Tony_Display.digitalWrite(IO6, LOW);
				Tony_Display.pinMode(AIO6, OUTPUT);
				Tony_Display.digitalWrite(AIO6, LOW);
				break;
			case  SLOT4 : 
				Tony_Display.pinMode(IO4, OUTPUT);
				Tony_Display.digitalWrite(IO4, LOW);
				Tony_Display.pinMode(AIO4, OUTPUT);
				Tony_Display.digitalWrite(AIO4, LOW);
				break;
			case  SLOT5 : 
				Tony_Display.pinMode(IO2, OUTPUT);
				Tony_Display.digitalWrite(IO2, LOW);
				Tony_Display.pinMode(AIO2, OUTPUT);
				Tony_Display.digitalWrite(AIO2, LOW);
				break;
			case  SLOT6 : 
				Tony_Display.pinMode(IO0, OUTPUT);
				Tony_Display.digitalWrite(IO0, LOW);
				Tony_Display.pinMode(AIO0, OUTPUT);
				Tony_Display.digitalWrite(AIO0, LOW);
				break;
			case  SLOT1_U : 
				Tony_Display.pinMode(IO11, OUTPUT);
				Tony_Display.digitalWrite(IO9, LOW);
				Tony_Display.pinMode(AIO1, OUTPUT);
				Tony_Display.digitalWrite(AIO1, LOW);
				break;
			case  SLOT2_U : 
				Tony_Display.pinMode(IO9, OUTPUT);
				Tony_Display.digitalWrite(IO9, LOW);
				Tony_Display.pinMode(AIO3, OUTPUT);
				Tony_Display.digitalWrite(AIO3, LOW);
				break;
			case  SLOT3_U : 
				Tony_Display.pinMode(IO7, OUTPUT);
				Tony_Display.digitalWrite(IO7, LOW);
				Tony_Display.pinMode(AIO5, OUTPUT);
				Tony_Display.digitalWrite(AIO5, LOW);
				break;
			case  SLOT4_U : 
				Tony_Display.pinMode(IO5, OUTPUT);
				Tony_Display.digitalWrite(IO5, LOW);
				Tony_Display.pinMode(AIO7, OUTPUT);
				Tony_Display.digitalWrite(AIO7, LOW);
				break;
			case  SLOT5_U : 
				Tony_Display.pinMode(IO3, OUTPUT);
				Tony_Display.digitalWrite(IO3, LOW);
				Tony_Display.pinMode(AIO9, OUTPUT);
				Tony_Display.digitalWrite(AIO9, LOW);
				break;
			case  SLOT6_U : 
				Tony_Display.pinMode(IO1, OUTPUT);
				Tony_Display.digitalWrite(IO1, LOW);
				Tony_Display.pinMode(AIO11, OUTPUT);
				Tony_Display.digitalWrite(AIO11, LOW);
				break;
		}
	}
	if(tfttype == TFT_240_320)
	{
		switch(slot)
		{
			case  SLOT1 :
				Tony_Display.pinMode(IO0, OUTPUT);  // Select CS
				Tony_Display.digitalWrite(IO0, LOW);
				Tony_Display.pinMode(IO8, OUTPUT);  // BLK
				Tony_Display.digitalWrite(IO8, HIGH);
				break;
			case  SLOT2 :
				Tony_Display.pinMode(IO2, OUTPUT);
				Tony_Display.digitalWrite(IO2, LOW);
				Tony_Display.pinMode(IO6, OUTPUT);
				Tony_Display.digitalWrite(IO6, HIGH);
				break;
			case  SLOT4 :
				Tony_Display.pinMode(IO6, OUTPUT);
				Tony_Display.digitalWrite(IO6, LOW);
				Tony_Display.pinMode(IO2, OUTPUT);
				Tony_Display.digitalWrite(IO2, HIGH);
				break;
			case  SLOT5 :
				Tony_Display.pinMode(IO8, OUTPUT);
				Tony_Display.digitalWrite(IO8, LOW);
				Tony_Display.pinMode(IO0, OUTPUT);
				Tony_Display.digitalWrite(IO0, HIGH);
				break;
			case  SLOT1_U :
				Tony_Display.pinMode(IO1, OUTPUT);
				Tony_Display.digitalWrite(IO1, LOW);
				Tony_Display.pinMode(IO9, OUTPUT);
				Tony_Display.digitalWrite(IO9, HIGH);
				break;
			case  SLOT2_U :
				Tony_Display.pinMode(IO3, OUTPUT);
				Tony_Display.digitalWrite(IO3, LOW);
				Tony_Display.pinMode(IO7, OUTPUT);
				Tony_Display.digitalWrite(IO7, HIGH);
				break;
			case  SLOT4_U :
				Tony_Display.pinMode(IO7, OUTPUT);
				Tony_Display.digitalWrite(IO7, LOW);
				Tony_Display.pinMode(IO3, OUTPUT);
				Tony_Display.digitalWrite(IO3, HIGH);
				break;
			case  SLOT5_U :
				Tony_Display.pinMode(IO9, OUTPUT);
				Tony_Display.digitalWrite(IO9, LOW);
				Tony_Display.pinMode(IO1, OUTPUT);
				Tony_Display.digitalWrite(IO1, HIGH);
				break;
		}
	}
}




#if !defined(ESP8266)
/*!
    @brief  Instantiate Adafruit ST7789 driver with selectable hardware SPI
    @param  spiClass  Pointer to an SPI device to use (e.g. &SPI1)
    @param  cs        Chip select pin #
    @param  dc        Data/Command pin #
    @param  rst       Reset pin # (optional, pass -1 if unused)
*/
Adafruit_ST7789::Adafruit_ST7789(SPIClass *spiClass, int8_t cs, int8_t dc,
  int8_t rst) : Adafruit_ST77xx(320, 240, spiClass, cs, dc, rst) {
}
#endif // end !ESP8266

// SCREEN INITIALIZATION ***************************************************

// Rather than a bazillion writecommand() and writedata() calls, screen
// initialization commands and arguments are organized in these tables
// stored in PROGMEM.  The table may look bulky, but that's mostly the
// formatting -- storage-wise this is hundreds of bytes more compact
// than the equivalent code.  Companion function follows.

static const uint8_t PROGMEM
  generic_st7789[] =  {                // Init commands for 7789 screens
    9,                              //  9 commands in list:
    ST77XX_SWRESET,   ST_CMD_DELAY, //  1: Software reset, no args, w/delay
      150,                          //    150 ms delay
    ST77XX_SLPOUT ,   ST_CMD_DELAY, //  2: Out of sleep mode, no args, w/delay
      255,                          //     255 = 500 ms delay
    ST77XX_COLMOD , 1+ST_CMD_DELAY, //  3: Set color mode, 1 arg + delay:
      0x55,                         //     16-bit color
      10,                           //     10 ms delay
    ST77XX_MADCTL , 1,              //  4: Mem access ctrl (directions), 1 arg:
      0x08,                         //     Row/col addr, bottom-top refresh
    ST77XX_CASET  , 4,              //  5: Column addr set, 4 args, no delay:
      0x00,
      0,        //     XSTART = 0
      0,
      240,  //     XEND = 240
    ST77XX_RASET  , 4,              //  6: Row addr set, 4 args, no delay:
      0x00,
      0,             //     YSTART = 0
      320>>8,
      320&0xFF,  //     YEND = 320
    ST77XX_INVON  ,   ST_CMD_DELAY,  //  7: hack
      10,
    ST77XX_NORON  ,   ST_CMD_DELAY, //  8: Normal display on, no args, w/delay
      10,                           //     10 ms delay
    ST77XX_DISPON ,   ST_CMD_DELAY, //  9: Main screen turn on, no args, delay
    255 };                          //     255 = max (500 ms) delay

/**************************************************************************/
/*!
    @brief  Initialization code common to all ST7789 displays
    @param  width  Display width
    @param  height Display height
    @param  mode   SPI data mode; one of SPI_MODE0, SPI_MODE1, SPI_MODE2
                   or SPI_MODE3 (do NOT pass the numbers 0,1,2 or 3 -- use
                   the defines only, the values are NOT the same!)
*/
/**************************************************************************/
void Adafruit_ST7789::init(uint16_t width, uint16_t height, uint8_t mode) {
  // Save SPI data mode. commonInit() calls begin() (in Adafruit_ST77xx.cpp),
  // which in turn calls initSPI() (in Adafruit_SPITFT.cpp), passing it the
  // value of spiMode. It's done this way because begin() really should not
  // be modified at this point to accept an SPI mode -- it's a virtual
  // function required in every Adafruit_SPITFT subclass and would require
  // updating EVERY such library...whereas, at the moment, we know that
  // certain ST7789 displays are the only thing that may need a non-default
  // SPI mode, hence this roundabout approach...
  spiMode = mode;
  // (Might get added similarly to other display types as needed on a
  // case-by-case basis.)

  commonInit(NULL);

  if ((width == 240) && (height == 240)) {
    _colstart = 0;
    _rowstart = 80;
  } else {
    _colstart = 0;
    _rowstart = 0;
  }
  WIDTH = width;
  HEIGHT = height;
  _width    = width;
  _height   = height;

  displayInit(generic_st7789);
  if(_tfttype == TFT_240_320)
  {
	invertDisplay(false);
	setRotation(0);
  }
  else
	setRotation(3);
}

/**************************************************************************/
/*!
    @brief  Set origin of (0,0) and orientation of TFT display
    @param  m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Adafruit_ST7789::setRotation(uint8_t m) {
  uint8_t madctl = 0;

  rotation = m & 3; // can't be higher than 3

  switch (rotation) {
   case 0:
     madctl  = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB;
     _xstart = _colstart;
     _ystart = _rowstart;
     _width = WIDTH;
     _height = HEIGHT;
	 
     break;
   case 1:
     madctl  = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
     _xstart = _rowstart;
     _ystart = _colstart;
     _height = WIDTH;
     _width = HEIGHT;
     break;
  case 2:
     madctl  = ST77XX_MADCTL_RGB;
     _xstart = 0;
     _ystart = 0;
     _width = WIDTH;
     _height = HEIGHT;
     break;
   case 3:
     madctl  = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
     _xstart = 0;
     _ystart = 0;
     _height = WIDTH;
     _width = HEIGHT;
     break;
  }

  sendCommand(ST77XX_MADCTL, &madctl, 1);
}
