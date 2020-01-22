
#ifndef TonySpace_RS485_h
#define TonySpace_RS485_h

#include "Arduino.h"
#include <inttypes.h>
#include "Stream.h"
#include "esp32-hal.h"


class Tony_RS485 : public Stream
{
public:
    Tony_RS485(int uart_nr);
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

protected:
	uint8_t select_mode;
	uint8_t pin_RX;
	uint8_t pin_TX;
    int _uart_nr;
    uart_t* _uart;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
extern Tony_RS485 RS485;
#endif

#endif
















