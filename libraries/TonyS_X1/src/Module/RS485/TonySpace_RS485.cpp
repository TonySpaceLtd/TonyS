#include "TonySpace_RS485.h"
#include "TonyS_X1.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>


#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
Tony_RS485 RS485(0);
#endif

void Tony_RS485::slot(uint8_t slot)
{
	switch(slot)
	{
		case  SLOT1 :
			select_mode = IO1;
			pin_RX = RX1;
			pin_TX = TX1;
			break;
		case  SLOT2 :
			select_mode = IO3;
			pin_RX = RX1;
			pin_TX = TX1;
			break;
		case  SLOT3 :
			select_mode = IO5;
			pin_RX = RX1;
			pin_TX = TX1;
			break;
		case  SLOT4 :
			select_mode = IO7;
			pin_RX = RX2;
			pin_TX = TX2;
			break;
		case  SLOT5 :
			select_mode = IO9;
			pin_RX = RX2;
			pin_TX = TX2;
			break;
		case  SLOT6 :
			select_mode = IO11;
			pin_RX = RX2;
			pin_TX = TX2;
			break;
		case  SLOT1_U :
			select_mode = IO1;
			pin_RX = RX1;
			pin_TX = TX1;
			break;
		case  SLOT2_U :
			select_mode = IO3;
			pin_RX = RX1;
			pin_TX = TX1;
			break;
		case  SLOT3_U :
			select_mode = IO5;
			pin_RX = RX1;
			pin_TX = TX1;
			break;
		case  SLOT4_U :
			select_mode = IO7;
			pin_RX = RX2;
			pin_TX = TX2;
			break;
		case  SLOT5_U :
			select_mode = IO9;
			pin_RX = RX2;
			pin_TX = TX2;
			break;
		case  SLOT6_U :
			select_mode = IO11;
			pin_RX = RX2;
			pin_TX = TX2;
			break;
	}	
	Tony.pinMode(select_mode, OUTPUT);
}

Tony_RS485::Tony_RS485(int uart_nr) : _uart_nr(uart_nr), _uart(NULL) {}

void Tony_RS485::begin(unsigned long baud)
{
	uint32_t config = SERIAL_8N1;
	int8_t rxPin = pin_RX;
	int8_t txPin = pin_TX;
	bool invert = false;
	unsigned long timeout_ms = 20000UL;
	
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
    uartEnd(_uart);
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
	Tony.digitalWrite(select_mode, 0);
    if(available()) {
        return uartRead(_uart);
    }
    return -1;
}

void Tony_RS485::flush()
{
    uartFlush(_uart);
}

size_t Tony_RS485::write(uint8_t c)
{
	Tony.digitalWrite(select_mode, 1);
    uartWrite(_uart, c);
    return 1;
}

size_t Tony_RS485::write(const uint8_t *buffer, size_t size)
{
	Tony.digitalWrite(select_mode, 1);
    uartWriteBuf(_uart, buffer, size);
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