#include <Arduino.h>
#include <SPI.h>

class nRF24L01
{
public:
    //variables

    //functions
    bool OK();
    void help();

    nRF24L01();
    nRF24L01(uint8_t CE_PIN,uint8_t CSN_PIN);
    ~nRF24L01();

    void set_CE_Pin(uint8_t CE_PIN);
    void set_CSN_Pin(uint8_t CSN_PIN);

    void begin();
private:
    // variables
    uint8_t CSN_PIN, CE_PIN;

    // functions
    void CSN_Low();
    void CE_Low();
    void CSN_High();
    void CE_High();

    void writeAdress(uint8_t Adress, uint8_t value);
    void writeAdress(uint8_t Adress, int value);
    void readAdress(uint8_t Adress, uint8_t& buffer);
    void readAdress(uint8_t Adress, int& buffer);

};


enum nRF24L01_Flags{

};