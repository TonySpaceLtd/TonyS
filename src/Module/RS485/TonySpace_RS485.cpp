#include "TonySpace_RS485.h"
#include "TonyS_X1.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>


#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
Tony_RS485 RS485;
#endif

Tony_RS485::Tony_RS485()
{
}

void Tony_RS485::slot(uint8_t slot)
{
	switch(slot)
	{
		case  SLOT1 :
			pin_RX = RX1;
			pin_TX = TX1;
			_uart_nr = 1;
			break;
		case  SLOT2 :
			pin_RX = RX1;
			pin_TX = TX1;
			_uart_nr = 1;
			break;
		case  SLOT3 :
			pin_RX = RX1;
			pin_TX = TX1;
			_uart_nr = 1;
			break;
		case  SLOT4 :
			pin_RX = RX2;
			pin_TX = TX2;
			_uart_nr = 2;
			break;
		case  SLOT5 :
			pin_RX = RX2;
			pin_TX = TX2;
			_uart_nr = 2;
			break;
		case  SLOT6 :
			pin_RX = RX2;
			pin_TX = TX2;
			_uart_nr = 2;
			break;
		case  SLOT1_U :
			pin_RX = RX1;
			pin_TX = TX1;
			_uart_nr = 1;
			break;
		case  SLOT2_U :
			pin_RX = RX1;
			pin_TX = TX1;
			_uart_nr = 1;
			break;
		case  SLOT3_U :
			pin_RX = RX1;
			pin_TX = TX1;
			_uart_nr = 1;
			break;
		case  SLOT4_U :
			pin_RX = RX2;
			pin_TX = TX2;
			_uart_nr = 2;
			break;
		case  SLOT5_U :
			pin_RX = RX2;
			pin_TX = TX2;
			_uart_nr = 2;
			break;
		case  SLOT6_U :
			pin_RX = RX2;
			pin_TX = TX2;
			_uart_nr = 2;
			break;
	}	
}


void Tony_RS485::begin(unsigned long baud)
{
	uint32_t config = SERIAL_8N1;
	int8_t rxPin = pin_RX;
	int8_t txPin = pin_TX;
	bool invert = false;
	unsigned long timeout_ms = 20000UL;
	
	_baud = baud;
	//calculate the time perdiod for 3 characters for the given bps in ms.
	uint32_t T3_5 = 0;
	if (_baud > 19200) T3_5 = 1750; 
	else T3_5 = 35000000; 
	_frameDelay = T3_5/_baud;
	
    if(0 > _uart_nr || _uart_nr > 2) {
        log_e("Serial number is invalid, please use 0, 1 or 2");
        return;
    }
    if(_uart) {
        end();
    }
    if(_uart_nr == 0 && rxPin < 0 && txPin < 0) {
        rxPin = 3;
        txPin = 1;
    }
    if(_uart_nr == 1 && rxPin < 0 && txPin < 0) {
        rxPin = RX1;
        txPin = TX1;
    }
    if(_uart_nr == 2 && rxPin < 0 && txPin < 0) {
        rxPin = RX2;
        txPin = TX2;
    }

    _uart = uartBegin(_uart_nr, baud ? baud : 9600, config, rxPin, txPin, 256, invert);

    if(!baud) {
        time_t startMillis = millis();
        unsigned long detectedBaudRate = 0;
        while(millis() - startMillis < timeout_ms && !(detectedBaudRate = uartDetectBaudrate(_uart))) {
            yield();
        }

        end();

        if(detectedBaudRate) {
            delay(100); // Give some time...
            _uart = uartBegin(_uart_nr, detectedBaudRate, config, rxPin, txPin, 256, invert);
        } else {
            log_e("Could not detect baudrate. Serial data at the port must be present within the timeout for detection to be possible");
            _uart = NULL;
        }
    }
}

void Tony_RS485::updateBaudRate(unsigned long baud)
{
	uartSetBaudRate(_uart, baud);
}

void Tony_RS485::end()
{
    if(uartGetDebug() == _uart_nr) {
        uartSetDebug(0);
    }
    uartEnd(_uart, pin_RX, pin_TX);
    _uart = 0;
}

size_t Tony_RS485::setRxBufferSize(size_t new_size) {
    return uartResizeRxBuffer(_uart, new_size);
}

void Tony_RS485::setDebugOutput(bool en)
{
    if(_uart == 0) {
        return;
    }
    if(en) {
        uartSetDebug(_uart);
    } else {
        if(uartGetDebug() == _uart_nr) {
            uartSetDebug(0);
        }
    }
}

int Tony_RS485::available(void)
{
    return uartAvailable(_uart);
}
int Tony_RS485::availableForWrite(void)
{
    return uartAvailableForWrite(_uart);
}

int Tony_RS485::peek(void)
{
    if (available()) {
        return uartPeek(_uart);
    }
    return -1;
}

int Tony_RS485::read(void)
{
    if(available()) {
        return uartRead(_uart);
    }
	//delayMicroseconds(1);
    return -1;
}

void Tony_RS485::flush()
{
    uartFlush(_uart);
}

size_t Tony_RS485::write(uint8_t c)
{
	uartWrite(_uart, c);
	//delayMicroseconds(2000);
    return 1;
}

size_t Tony_RS485::write(const uint8_t *buffer, size_t size)
{
	uartWriteBuf(_uart, buffer, size);
	//delayMicroseconds(2000);
    return size;
}
uint32_t  Tony_RS485::baudRate()

