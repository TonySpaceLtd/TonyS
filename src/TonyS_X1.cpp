// V.TS003
#include "TonyS_X1.h"

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
TonyS_X1 Tony;
#endif

TonyS_X1::TonyS_X1()
{
	
}

MAX11301 MAX11301;
TonySpace_IO IO;

byte workingDevice = 0x00;
byte board_model = UNKNOWN_MODEL;
byte pinStatus[20];
uint8_t type_command = 0;
uint8_t get_data[5] = {0,0,0,0,0};
uint16_t chkSum1 = 0;
uint8_t chkSum2 = 0;
bool check = 0;

bool TonyS_X1::begin() 
{
	bool check_results = 0;
	preparePower();
	delay(100);
	Wire.begin();
	delay(2);
	checkIC();
	IO.Real_pinMode(LED_BUILTIN, OUTPUT); // ------- Set to OUTPUT
	IO.Real_digitalWrite(LED_BUILTIN, LOW);  //---- OFF LED_BUILTIN
	
	if((workingDevice & 0x01) == 0x01)
	{
		if (board_model == PRO_MODEL) 
		{
			check_results = MAX11301.Config_deviceControl(); 			
			// Set MAX11301 to default pin mode 
			for(uint8_t i=0; i<20; i++)
			{
				pinStatus[i] = modeGPI;
			}
		}
		Tony.pinMode(IO15, OUTPUT); //----  Set Pin IO14 (Relay 1) to OUTPUT
		Tony.pinMode(IO16, OUTPUT); //----  Set Pin IO15 (Relay 2) to OUTPUT
		Tony.digitalWrite(IO15, LOW); //---- Write LOW to pin IO14 (Relay 1)
		Tony.digitalWrite(IO16, LOW); //---- Write LOW to pin IO15 (Relay 2)
	}
	return check_results;
}

void TonyS_X1::pinMode(uint8_t pin, uint16_t type)
{
	if(pin >= 100)   
	{	
		if (board_model == PRO_MODEL) 
		{
			if(ismax_rdy())
			{
				switch(type)
				{
					case INPUT:
					{
						MAX11301.Basic_Config_Port_For_GPI(pin, 0xfff); //Threshold ‭‭4095 ‬= 2.5V  , 0xfff = 2.5V(MAX)
						pinStatus[pin] = modeGPI;
						break;
					}
					case OUTPUT: 					
					{
						MAX11301.Basic_Config_Port_For_GPO(pin, 1352); //Logic's ouput 1352 = 3.3V  
						pinStatus[pin] = modeGPO;
						break;
					}
					case INPUT_PULLUP:
					{
						Serial.println();
						Serial.println("This pin not support INPUT_PULLUP mode.");
						Serial.println("Please select only pin IO0, IO1, IO2, IO3, IO10 and IO11.");
						break;
					}
					case ADC2V5:
					{
						MAX11301.Basic_Config_Port(pin, ADCtype2);  // Config Port (pin) to ADC 0-10V	 
						pinStatus[pin] = modeADC;
						break;
					}
					case ADC10V:
					{
						MAX11301.Basic_Config_Port(pin, ADCtype1);  // Config Port (pin) to ADC 0-10V
						pinStatus[pin] = modeADC;
						break;
					}
					default:
					break;
				}
			}
			else
			{
				Serial.println();
				Serial.println("MAX11301 not available !.");
				Serial.println("Please select only pin IO0, IO1, IO2, IO3, IO10 and IO11.");
			}
		}
		else if (board_model == BASIC_MODEL) 
		{
			if(isstm_rdy())
			{
				pin -= 12;
				pin = constrain(pin, 0, 7);
				if(type == INPUT) pinMode_STM32(pin, SET_INPUT);
				else if(type == OUTPUT) pinMode_STM32(pin, SET_OUTPUT);
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
				Serial.println("STM32 not available !.");
				Serial.println("Please select only pin IO0, IO1, IO2, IO3, IO10 and IO11.");
			}
		}	
	}
	else
	{	
		IO.Real_pinMode(pin, type); 
	}
}

