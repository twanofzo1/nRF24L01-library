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
    void hardReset();

    bool isDataAvaliable();
    bool readData();

    void test(); // remove later
private:
    // variables
    uint8_t CE_PIN,CSN_PIN;

    // functions
    void CSN_Low();
    void CE_Low();
    void CSN_High();
    void CE_High();

    void powerUp();
    void powerDown();

    void setModeStandby();
    void setModeStandbyII();
    void setModeRX();
    void setModeTX();

    void setChanel(uint8_t chanel); // should be 0-127

    void writeRegister(uint8_t adress, uint8_t value);
    void writeRegister(uint8_t adress, uint8_t* values , uint8_t length);
    void readRegister(uint8_t adress, uint8_t& buffer);
    void readRegister(uint8_t adress, uint8_t* buffer , uint8_t length);

    void beginTransaction();
    void resetRegisters();

    void flushRX();
    void flushTX(); 
};





enum ARC_retransmit{
    ARC_Retransmit_None,
    ARC_Retransmit_1_Time,
    ARC_Retransmit_2_Times,
    ARC_Retransmit_3_Times,
    ARC_Retransmit_4_Times,
    ARC_Retransmit_5_Times,
    ARC_Retransmit_6_Times,
    ARC_Retransmit_7_Times,
    ARC_Retransmit_8_Times,
    ARC_Retransmit_9_Times,
    ARC_Retransmit_10_Times,
    ARC_Retransmit_11_Times,
    ARC_Retransmit_12_Times,
    ARC_Retransmit_13_Times,
    ARC_Retransmit_14_Times,
    ARC_Retransmit_15_Times,
};

enum ARD_Wait_uS{
    ARD_Wait_uS_250,
    ARD_Wait_uS_500,
    ARD_Wait_uS_750,
    ARD_Wait_uS_1000,
    ARD_Wait_uS_1250,
    ARD_Wait_uS_1500,
    ARD_Wait_uS_1750,
    ARD_Wait_uS_2000,    
    ARD_Wait_uS_2250,
    ARD_Wait_uS_2500,
    ARD_Wait_uS_2750,
    ARD_Wait_uS_3000,     
    ARD_Wait_uS_3250,
    ARD_Wait_uS_3500,
    ARD_Wait_uS_3750,
    ARD_Wait_uS_4000,
};