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
			select_mode = IO0;
			pin_RX = RX1;
			pin_TX = TX1;
			_uart_nr = 1;
			break;
		case  SLOT2 :
			select_mode = IO2;
			pin_RX = RX1;
			pin_TX = TX1;
			_uart_nr = 1;
			break;
		case  SLOT3 :
			select_mode = IO4;
			pin_RX = RX1;
			pin_TX = TX1;
			_uart_nr = 1;
			break;
		case  SLOT4 :
			select_mode = IO6;
			pin_RX = RX2;
			pin_TX = TX2;
			_uart_nr = 2;
			break;
		case  SLOT5 :
			select_mode = IO8;
			pin_RX = RX2;
			pin_TX = TX2;
			_uart_nr = 2;
			break;
		case  SLOT6 :
			select_mode = IO10;
			pin_RX = RX2;
			pin_TX = TX2;
			_uart_nr = 2;
			break;
		case  SLOT1_U :
			select_mode = IO1;
			pin_RX = RX1;
			pin_TX = TX1;
			_uart_nr = 1;
			break;
		case  SLOT2_U :
			select_mode = IO3;
			pin_RX = RX1;
			pin_TX = TX1;
			_uart_nr = 1;
			break;
		case  SLOT3_U :
			select_mode = IO5;
			pin_RX = RX1;
			pin_TX = TX1;
			_uart_nr = 1;
			break;
		case  SLOT4_U :
			select_mode = IO7;
			pin_RX = RX2;
			pin_TX = TX2;
			_uart_nr = 2;
			break;
		case  SLOT5_U :
			select_mode = IO9;
			pin_RX = RX2;
			pin_TX = TX2;
			_uart_nr = 2;
			break;
		case  SLOT6_U :
			select_mode = IO11;
			pin_RX = RX2;
			pin_TX = TX2;
			_uart_nr = 2;
			break;
	}	
	Tony.pinMode(select_mode, OUTPUT);
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
	_frameDelay = 24000/_baud;
	
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

    _uart = uartBegin(_uart_nr, baud ? baud : 9600, config, rxPin, txPin, 256, invert,112);

    if(!baud) {
        time_t startMillis = millis();
        unsigned long detectedBaudRate = 0;
        while(millis() - startMillis < timeout_ms && !(detectedBaudRate = uartDetectBaudrate(_uart))) {
            yield();
        }

        end();

        if(detectedBaudRate) {
            delay(100); // Give some time...
            _uart = uartBegin(_uart_nr, detectedBaudRate, config, rxPin, txPin, 256, invert,112);
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
   delay(10);
    //uartEnd(_uart, pin_RX, pin_TX);
	uartEnd(_uart);
    _uart = 0;
	
}

size_t Tony_RS485::setRxBufferSize(size_t new_size) {
   // return uartResizeRxBuffer(_uart, new_size
   return 0;
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
	Tony.digitalWrite(select_mode, 0);
    if(available()) {
        return uartRead(_uart);
    }
	delayMicroseconds(1);
    return -1;
}

void Tony_RS485::flush()
{
    uartFlush(_uart);
}

size_t Tony_RS485::write(uint8_t c)
{
	if (modeRTU == 0) 
	{
		Tony.digitalWrite(select_mode, 1);
		uartWrite(_uart, c);
		delayMicroseconds(2000);
		Tony.digitalWrite(select_mode, 0);
	}
	else
	{
		uartWrite(_uart, c);
		delayMicroseconds(2000);
	}
    return 1;
}

size_t Tony_RS485::write(const uint8_t *buffer, size_t size)
{
	if (modeRTU == 0) 
	{
		Tony.digitalWrite(select_mode, 1);
		uartWriteBuf(_uart, buffer, size);
		delayMicroseconds(2000);
		Tony.digitalWrite(select_mode, 0);
	}
	else
	{
		uartWriteBuf(_uart, buffer, size);
		delayMicroseconds(2000);
	}
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

void Tony_RS485::checkSerial(void)
{
	//while there is more data in the UART than when last checked
	while(RS485.available()> _len)
	{
		//Serial.println(_len); 		
		_len = RS485.available();
		//Wait for 3 bytewidths of data (SOM/EOM)
		delay(_frameDelay);
		//Check the UART again
	}  
}

/*
Copies the contents of the UART to a buffer
*/
void Tony_RS485::serialRx(void)
{
	byte i;
	
	for (i=0 ; i < _len ; i++)
	{
	   _data[i] = RS485.read();
	}
}

byte Tony_RS485::readByte(uint8_t byteNumber)
{
	if(byteNumber <= _len)
	{
		return (_data[byteNumber]);
	}
	else
	{
		return 0;
	}
}

uint16_t Tony_RS485::get_byteNumber(void)
{
	return _len;
}

bool Tony_RS485::requestData(uint8_t slave_ID, uint8_t function, uint16_t startAddress, uint16_t numberData)
{
	uint16_t crc = 0xFFFF;
	byte sendByte[8];
	slaveID = slave_ID;
	modeRTU = 1;
	
	sendByte[0] = slaveID;
	sendByte[1] = function;
	sendByte[2] = startAddress>>8;
	sendByte[3] = startAddress;
	sendByte[4] = numberData>>8;
	sendByte[5] = numberData;
	
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
	
	Tony.digitalWrite(select_mode, 1);
	for(uint8_t i = 0 ; i < 8 ; i++)
	{
		RS485.write(sendByte[i]);
		//delay(2);
	}
	delay(16);
	Tony.digitalWrite(select_mode, 0);
	modeRTU = 0;
	
	//initialize mesasge length
	_len = 0;
	//Serial.println("Checking Serial");       
	//check for data in the recieve buffer
	this->checkSerial();
    //Serial.println("Checked");    
	
	//if there is nothing in the recieve buffer, bail.
	if(_len == 0)
	{
		return 0;
	}
	Serial.println("Reading...");   
	//retrieve the query message from the serial uart
	this->serialRx();
	//Serial.println("Done");   
	return 1;
}