void TonyS_X1::digitalWrite(uint8_t pin, bool value)
{
	if(pin >= 100)
	{
		if (board_model == PRO_MODEL) 
		{
			if(value == 1)
			{
				MAX11301.write_speedGPO(pin, 1); 
			}
			else
			{
				MAX11301.write_speedGPO(pin, 0); 
			}
		}
		else if (board_model == BASIC_MODEL) 
		{
			pin -= 12;
			pin = constrain(pin, 0, 7);
			if(value == 0) digitalWrite_STM32(pin, OUTPUT_LOW);
			else if(value == 1) digitalWrite_STM32(pin, OUTPUT_HIGH);
		}	
	}
	else
	{
		IO.Real_digitalWrite(pin, value);
	}
}

void TonyS_X1::digitalnormalWrite(uint8_t pin, bool value)
{
	if(pin >= 100)
	{
		if (board_model == PRO_MODEL) 
		{
			if(value == 1)
			{
				MAX11301.writeGPO(pin, 1);  
			}
			else
			{
				MAX11301.writeGPO(pin, 0); 
			}
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
	if(pin >= 100)
	{
		if (board_model == PRO_MODEL) 
		{
			value = MAX11301.readGPI(pin);
		}
		else if (board_model == BASIC_MODEL) 
		{
			pin -= 12;
			pin = constrain(pin, 0, 7);
			value = digitalRead_STM32(pin);
		}
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
	if(pin >= 100)
	{
		if (board_model == PRO_MODEL) 
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
		else if (board_model == BASIC_MODEL) 
		{
			pin = constrain(pin, 0, 11);
			dataADC = readADC_STM32(pin);
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
	if (board_model == PRO_MODEL) 
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
	else if (board_model == BASIC_MODEL) Serial.println("Please use the PRO version");
}

void TonyS_X1::preparePower()
{
	IO.Real_pinMode(powerPin, OUTPUT); // ------- Set to OUTPUT
	IO.Real_pinMode(POWER_EXTENSION_PORT, OUTPUT); // ------- Set to OUTPUT
	onExtension(); // Power of MAX11301 or STM32
	onPower();
}

void TonyS_X1::onPower()
{
	IO.Real_digitalWrite(powerPin, HIGH); //----- HIGH for ON Board's power
}

void TonyS_X1::offPower()
{
	IO.Real_digitalWrite(powerPin, LOW);  //----- LOW for OFF Board's power
}

void TonyS_X1::onExtension()
{
	IO.Real_digitalWrite(POWER_EXTENSION_PORT, HIGH);  //---- HIGH for ON Board's power
}

void TonyS_X1::offExtension()
{
	IO.Real_digitalWrite(POWER_EXTENSION_PORT, LOW);  //----- LOW for OFF Board's power
}

void TonyS_X1::checkIC()
{
	Wire.begin();
	byte error;

	Wire.beginTransmission(ADDR_MAX11301);
	error = Wire.endTransmission();
	if(error == 0)
	{
		board_model = PRO_MODEL;
		workingDevice |= 0x01;
	}
	else
	{
		Wire.beginTransmission(ADDR_STM32);
		error = Wire.endTransmission();
		if(error == 0)
		{
			board_model = BASIC_MODEL;
			workingDevice |= 0x01;
		}	
	}
	delay(10);
	Serial.println();
	Serial.println("BOARD        =  Tony Space");
	if (board_model == PRO_MODEL) 
	{
		Wire.beginTransmission(ADDR_DS3231);
		error = Wire.endTransmission();
		if(error == 0)
		{
			workingDevice |= 0x10;
		}
		Serial.println("MODEL        =  PRO");
		Serial.print(F("MAX11301     =  "));
		Serial.println(((workingDevice & 0x01) == 0x01)? "ON":"OFF");
		Serial.print(F("RTC(DS3231)  =  "));
		Serial.println(((workingDevice & 0x10) == 0x10)? "ON":"OFF");
	}
	else if (board_model == BASIC_MODEL) 
	{
		Wire.beginTransmission(ADDR_MCP7940);
		error = Wire.endTransmission();
		if(error == 0)
		{
			workingDevice |= 0x10;
		}
		Serial.println("MODEL        =  BASIC");
		Serial.print(F("STM32        =  "));
		Serial.println(((workingDevice & 0x01) == 0x01)? "ON":"OFF");
		Serial.print(F("RTC(MCP7940) =  "));
		Serial.println(((workingDevice & 0x10) == 0x10)? "ON":"OFF");
	}
	else Serial.println("MODEL        =  NON DETECTED");
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
		
		Wire.beginTransmission(ADDR_MAX11301);
		error = Wire.endTransmission();
		if(error == 0)
		{
			workingDevice |= 0x01;
			return(true);
		}
	}
	return(false);
}

bool TonyS_X1::isstm_rdy()
{
	if((workingDevice & 0x01) == 0x01)
	{
		return(true);
	}
	else	// recheck MAX
	{
		byte error;
		
		Wire.beginTransmission(ADDR_STM32);
		error = Wire.endTransmission();
		if(error == 0)
		{
			workingDevice |= 0x01;
			return(true);
		}
	}
	return(false);
}

void TonyS_X1::sendCommand(uint8_t pin, uint8_t command)
{
  uint16_t get_chkSum = buildcheckSum(pin, command);
  chkSum1 = get_chkSum;
  chkSum1 >>= 8;
  chkSum2 = get_chkSum;
  Wire.beginTransmission(ADDR_STM32);    
  Wire.write(pin);  
  Wire.write(command);  
  Wire.write(chkSum1);  
  Wire.write(chkSum2); 
  Wire.endTransmission();
}

void TonyS_X1::receiveData()
{
  delay(6);
  if(Wire.requestFrom(ADDR_STM32, 5))    //  if request succeeded
  {
    uint8_t count=0;
    while(Wire.available())    // slave may send less than requested
    {
      get_data[count] = Wire.read();    // receive a byte as character
      count++;
    }
  }
  Wire.endTransmission();
}


uint16_t TonyS_X1::buildcheckSum(uint8_t pin,uint8_t value)
{
  return (pin+value);
}

bool TonyS_X1::reCheck()
{
  if(chkSum1 == get_data[3] && chkSum2 == get_data[4])
  {
    return 1;
  }
  else return 0;
}

void TonyS_X1::clearBuffer()
{
  type_command = 0;
  chkSum1 = 0;
  chkSum2 = 0;
  check = 0;
  for(uint8_t i=0; i<5; i++)
  {
    get_data[i] = 0;
  }
}

uint16_t TonyS_X1::readADC_STM32(uint8_t pin)
{
  uint16_t data_ADC = 0;
  type_command = READ_ADC;
  handleData(pin);
  data_ADC = get_data[1];
  data_ADC = (data_ADC<<8)+get_data[2];
  clearBuffer();
  return data_ADC;
}

bool TonyS_X1::digitalRead_STM32(uint8_t pin)
{
  bool read_input = 0;
  type_command = READ_DIGITAL_INPUT;
  handleData(pin);
  read_input = get_data[1];
  clearBuffer();
  return read_input;
}

void TonyS_X1::pinMode_STM32(uint8_t pin, uint8_t pinmode)
{
  type_command = pinmode;
  handleData(pin);
  clearBuffer();
}

void TonyS_X1::digitalWrite_STM32(uint8_t pin, uint8_t pinmode)
{
  type_command = pinmode;
  handleData(pin);
  clearBuffer();
}

void TonyS_X1::handleData(uint8_t pin)
{
  for(uint8_t i=0; i<10; i++)
  {
    sendCommand(pin, type_command);
    receiveData();
    check = reCheck();
    if(check == 1) break;
    else if(i == 9) Serial.println("Fail...");
    delay(10);
  }
}


HardwareSerial TonyS_X1:: SerialBegin(uint8_t slot,unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert, unsigned long timeout_ms) 
{		
	if(slot==SLOT1 || slot==SLOT1_U || slot==SLOT2 || slot==SLOT2_U || slot==SLOT3 || slot==SLOT3_U)
	{
		Serial1.begin(baud,config,RX1,TX1,invert,timeout_ms);
		return(Serial1);
	}
	else if(slot==SLOT4 || slot==SLOT4_U || slot==SLOT5 || slot==SLOT5_U || slot==SLOT6 || slot==SLOT6_U)
	{
		Serial2.begin(baud,config,RX2,TX2,invert,timeout_ms);
		return(Serial2);
	}
}
