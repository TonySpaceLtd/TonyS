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
	onPower();
	onMAX11301();
	delay(100);
	Wire.begin();
	checkIC();
	
	IO.Real_pinMode(LED_BUILTIN, OUTPUT); // ------- Set to OUTPUT
	IO.Real_digitalWrite(LED_BUILTIN, LOW);  //---- OFF LED_BUILTIN
	
	if((workingDevice & 0x01) == 0x01)
	{
		MAX11301.Config_deviceControl(); 
		
		// Set MAX11301 to default pin mode 
		for(uint8_t i=0; i<20; i++)
		{
			pinStatus[i] = modeGPI;
		}
		
		pinMode(IO15, OUTPUT); //----  Set Pin IO14 (Relay 1) to OUTPUT
		pinMode(IO16, OUTPUT); //----  Set Pin IO15 (Relay 2) to OUTPUT
		digitalWrite(IO15, LOW); //---- Write LOW to pin IO14 (Relay 1)
		digitalWrite(IO16, LOW); //---- Write LOW to pin IO15 (Relay 2)
	}
}

void TonyS_X1::pinMode(uint8_t pin, uint8_t type)
{
	if(pin <= 19)   
	{	
		if(ismax_rdy())
		{
			if(type == INPUT)
			{
				MAX11301.Basic_Config_Port_For_GPI(pin, 0xfff); //Threshold ‭‭4095 ‬= 2.5V  , 0xfff = 2.5V(MAX)
				pinStatus[pin] = modeGPI;
			}
			else if(type == OUTPUT)
			{
				MAX11301.Basic_Config_Port_For_GPO(pin, 1352); //Logic's ouput 1352 = 3.3V  
				pinStatus[pin] = modeGPO;
			}
			else if(type == INPUT_PULLUP)
			{
				Serial.println();
				Serial.println("This pin not support INPUT_PULLUP mode.");
				Serial.println("Please select only pin IO0, IO1, IO2, IO3, IO10 and IO11.");
			}
		}
		else
		{
			Serial.println();
			Serial.println("MAX11301 not available !.");
			Serial.println("Please select only pin IO0, IO1, IO2, IO3, IO10 and IO11.");
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
			MAX11301.writeGPO(pin, 1); //Logic's ouput 1 = HIGH 
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
		if(pinStatus[pin] == modeADC)
		{
			dataADC = MAX11301.readADC(pin);  // Read ADC from port (pin) 
		}
		else
		{
			MAX11301.Basic_Config_Port(pin, ADCtype1);  // Config Port (pin) to ADC 0-10V
			dataADC = MAX11301.readADC(pin);  // Read ADC from port (pin) 
			pinStatus[pin] = modeADC;
		}
	}
	else
	{
		dataADC = IO.Real_analogRead(pin);
	}
	return(dataADC);
}

void TonyS_X1::analogWrite(uint8_t pin, uint16_t Output)
{
	if(pinStatus[pin] == modeDAC)
	{
		MAX11301.writeDAC(pin, Output);  //Output 0-4095
	}
	else
	{
		MAX11301.Basic_Config_Port(pin, DACtype); 
		MAX11301.writeDAC(pin, Output);  //Output 0-4095
		pinStatus[pin] = modeDAC;
	}
}

void TonyS_X1::onPower()
{
	IO.Real_pinMode(powerPin, OUTPUT); // ------- Set to OUTPUT
	IO.Real_digitalWrite(powerPin, HIGH); //----- HIGH for ON Board's power
}

void TonyS_X1::offPower()
{
	IO.Real_pinMode(powerPin, OUTPUT); // ------- Set to OUTPUT
	IO.Real_digitalWrite(powerPin, LOW);  //----- LOW for OFF Board's power
}

void TonyS_X1::onMAX11301()
{	
	IO.Real_pinMode(POWER_MAX11301, OUTPUT); // ------- Set to OUTPUT
	IO.Real_digitalWrite(POWER_MAX11301, HIGH);  //---- HIGH for ON Board's power
}

void TonyS_X1::offMAX11301()
{
	IO.Real_pinMode(POWER_MAX11301, OUTPUT); // ------- Set to OUTPUT
	IO.Real_digitalWrite(POWER_MAX11301, LOW);  //----- LOW for OFF Board's power
}

void TonyS_X1::checkIC()
{
	Wire.begin();
	byte error;
	byte check_addrRTC = 0x68;
	byte check_addrMAX11301 = 0x38;
	
	Serial.println("Checking IC on board...");
	Wire.beginTransmission(check_addrRTC);
	error = Wire.endTransmission();
	if(error == 0)
	{
		workingDevice |= 0x10;
	}
	delay(10);

	Wire.beginTransmission(check_addrMAX11301);
	error = Wire.endTransmission();
	if(error == 0)
	{
		workingDevice |= 0x01;
	}
	
	Serial.print(F("RTC(DS3231) = "));
	Serial.println(((workingDevice & 0x10) == 0x10)? "ON":"OFF");
	
	Serial.print(F("MAX11301    = "));
	Serial.println(((workingDevice & 0x01) == 0x01)? "ON":"OFF");
}

bool TonyS_X1::isrtc_rdy()
{
	if((workingDevice & 0x10) == 0x10)
	{
		return(true);
	}
	else	// recheck RTC
	{
		byte error;
		byte check_addrRTC = 0x68;
		
		Wire.beginTransmission(check_addrRTC);
		error = Wire.endTransmission();
		if(error == 0)
		{
			workingDevice |= 0x10;
			return(true);
		}
	}
	return(false);
}

bool TonyS_X1::ismax_rdy()
{
	if((workingDevice & 0x01) == 0x01)
	{
		return(true);
	}
	else	// recheck MAX
	{
		byte error;
		byte check_addrMAX11301 = 0x38;
		
		Wire.beginTransmission(check_addrMAX11301);
		error = Wire.endTransmission();
		if(error == 0)
		{
			workingDevice |= 0x01;
			return(true);
		}
	}
	return(false);
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
