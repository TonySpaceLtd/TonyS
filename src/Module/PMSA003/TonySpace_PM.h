/*  --------------------------------------------------------------------
	Cr.Alvaro Valdebenito for PM's Library 
	Cr.Adafruit for NeoPixel's Library
    --------------------------------------------------------------------*/
#ifndef TonySpace_PM_h
#define TonySpace_PM_h

#if (ARDUINO >= 100)
    #include <Arduino.h>
#else
    #include <WProgram.h>
    #include <pins_arduino.h>
#endif

// RGB NeoPixel permutations; white and red offsets are always same
// Offset:         W          R          G          B   // HEX representation
#define NEO_RGB  ((0 << 6) | (0 << 4) | (1 << 2) | (2)) // 0x06
#define NEO_RBG  ((0 << 6) | (0 << 4) | (2 << 2) | (1)) // 0x09
#define NEO_GRB  ((1 << 6) | (1 << 4) | (0 << 2) | (2)) // 0x52
#define NEO_GBR  ((2 << 6) | (2 << 4) | (0 << 2) | (1)) // 0xA1
#define NEO_BRG  ((1 << 6) | (1 << 4) | (2 << 2) | (0)) // 0x58
#define NEO_BGR  ((2 << 6) | (2 << 4) | (1 << 2) | (0)) // 0xA4

// RGBW NeoPixel permutations; all 4 offsets are distinct
// Offset:         W          R          G          B   // HEX representation
#define NEO_WRGB ((0 << 6) | (1 << 4) | (2 << 2) | (3)) // 0x1B
#define NEO_WRBG ((0 << 6) | (1 << 4) | (3 << 2) | (2)) // 0x1E
#define NEO_WGRB ((0 << 6) | (2 << 4) | (1 << 2) | (3)) // 0x27
#define NEO_WGBR ((0 << 6) | (3 << 4) | (1 << 2) | (2)) // 0x36
#define NEO_WBRG ((0 << 6) | (2 << 4) | (3 << 2) | (1)) // 0x2D
#define NEO_WBGR ((0 << 6) | (3 << 4) | (2 << 2) | (1)) // 0x39

#define NEO_RWGB ((1 << 6) | (0 << 4) | (2 << 2) | (3)) // 0x4B
#define NEO_RWBG ((1 << 6) | (0 << 4) | (3 << 2) | (2)) // 0x4E
#define NEO_RGWB ((2 << 6) | (0 << 4) | (1 << 2) | (3)) // 0x87
#define NEO_RGBW ((3 << 6) | (0 << 4) | (1 << 2) | (2)) // 0xC6
#define NEO_RBWG ((2 << 6) | (0 << 4) | (3 << 2) | (1)) // 0x8D
#define NEO_RBGW ((3 << 6) | (0 << 4) | (2 << 2) | (1)) // 0xC9

#define NEO_GWRB ((1 << 6) | (2 << 4) | (0 << 2) | (3)) // 0x63
#define NEO_GWBR ((1 << 6) | (3 << 4) | (0 << 2) | (2)) // 0x72
#define NEO_GRWB ((2 << 6) | (1 << 4) | (0 << 2) | (3)) // 0x93
#define NEO_GRBW ((3 << 6) | (1 << 4) | (0 << 2) | (2)) // 0xD2
#define NEO_GBWR ((2 << 6) | (3 << 4) | (0 << 2) | (1)) // 0xB1
#define NEO_GBRW ((3 << 6) | (2 << 4) | (0 << 2) | (1)) // 0xE1

#define NEO_BWRG ((1 << 6) | (2 << 4) | (3 << 2) | (0)) // 0x6C
#define NEO_BWGR ((1 << 6) | (3 << 4) | (2 << 2) | (0)) // 0x78
#define NEO_BRWG ((2 << 6) | (1 << 4) | (3 << 2) | (0)) // 0x9C
#define NEO_BRGW ((3 << 6) | (1 << 4) | (2 << 2) | (0)) // 0xD8
#define NEO_BGWR ((2 << 6) | (3 << 4) | (1 << 2) | (0)) // 0xB4
#define NEO_BGRW ((3 << 6) | (2 << 4) | (1 << 2) | (0)) // 0xE4

