
#ifndef TonySpace_RS485_h
#define TonySpace_RS485_h

#include "Arduino.h"
#include <inttypes.h>
#include "Stream.h"
#include "esp32-hal.h"


class Tony_RS485 : public Stream
{
public:
    Tony_RS485();
	void slot(uint8_t slot);
    void begin(unsigned long baud);
    void end();
    void updateBaudRate(unsigned long baud);
    int available(void);
    int availableForWrite(void);
    int peek(void);
    int read(void);
    void flush(void);
    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size);

    inline size_t write(const char * s)
    {
        return write((uint8_t*) s, strlen(s));
    }
    inline size_t write(unsigned long n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(long n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(unsigned int n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(int n)
    {
        return write((uint8_t) n);
    }
    uint32_t baudRate();
    operator bool() const;

    size_t setRxBufferSize(size_t);
    void setDebugOutput(bool);
	
	uint16_t checkSerial(void);
	void serialRx(byte slave_id,uint16_t start_addr, uint16_t end_addr);
	bool requestData(uint8_t slave_ID, uint8_t function, uint16_t startAddress, uint16_t numberData, uint16_t timeout);
	byte readByte(uint8_t byteNumber);
	uint16_t readRegister(uint16_t address);
	bool write_singleCoil(uint8_t slave_ID, uint16_t address, bool value);
	uint16_t get_byteNumber(void);
	bool isReady();

protected:
	uint8_t pin_RX, 
			pin_TX;
			
	uint16_t start_address,
			 _len;
			
    int _uart_nr;
	
    uart_t* _uart;
	
	byte _data[257];

	word _baud,
		 _crc,
		 _frameDelay;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
extern Tony_RS485 RS485;
#endif

#endif
















