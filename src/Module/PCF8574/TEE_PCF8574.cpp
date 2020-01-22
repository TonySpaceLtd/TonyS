#include "TEE_PCF8574.h"
#include "TonyS_X1.h"


PCF8574::PCF8574()
{
}
bool PCF8574::begin(char addr)
{
	bool ret = false;
	Wire.begin();
	Wire.beginTransmission(addr);
	byte error = Wire.endTransmission();
	if(error==0)
	{	
		addr_ = addr;
		ret = true;
	}		
	return ret;
}
uint8_t PCF8574::read_()
{
	uint8_t data=0;
	Wire.requestFrom(addr_, 1);  
	if (Wire.available()) 
	{
		data = Wire.read();          
	}
	return(data);
}
void  PCF8574::write(uint8_t data)
{
	Wire.beginTransmission(addr_);   
	Wire.write(data);                
	Wire.endTransmission();         
}

///////////////////////////////////////////////////////////////////
PCF8574_IO::PCF8574_IO()
{
	output_buf=0xFF;
	pin_mode = 0;
}
void PCF8574_IO::pinMode(uint8_t pin,uint8_t mode)
{
	if(mode == PCF_INPUT)//==1
	{
			pin_mode |= 1<<pin;
	}
	else
	{
			pin_mode &= ~(1<<pin);
	}
	
}

void PCF8574_IO::digitalWrite(uint8_t pin,uint8_t mode)
{
	if(mode)
		output_buf |= 1 << pin;
	else
		output_buf &= ~(1 << pin);	
	write(output_buf);
}
uint8_t PCF8574_IO::digitalRead(uint8_t pin)
{
	write(pin_mode|output_buf);
	uint8_t r = read_();	
	if(r&1<<pin)
		return 1;
	return 0;
}
uint8_t PCF8574_IO::read()
{
	write(pin_mode|output_buf);
	return read_();
	
}