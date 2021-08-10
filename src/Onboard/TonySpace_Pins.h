// V.TS003
#ifndef TonySpace_Pins_h
#define TonySpace_Pins_h

// Mapping of analog pins (AIO)
static const uint8_t AIO0 = 107; //7
static const uint8_t AIO1 = 106; //6
static const uint8_t AIO2 = 109; //9
static const uint8_t AIO3 = 108; //8
static const uint8_t AIO4 = 111; //11
static const uint8_t AIO5 = 110; //10
static const uint8_t AIO6 = 101; //1
static const uint8_t AIO7 = 100; //0	
static const uint8_t AIO8 = 102; //2	
static const uint8_t AIO9 = 103; //3	
static const uint8_t AIO10 = 104; //4	
static const uint8_t AIO11 = 105; //5	



// Mapping of digital pins (IO)
static const uint8_t IO0 = 25;  //Real = 25
static const uint8_t IO1 = 26;  //Real = 26
static const uint8_t IO2 = 27;  //Real = 27
static const uint8_t IO3 = 14;  //Real = 14
static const uint8_t IO4 = 117; //17  
static const uint8_t IO5 = 116; //16  
static const uint8_t IO6 = 112; //12	
static const uint8_t IO7 = 113; //13	
static const uint8_t IO8 = 114; //14	
static const uint8_t IO9 = 115; //15	
static const uint8_t IO10 = 4;  //Real = 4
static const uint8_t IO11 = 13;  //Real = 13

static const uint8_t IO_CS = 5; //Real = 5
static const uint8_t IO_MOSI = 23;  //Real = 23
static const uint8_t IO_MISO = 19;  //Real = 19
static const uint8_t IO_CLK = 18;  //Real = 18

static const uint8_t IO_SDA = 21;  //Real = 21
static const uint8_t IO_SCL = 22;  //Real = 22

static const uint8_t IO12 = 15;// LED_BUILTIN
static const uint8_t IO13 = 2; // ON-OFF Board's Power
static const uint8_t IO14 = 12; // ON-OFF MAX11301's Power
static const uint8_t IO15 = 118; // ON-OFF Relay 1
static const uint8_t IO16 = 119; // ON-OFF Relay 2

#define  RX0  		    3   
#define  TX0  		    1   
#define  RX1  		    34   
#define  TX1  		    32   
#define  RX2  		    35   
#define  TX2  		    33 

#define  LED_BUILTIN    	  IO12   //  Pin IO12 = LED Build in
#define  POWER_PIN      	  IO13   //  Pin IO13 = ON-OFF Power
#define  powerPin       	  IO13  
#define  POWER_EXTENSION_PORT IO14   //  Pin IO14 = ON-OFF MAX11301 Power
#define  powerEXTENSION_PORT  IO14   //  Pin IO14 = ON-OFF MAX11301 Power

static const uint8_t Relay_1 = IO15; // ON-OFF Relay 1
static const uint8_t Relay_2 = IO16; // ON-OFF Relay 2

#define  SLOT1    		1
#define  SLOT1_U    	2
#define  SLOT2    		3
#define  SLOT2_U    	4
#define  SLOT3    		5
#define  SLOT3_U    	6
#define  SLOT4    		7
#define  SLOT4_U    	8
#define  SLOT5    		9
#define  SLOT5_U    	10
#define  SLOT6    		11
#define  SLOT6_U    	12

static const uint8_t getPinIO[13] = {0,IO0,IO1,IO2,IO3,IO4,IO5,IO6,IO7,IO8,IO9,IO10,IO11};

#endif /* TonySpace_Pins_h */
