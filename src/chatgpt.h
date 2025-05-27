#pragma once

#include <Arduino.h>
#include <SPI.h>

class nRF24L01 {
public:
    nRF24L01(uint8_t csnPin, uint8_t cePin);

    void begin();
    void setChannel(uint8_t channel);
    void setAutoRetransmit(uint8_t ard, uint8_t arc);
    void setPayloadSize(uint8_t size);
    void openWritingPipe(const uint8_t* address, uint8_t len);
    void openReadingPipe(uint8_t pipe, const uint8_t* address, uint8_t len);
    void startListening();
    void stopListening();

    bool available();
    void read(void* buf, uint8_t len);
    void write(const void* buf, uint8_t len);

private:
    uint8_t _csnPin, _cePin;

    void csnLow();
    void csnHigh();
    void ceLow();
    void ceHigh();

    uint8_t readRegister(uint8_t reg);
    void readRegister(uint8_t reg, uint8_t* buf, uint8_t len);
    void writeRegister(uint8_t reg, uint8_t value);
    void writeRegister(uint8_t reg, const uint8_t* buf, uint8_t len);
    void flushTX();
    void flushRX();
};
