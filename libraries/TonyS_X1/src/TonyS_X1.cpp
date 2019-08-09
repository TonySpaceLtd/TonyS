#include "TonyS_X1.h"

TonyS_X1 Tony;

TonyS_X1::TonyS_X1()
{
	
}

MAX11301 MAX11301;
TonySpace_IO IO;

byte workingDevice = 0x00;

void TonyS_X1::begin() 
{
	delay(500);
	onPower();
	delay(1000);
	Wire.begin();
	checkIC();
	
	IO.Real_pinMode(LED_BUILTIN, OUTPUT); // ------- Set to OUTPUT
	pinMode(IO14, OUTPUT); //----  Set Pin IO14 (Relay 1) to OUTPUT
	pinMode(IO15, OUTPUT); //----  Set Pin IO15 (Relay 2) to OUTPUT
	IO.Real_digitalWrite(LED_BUILTIN, LOW);  //---- OFF LED_BUILTIN
	digitalWrite(IO14, LOW); //---- Write LOW to pin IO14 (Relay 1)
	digitalWrite(IO15, LOW); //---- Write LOW to pin IO15 (Relay 2)
	
	if(workingDevice == 0x01 || workingDevice == 0x11)
	{
		MAX11301.Config_deviceControl(); 
	}
}

void TonyS_X1::pinMode(uint8_t pin, uint8_t type)
{
	if(pin <= 19)   
	{	
		if(workingDevice == 0x01 || workingDevice == 0x11)
		{
			if(type == INPUT)
			{
				MAX11301.Basic_Config_Port_For_GPI(pin, 3276); //Threshold ‭‭3276‬ ‬= 2.0V  , 0xFFF = 2.5V(MAX)
			}
			else if(type == OUTPUT)
			{
				MAX11301.Basic_Config_Port_For_GPO(pin, 1352); //Logic's ouput 1352 = 3.3V  
			}
			else if(type == INPUT_PULLUP)
			{
				Serial.println();
				Serial.println("This pin not support INPUT_PULLUP mode.");
				Serial.println("Please select only pin IO0 ,IO1, IO4 ,IO5 , IO20 and IO21.");
			}
		}
		else
		{
			Serial.println();
			Serial.println("MAX11301 not available !.");
			Serial.println("Please select only pin IO0 ,IO1, IO4 ,IO5 , IO20 and IO21.");
		}
	}
	else
	{	
		IO.Real_pinMode(pin, type); 
	}
}

void TonyS_X1::digitalWrite(uint8_t pin, bool value)
{
	if(pin <= 19)
	{
		if(value == 0)
		{
			MAX11301.writeGPO(pin, 0);
		}
		else if(value == 1)
		{
			MAX11301.writeGPO(pin, 1352); //Logic's ouput 1352 = 3.3V  
		}
	}
	else
	{
		IO.Real_digitalWrite(pin, value);
	}
}

bool TonyS_X1::digitalRead(uint8_t pin)
{
	bool value = 0;
	if(pin <= 19)
	{
		value = MAX11301.readGPI(pin);
	}
	else
	{
		value = IO.Real_digitalRead(pin);
	}
	return(value);
}

uint16_t TonyS_X1::analogRead(uint8_t pin)
{
	uint16_t dataADC = 0;
	if(pin <= 19)
	{
		MAX11301.Basic_Config_Port(pin, ADCtype1);  // Config Port (pin) to ADC 0-10V
		dataADC = MAX11301.readADC(pin);  // Read ADC from port (pin) 
	}
	else
	{
		dataADC = IO.Real_analogRead(pin);
	}
	return(dataADC);
}

void TonyS_X1::analogWrite(uint8_t pin, uint16_t Output)
{
	MAX11301.Basic_Config_Port(pin, DACtype); 
	MAX11301.writeDAC(pin, Output);  //Output 0-10V
}

void TonyS_X1::onPower()
{
	IO.Real_pinMode(powerPin, OUTPUT); // ------- Set to OUTPUT
	IO.Real_digitalWrite(powerPin, HIGH);
}

void TonyS_X1::offPower()
{
	IO.Real_pinMode(powerPin, OUTPUT); // ------- Set to OUTPUT
	IO.Real_digitalWrite(powerPin, LOW);  //---- HIGH for ON Board's power
	//IO.Real_pinMode(2, OUTPUT); // ------- Set to OUTPUT
	//IO.Real_digitalWrite(2, LOW);  //---- HIGH for ON Board's power
}

void TonyS_X1::checkIC()
{
	Wire.begin();
	byte error;
	byte check_addrMAX11301 = 0x38;
	byte check_addrRTC = 0x68;
	int maxDevices = 0;
	int rtcDevices = 0;
	
	Serial.println("Checking IC on board...");
	Wire.beginTransmission(check_addrRTC);
	error = Wire.endTransmission();
	if(error == 0)
	{
		rtcDevices = 1;
	}
	delay(10);

	Wire.beginTransmission(check_addrMAX11301);
	error = Wire.endTransmission();
	if(error == 0)
	{
		maxDevices = 1;
	}
	if(rtcDevices == 1 && maxDevices == 1)
	{
		Serial.println("RTC(DS3231) = ON");
		Serial.println("MAX11301)   = ON");
		workingDevice = 0x11;
	}
	else if(rtcDevices == 1 && maxDevices == 0)
	{
		Serial.println("RTC(DS3231) = ON");
		Serial.println("MAX11301)   = OFF");
		workingDevice = 0x10;
	}
	else if(rtcDevices == 0 && maxDevices == 1)
	{
		Serial.println("RTC(DS3231) = OFF");
		Serial.println("MAX11301)   = ON");
		workingDevice = 0x01;
	}
	else if(rtcDevices == 0 && maxDevices == 0)
	{
		Serial.println("RTC(DS3231) = OFF");
		Serial.println("MAX11301)   = OFF");
		workingDevice = 0x00;
	}
}
HardwareSerial TonyS_X1:: SerialBegin(uint8_t slot,unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert, unsigned long timeout_ms) 
{		
	if(slot <=SLOT3_U)
	{
		Serial1.begin(baud,config,RX1,TX1,invert,timeout_ms);
		return(Serial1);
	}
	Serial2.begin(baud,config,RX2,TX2,invert,timeout_ms);
	return(Serial2);
	
}