// Add NEO_KHZ400 to the color order value to indicate a 400 KHz
// device.  All but the earliest v1 NeoPixels expect an 800 KHz data
// stream, this is the default if unspecified.  Because flash space
// is very limited on ATtiny devices (e.g. Trinket, Gemma), v1
// NeoPixels aren't handled by default on those chips, though it can
// be enabled by removing the ifndef/endif below -- but code will be
// bigger.  Conversely, can disable the NEO_KHZ400 line on other MCUs
// to remove v1 support and save a little space.

#define NEO_KHZ800 0x0000 // 800 KHz datastream
#ifndef __AVR_ATtiny85__
    #define NEO_KHZ400 0x0100 // 400 KHz datastream
#endif

// If 400 KHz support is enabled, the third parameter to the constructor
// requires a 16-bit value (in order to select 400 vs 800 KHz speed).
// If only 800 KHz is enabled (as is default on ATtiny), an 8-bit value
// is sufficient to encode pixel color order, saving some space.

#ifdef NEO_KHZ400
    typedef uint16_t neoPixelType;
#else
    typedef uint8_t  neoPixelType;
#endif



enum PMS
{
  PLANTOWER_AUTO,             // self discovery
  PLANTOWER_24B,              // 24 byte long message, no count info (LBC)
  PLANTOWER_32B,              // 32 byte long message, w/count info (LBC)
  PLANTOWER_32B_S,            // 32 byte long message, w/count info and HCHO (LBC)
  PLANTOWER_32B_T,            // 32 byte long message, w/partial count info, temp and rhum (LBC)
  PLANTOWER_40B,              // 40 byte long message, w/count info, temp, rhum and HCHO (LBC)
  PMSx003 = PLANTOWER_AUTO,   // self discovery
  PMS1003 = PLANTOWER_32B,    // G1
  PMS3003 = PLANTOWER_24B,    // G3
  PMS5003 = PLANTOWER_32B,    // G5
  PMS5003S = PLANTOWER_32B_S, // G5S
  PMS5003T = PLANTOWER_32B_T, // G5T
  PMS5003ST = PLANTOWER_40B,  // G5ST
  PMS7003 = PLANTOWER_32B,    // G7
  PMSA003 = PLANTOWER_32B     // G10
};

class TONY_PM
{
public:
  union
  {
    uint16_t data[9]; // all PM/NC data
    struct
    {
      uint16_t pm[3]; // particulate matter [ug/m3]
      uint16_t nc[6]; // number concentration [#/100cc]
    };
    struct
    {
      // pmX [ug/m3]: PM1.0, PM2.5 & PM10
      uint16_t pm01, pm25, pm10;
      // nXpY [#/100cc]: number concentrations under X.Y um
      uint16_t n0p3, n0p5, n1p0, n2p5, n5p0, n10p0;
    };
  };
  union
  {
    float extra[3]; // T/RH/HCHO
    struct
    {
      // temperature [°C], relative humidity [%], formaldehyde concentration [mg/m3]
      float temp, rhum, hcho;
    };
  };

  void slot(uint8_t slot);
  void begin();
#define PMS_ERROR_TIMEOUT "Sensor read timeout"
#define PMS_ERROR_PMS_TYPE "Wrong PMSx003 sensor type"
#define PMS_ERROR_MSG_UNKNOWN "Unknown message protocol"
#define PMS_ERROR_MSG_HEADER "Incomplete message header"
#define PMS_ERROR_MSG_BODY "Incomplete message body"
#define PMS_ERROR_MSG_START "Wrong message start"
#define PMS_ERROR_MSG_LENGTH "Message too long"
#define PMS_ERROR_MSG_CKSUM "Wrong message checksum"
  enum STATUS
  {
    OK,
    ERROR_TIMEOUT,
    ERROR_PMS_TYPE,
    ERROR_MSG_UNKNOWN,
    ERROR_MSG_HEADER,
    ERROR_MSG_BODY,
    ERROR_MSG_START,
    ERROR_MSG_LENGTH,
    ERROR_MSG_CKSUM
  };
  STATUS status;
  STATUS read(bool tsi_mode = false, bool truncated_num = false);
  operator bool() { return status == OK; }
  void sleep();
  void wake();
  inline bool has_particulate_matter() { return status == OK; }
  inline bool has_number_concentration() { return (status == OK) && (pms != PMS3003); }
  inline bool has_temperature_humidity() { return (status == OK) && ((pms == PMS5003T) || (pms == PMS5003ST)); }
  inline bool has_formaldehyde() { return (status == OK) && ((pms == PMS5003S) || (pms == PMS5003ST)); }
    