{
	return uartGetBaudRate(_uart);
}
Tony_RS485::operator bool() const
{
    return true;
}

uint16_t Tony_RS485::checkSerial(void)
{
	//while there is more data in the UART than when last checked
	while(RS485.available()> _len)
	{
		_len = RS485.available();
		//Wait for 3 bytewidths of data (SOM/EOM)
		//delayMicroseconds(_frameDelay);
		//Check the UART again
		//Serial.println(_len);  
	}  
	return _len;
}

/*
Copies the contents of the UART to a buffer
*/
void Tony_RS485::serialRx(byte slave_id,uint16_t start_addr, uint16_t end_addr)
{
	uint16_t crc = 0xFFFF;
	byte i;
	byte data_buffer = 0;
	uint8_t count_data = 0;
	bool first_data = 0;
	bool start_read = 0;
	
	if(_len>2)
	{
		for (i=0 ; i < _len ; i++)
		{
			data_buffer = RS485.read();
			
			if(start_read == 1)
			{
				_data[count_data] = data_buffer;
				count_data++;
			}
			
			if(data_buffer == slave_id && start_read == 0)
			{
				_data[count_data] = data_buffer;
				start_read = 1;
				count_data++;
			}
		}
		
		for (int position = 0; position < count_data-2; position++) {
			crc ^= (uint16_t)_data[position];          // XOR byte into least sig. byte of crc
	  
			for (int i = 8; i != 0; i--) {    // Loop over each bit
			if ((crc & 0x0001) != 0) {      // If the LSB is set
				crc >>= 1;                    // Shift right and XOR 0xA001
				crc ^= 0xA001;
			}
			else                            // Else LSB is not set
			crc >>= 1;                    // Just shift right
			}
		}	
		 // Serial.print("CRC : ");
		 // Serial.println(crc, HEX);
	}
	 // Serial.print("Len : ");
	 // Serial.println(_len);
	uint32_t check_crc = _data[count_data-1];
	check_crc <<= 8;
	check_crc = check_crc+_data[count_data-2];
	 // Serial.print("CHECK CRC : ");
	 // Serial.println(check_crc, HEX);
	if(crc != check_crc || _len != count_data) _len = 0;//memset(_data, 0, sizeof(_data));
}

byte Tony_RS485::readByte(uint8_t byteNumber)
{
	if(byteNumber <= _len) return (_data[byteNumber]);
	else return 0;
}

uint16_t Tony_RS485::readRegister(uint16_t address)
{
	if((address-start_address) <= _len)
	{	
		uint16_t data_buff = _data[(((address-start_address)*2)+3)]; 
		data_buff <<= 8;
		data_buff = data_buff + _data[((address-start_address)*2)+4];
		return data_buff;
	}
	else return 0;
}

uint16_t Tony_RS485::get_byteNumber(void)
{
	return _len;
}

bool Tony_RS485::write_singleCoil(uint8_t slave_ID, uint16_t address, bool value)
{
	if(value == 1) 
	{ 
		for(uint8_t i=0; i<9; i++)
		{			
			if(requestData(slave_ID, 5, address, 65280, 10)) return 1; // 65280 = 0xFF00
			delay(100);
		}
		return 0; 
	}
	else 
	{
		for(uint8_t i=0; i<9; i++)
		{			
			if(requestData(slave_ID, 5, address, 0, 10)) return 1;
			delay(100);
		}
		return 0;
	}
}

bool Tony_RS485::isReady()
{
	if(RS485.read()>1) return 0;
	else return 1;
}

bool Tony_RS485::requestData(uint8_t slave_ID, uint8_t function, uint16_t startAddress, uint16_t numberData, uint16_t timeout)
{
	uint16_t crc = 0xFFFF;
	byte sendByte[8];
	start_address = startAddress;
	
	sendByte[0] = slave_ID;
	sendByte[1] = function;
	sendByte[2] = startAddress>>8;
	sendByte[3] = startAddress;
	sendByte[4] = numberData>>8;
	sendByte[5] = numberData;
	
	while(RS485.isReady() != 1);
	memset(_data, 0, 257);
	
	for (int position = 0; position < 6; position++) {
    crc ^= (uint16_t)sendByte[position];          // XOR byte into least sig. byte of crc
  
		for (int i = 8; i != 0; i--) {    // Loop over each bit
		if ((crc & 0x0001) != 0) {      // If the LSB is set
			crc >>= 1;                    // Shift right and XOR 0xA001
			crc ^= 0xA001;
		}
		else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
		}
	}	
	sendByte[6] = crc;
	sendByte[7] = crc>>8;
	
	for(uint8_t i = 0 ; i < 8 ; i++) RS485.write(sendByte[i]);
	delayMicroseconds(_frameDelay);
	
	//initialize mesasge length
	_len = 0;
	//Serial.println("Checking Serial");       
	//check for data in the recieve buffer
	uint32_t start_time = millis();
	while(checkSerial() < (5+(numberData*2)))
	{
		uint32_t current_time = millis() - start_time; 
		if(current_time >= timeout) return 0;
		delay(100);
	}
		
	//Serial.println("Reading...");   
	//retrieve the query message from the serial uart
	serialRx(slave_ID, startAddress, numberData);
	//Serial.println("Done");   
	return 1;
}
