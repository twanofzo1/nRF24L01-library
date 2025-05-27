#include "nRF24L01.h"
#include "nRF24L01_registers.h" // Use your defined register constants here

nRF24L01::nRF24L01(uint8_t csnPin, uint8_t cePin)
    : _csnPin(csnPin), _cePin(cePin) {}

void nRF24L01::begin() {
    pinMode(_csnPin, OUTPUT);
    pinMode(_cePin, OUTPUT);
    SPI.begin();
    ceLow();
    csnHigh();

    delay(100); // Power-on reset delay

    writeRegister(CONFIG, EN_CRC | PWR_UP);
    delay(2); // Wait for the chip to power up

    flushTX();
    flushRX();
}

void nRF24L01::setChannel(uint8_t channel) {
    writeRegister(RF_CH, channel & 0x7F);
}

void nRF24L01::setAutoRetransmit(uint8_t ard, uint8_t arc) {
    writeRegister(SETUP_RETR, (ard << 4) | (arc & 0x0F));
}

void nRF24L01::setPayloadSize(uint8_t size) {
    writeRegister(RX_PW_P0, size);
}

void nRF24L01::openWritingPipe(const uint8_t* address, uint8_t len) {
    writeRegister(RX_ADDR_P0, address, len);
    writeRegister(TX_ADDR, address, len);
}

void nRF24L01::openReadingPipe(uint8_t pipe, const uint8_t* address, uint8_t len) {
    writeRegister(EN_RXADDR, readRegister(EN_RXADDR) | (1 << pipe));
    if (pipe <= 1) {
        writeRegister(RX_ADDR_P0 + pipe, address, len);
    } else {
        writeRegister(RX_ADDR_P0 + pipe, &address[0], 1); // Only LSB
    }
}

void nRF24L01::startListening() {
    writeRegister(CONFIG, readRegister(CONFIG) | PRIM_RX);
    ceHigh();
    delayMicroseconds(130);
}

void nRF24L01::stopListening() {
    ceLow();
    writeRegister(CONFIG, readRegister(CONFIG) & ~PRIM_RX);
}

bool nRF24L01::available() {
    return readRegister(STATUS) & RX_DR;
}

void nRF24L01::read(void* buf, uint8_t len) {
    csnLow();
    SPI.transfer(SPI_R_RX_PAYLOAD);
    uint8_t* p = (uint8_t*)buf;
    for (int i = 0; i < len; i++) {
        p[i] = SPI.transfer(0xFF);
    }
    csnHigh();
    writeRegister(STATUS, RX_DR);
}

void nRF24L01::write(const void* buf, uint8_t len) {
    stopListening();
    flushTX();

    csnLow();
    SPI.transfer(SPI_W_TX_PAYLOAD);
    const uint8_t* p = (const uint8_t*)buf;
    for (int i = 0; i < len; i++) {
        SPI.transfer(p[i]);
    }
    csnHigh();

    ceHigh();
    delayMicroseconds(15); // Pulse CE
    ceLow();

    while (!(readRegister(STATUS) & (TX_DS | MAX_RT))) {}

    writeRegister(STATUS, TX_DS | MAX_RT);
}



uint8_t nRF24L01::readRegister(uint8_t reg) {
    csnLow();
    SPI.transfer(SPI_R_REGISTER | (reg & 0x1F));
    uint8_t value = SPI.transfer(0xFF);
    csnHigh();
    return value;
}

void nRF24L01::readRegister(uint8_t reg, uint8_t* buf, uint8_t len) {
    csnLow();
    SPI.transfer(SPI_R_REGISTER | (reg & 0x1F));
    for (int i = 0; i < len; i++) {
        buf[i] = SPI.transfer(0xFF);
    }
    csnHigh();
}

void nRF24L01::writeRegister(uint8_t reg, uint8_t value) {
    csnLow();
    SPI.transfer(SPI_W_REGISTER | (reg & 0x1F));
    SPI.transfer(value);
    csnHigh();
}

void nRF24L01::writeRegister(uint8_t reg, const uint8_t* buf, uint8_t len) {
    csnLow();
    SPI.transfer(SPI_W_REGISTER | (reg & 0x1F));
    for (int i = 0; i < len; i++) {
        SPI.transfer(buf[i]);
    }
    csnHigh();
}

void nRF24L01::flushTX() {
    csnLow();
    SPI.transfer(SPI_FLUSH_TX);
    csnHigh();
}

void nRF24L01::flushRX() {
    csnLow();
    SPI.transfer(SPI_FLUSH_RX);
    csnHigh();
}