  // adding offsets works well in normal range
  // might introduce under- or overflow at the ends of the sensor range
  inline void  set_rhum_offset(float offset) { rhum_offset = offset; };
  inline void  set_temp_offset(float offset) { temp_offset = offset; };
  inline float get_rhum_offset() { return rhum_offset; };
  inline float get_temp_offset() { return temp_offset; };
#ifdef PMS_DEBUG
#ifdef HAS_HW_SERIAL
  inline void print_buffer(Stream &term, const char *fmt)
  {
    char tmp[8];
    for (uint8_t n = 0; n < BUFFER_LEN; n += 2)
    {
      sprintf(tmp, fmt, buff2word(n));
      term.print(tmp);
    }
  }
#endif
  // debug timing
  inline uint16_t waited_ms() { return wait_ms; }
  // debug readBytes
  inline uint16_t bytes_read() { return nbytes; }
#endif

	//----------------------- RGB -----------------------//
	void show(void);
    void setPin(uint8_t p);
    void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
    void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
    void setPixelColor(uint16_t n, uint32_t c);
    void fill(uint32_t c = 0, uint16_t first = 0, uint16_t count = 0);
    void setBrightness(uint8_t);
    void clear();
    void updateLength(uint16_t n);
    void updateType(neoPixelType t);
    uint8_t * getPixels(void) const;
    uint8_t getBrightness(void) const;
    uint8_t sine8(uint8_t) const;
    uint8_t gamma8(uint8_t) const;
    int8_t getPin(void) {
        return rgb_pin;
    };
    uint16_t numPixels(void) const;
    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
    uint32_t getPixelColor(uint16_t n) const;
    inline bool canShow(void) {
        return (micros() - endTime) >= 300L;
    }

protected:
	Stream *uart;  // hardware/software serial
	PMS pms;       // sensor type/message protocol
	bool hwSerial; // Is uart hardware serial? (or software serial)
	uint8_t pin_RX, 
		  pin_TX;

	// Correct Temperature & Humidity
	float temp_offset = 0.0;
	float rhum_offset = 0.0;

	// utility functions
	STATUS trigRead();
	bool checkBuffer(size_t bufferLen);
	void decodeBuffer(bool tsi_mode, bool truncated_num);

	// message timing
	static const uint16_t max_wait_ms = 1000;
	uint16_t wait_ms; // time spent waiting for new sample

	// message buffer
	static const uint8_t BUFFER_LEN = 40;
	uint8_t buffer[BUFFER_LEN], nbytes;
	inline uint16_t buff2word(uint8_t n) { return (buffer[n] << 8) | buffer[n + 1]; }
  
  //-------------- RGB ------------//
	boolean
	#ifdef NEO_KHZ400  // If 400 KHz NeoPixel support enabled...
	is800KHz,      // ...true if 800 KHz pixels
	#endif
	begun;         // true if begin() previously called
	uint16_t
	numLEDs,       // Number of RGB LEDs in strip
	numBytes;      // Size of 'pixels' buffer below (3 or 4 bytes/pixel)
	int8_t
	rgb_pin;           // Output pin number (-1 if not yet set)
	uint8_t
	brightness,
	*pixels,        // Holds LED color values (3 or 4 bytes each)
	rOffset,       // Index of red byte within each 3- or 4-byte pixel
	gOffset,       // Index of green byte
	bOffset,       // Index of blue byte
	wOffset;       // Index of white byte (same as rOffset if no white)
	uint32_t
	endTime;       // Latch timing reference
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
extern TONY_PM TonyPM; // TonyPMx003, UART
#endif

#endif //_SERIALPM_H

