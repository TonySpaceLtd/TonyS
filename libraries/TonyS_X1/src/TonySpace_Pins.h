#ifndef TonySpace_Pins_h
#define TonySpace_Pins_h

// Mapping of analog pins (AIO)
static const uint8_t AIO0 = 7;
static const uint8_t AIO1 = 6;
static const uint8_t AIO2 = 9;
static const uint8_t AIO3 = 8;
static const uint8_t AIO4 = 11; 
static const uint8_t AIO5 = 10;
static const uint8_t AIO6 = 1;	
static const uint8_t AIO7 = 0;	
static const uint8_t AIO8 = 2;	
static const uint8_t AIO9 = 3;	
static const uint8_t AIO10 = 4;	
static const uint8_t AIO11 = 5;	



// Mapping of digital pins (IO)
static const uint8_t IO0 = 125;  //Real = 25
static const uint8_t IO1 = 126;  //Real = 26
static const uint8_t IO2 = 127;  //Real = 27
static const uint8_t IO3 = 114;  //Real = 14
static const uint8_t IO4 = 17;   
static const uint8_t IO5 = 16;   
static const uint8_t IO6 = 12;	
static const uint8_t IO7 = 13;	
static const uint8_t IO8 = 14;	
static const uint8_t IO9 = 15;	
static const uint8_t IO10 = 104; //Real = 4
static const uint8_t IO_CS = 105; //Real = 5
static const uint8_t IO11 = 113; //Real = 13

static const uint8_t IO12 = 115;// LED_BUILTIN
static const uint8_t IO13 = 102; // ON-OFF Board's Power
static const uint8_t IO14 = 18; // ON-OFF Relay 1
static const uint8_t IO15 = 19; // ON-OFF Relay 2

#define  RX1  		    34   
#define  TX1  		    32   
#define  RX2  		    35   
#define  TX2  		    33 

#define  LED_BUILTIN    IO12   //  Pin IO24 = LED Build in
#define  POWER_PIN      IO13   //  Pin IO25 = ON-OFF Power
#define  powerPin       IO13  

static const uint8_t Relay_1 = IO14; // ON-OFF Relay 1
static const uint8_t Relay_2 = IO15; // ON-OFF Relay 2

#define  SLOT1    		0
#define  SLOT1_U    	1
#define  SLOT2    		2
#define  SLOT2_U    	3
#define  SLOT3    		4
#define  SLOT3_U    	5
#define  SLOT4    		6
#define  SLOT4_U    	7
#define  SLOT5    		8
#define  SLOT5_U    	9
#define  SLOT6    		10
#define  SLOT6_U    	11

#endif /* TonySpace_Pins_h */
