#include <Arduino.h>
#include <SPI.h>

//#define nRF24L01_DEBUG
//#define nRF24L01_TESTS


//TODO 1/2 mbps for chanel

// TODO 
/*
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
*/



/**
 * @brief Output power levels (dBm) and current consumption.
 */
enum nRF24L01_PowerMode : uint8_t{
    
    /**
     * -18dBm
     * 7.0 mA dc consumption a pulse
     */
    nRF24L01_PowerMode_NEGATIVE_18_dBm,
    /**
     * -12dBm
     * 7.5 mA dc consumption a pulse
     */
    nRF24L01_PowerMode_NEGATIVE_12_dBm,
    /**
     * -6dBm
     * 9.0 mA dc consumption a pulse
     */
    nRF24L01_PowerMode_NEGATIVE_6_dBm,
    /**
     * 0dBm
     * 11.3 mA dc consumption a pulse
     */
    nRF24L01_PowerMode_0_dBm,
};

/**
 * @brief Operating modes of the chip.
 */
enum nRF24L01_Mode : uint8_t{
    /**
     * sets the mode of the receiver to transmit
     */
    nRF24L01_Mode_TRANSMIT, 
    /**
     * sets the mode of the receiver to receive
     */
    nRF24L01_Mode_RECEIVE
};

/**
 * @brief Auto Retransmit Count options (0–15 retransmits).
 */
enum ARC_Retransmit : uint8_t{ // TODO Comment
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

/**
 * @brief Auto Retransmit Delay options (250–4000µs).
 */
enum ARD_Wait_uS : uint8_t{ // TODO Comment
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

enum nRF24L01_AdressWidth : uint8_t{ // TODO Comment
    nRF24L01_AdressWidth_ILLEGAL,
    nRF24L01_AdressWidth_3_Bytes,
    nRF24L01_AdressWidth_4_Bytes,
    nRF24L01_AdressWidth_5_Bytes
};

enum nRF24L01_AirDataRate : uint8_t{ // TODO Comment
    nRF24L01_AirDataRate_1_Mbps,
    nRF24L01_AirDataRate_2_Mbps
};

enum nRF24L01_Pipe : uint8_t{// TODO Comment
    nRF24L01_Pipe_P0,
    nRF24L01_Pipe_P1,
    nRF24L01_Pipe_P2,
    nRF24L01_Pipe_P3,
    nRF24L01_Pipe_P4,
    nRF24L01_Pipe_P5,
    nRF24L01_Pipe_ALL
};


enum nRF24L01_PayloadSize : uint8_t{// TODO Comment
    nRF24L01_PayloadSize_NOT_USED,
    nRF24L01_PayloadSize_1_BYTE,
    nRF24L01_PayloadSize_2_BYTES,
    nRF24L01_PayloadSize_3_BYTES,
    nRF24L01_PayloadSize_4_BYTES,
    nRF24L01_PayloadSize_5_BYTES,
    nRF24L01_PayloadSize_6_BYTES,
    nRF24L01_PayloadSize_7_BYTES,
    nRF24L01_PayloadSize_8_BYTES,
    nRF24L01_PayloadSize_9_BYTES,
    nRF24L01_PayloadSize_10_BYTES,
    nRF24L01_PayloadSize_11_BYTES,
    nRF24L01_PayloadSize_12_BYTES,
    nRF24L01_PayloadSize_13_BYTES,
    nRF24L01_PayloadSize_14_BYTES,
    nRF24L01_PayloadSize_15_BYTES,
    nRF24L01_PayloadSize_16_BYTES,
    nRF24L01_PayloadSize_17_BYTES,
    nRF24L01_PayloadSize_18_BYTES,
    nRF24L01_PayloadSize_19_BYTES,
    nRF24L01_PayloadSize_20_BYTES,
    nRF24L01_PayloadSize_21_BYTES,
    nRF24L01_PayloadSize_22_BYTES,
    nRF24L01_PayloadSize_23_BYTES,
    nRF24L01_PayloadSize_24_BYTES,
    nRF24L01_PayloadSize_25_BYTES,
    nRF24L01_PayloadSize_26_BYTES,
    nRF24L01_PayloadSize_27_BYTES,
    nRF24L01_PayloadSize_28_BYTES,
    nRF24L01_PayloadSize_29_BYTES,
    nRF24L01_PayloadSize_30_BYTES,
    nRF24L01_PayloadSize_31_BYTES,
    nRF24L01_PayloadSize_32_BYTES,
};


enum nRF24L01_Status : uint8_t{ // TODO comment
    nRF24L01_Status_OK,
    nRF24L01_Status_ERROR_DEVICE_NOT_FOUND,
    nRF24L01_Status_ERROR_NO_CSN_PIN_FOUND,
    nRF24L01_Status_ERROR_NO_CE_PIN_FOUND,
    ERROR_INCORRECT_WIRERING,
    ERROR_MAX_RT_REACHED,             // Max number of TX retries reached
    ERROR_TX_FIFO_FULL,               // Cannot write payload, TX FIFO full
    ERROR_RX_FIFO_EMPTY,              // No data to read
    ERROR_ACK_NOT_RECEIVED,           // ACK expected but not received
    ERROR_PIPE_DISABLED,              // Reading from a pipe that is not enabled
};

class nRF24L01
{
private:
    uint8_t CE_PIN;
    uint8_t CSN_PIN;
    nRF24L01_Status status;


    uint8_t getDynamicPayloadLength();

    void writebit(uint8_t adress, uint8_t bit, bool val); // TODO Comment
    void writebits(uint8_t adress, uint8_t mask, uint8_t bits);// TODO Comment

    /**
     * @brief sets the CSN pin to low
     */
    void CSN_Low();


    /**
     * @brief sets the CE pin to Low
     */
    void CE_Low();


    /**
     * @brief sets the CSN pin to high
     */
    void CSN_High();


    /**
     * @brief sets the CE pin to high
     */
    void CE_High();


    /**
     * @brief Writes a single byte to a register.
     * @param address Register address.
     * @param value Value to write.
     */
    void writeRegister(uint8_t adress, uint8_t value);


    /**
     * @brief Writes multiple bytes to a register.
     * @param address Register address.
     * @param values Pointer to the data buffer.
     * @param length Number of bytes to write.
     */
    void writeRegister(uint8_t adress, uint8_t* values , uint8_t length);


    /**
     * @brief Reads a single byte from a register.
     * @param address Register address.
     * @return Value read.
     */
    uint8_t readRegister(uint8_t adress);


    /**
     * @brief Begins SPI transaction with the correct settings.
     * 8.0 mhz
     * MSB first
     * SPI mode 0
     */ 
    void beginTransaction();


    /**
     * @brief Ends SPI transaction (calls SPI.endTransaction).
     */
    void endTransaction();


    /**
     * @brief resets important registers to their default state
     * resets: CONFIG, EN_RXADDR,EN_AA, SETUP_RETR, RF_CH, RF_SETUP, RX_ADDR_P0, STATUS, 
     */
    void resetRegisters();


    /**
     * @brief Clears the RX FIFO buffer.
     */
    void flushRX();


     /**
     * @brief Clears the TX FIFO buffer.
     */
    void flushTX(); 


    /**
     * @brief Writes data to the TX payload buffer.
     * @param data Pointer to the data buffer (max 32 bytes).
     */
    void writeTX_Buffer(String& data);


    /**
     * @brief Reads data from the RX payload buffer.
     * @param buffer Pointer to a buffer where data will be stored.
     */
    String readRX_Buffer();


    /**
     * @brief Starts the transmit operation (CE pulse).
     */
    void transmit_Payload(); 
    void toggleFeatures();
public:
    void setAdressWidth(nRF24L01_AdressWidth adresswidth); // TODO comment


    void setAirDataRate(nRF24L01_AirDataRate rate); // TODO comment

    
    void setRX_Pipe(nRF24L01_Pipe pipe,bool on); // TODO comment

    nRF24L01_Status getStatus(); // TODO make and comment
    

    /**
     * @brief creates an nRF24 controler class
     * @note the CE and CSN pin must be provided before calling begin
     */
    nRF24L01();


    /**
     * @brief creates an nRF24 controler class and sets the CE and CSN 
     * @param CE_PIN chip enable output pin needed for SPI
     * @param CSN_PIN chip select not output pin needed for SPI
     */
     nRF24L01(uint8_t CE_PIN,uint8_t CSN_PIN);


    /**
     * @brief powers off the chip
     */
     ~nRF24L01();


    /**
     * @brief sets the CE_PIN (chip enable) pin needed for spi
     */
    void set_CE_Pin(uint8_t CE_PIN);


    /**
     * @brief sets the CSN_PIN (chip select not) pin needed for spi
     */
    void set_CSN_Pin(uint8_t CSN_PIN);


    /**
     * @brief starts the nRF24 chip and resets registers to their default state
     */
    void begin(); // TODO returns nRF24L01_Status


    /**
     * @brief resets all registers to their default state
     */
    void hardReset(); // TODO returns nRF24L01_Status


    /**
     * @brief checks if a package is received and ready for reading
     */
    bool isDataAvaliable(); // TODO


    /**
     * @brief reads received data
     * @note nRF24 chip must be in receiving mode
     * @return the received data
     */
    String readData(); // TODO


    /**
     * @brief sends a package in c_str style
     * @note nRF24 must be in transmitting mode
     */ 
    nRF24L01_Status send(String& data); // TODO returns nRF24L01_Status


    /**
     * @brief Sets the RF channel frequency.
     * @param channel Channel number (0–125). Anything higher will be clamped to 125.
     * @note 1Mbps = 1MHz spacing; 2Mbps = 2MHz spacing required.
     *
     * The nRF24 operates in the frequency range of 2400-2525 MHz.
     * The formula for the frequency is:
     * F0 = 2400 + channel [MHz].
     */
    void setFrequency(uint8_t chanel); // TODO 2mhz spacing // TODO returns nRF24L01_Status


    /**
     * @brief Sets auto retransmit count and delay.
     * @param arc Number of retransmits (ARC).
     * @param ard Delay between retransmits (ARD) in 250µs steps.
     */
    void setRetransmits(ARC_Retransmit arc, ARD_Wait_uS ard); // TODO returns nRF24L01_Status


    /**
     * @brief Sets the RF output power mode.
     * @param mode Power level (LOW, MEDIUM, HIGH, MAX).
     */
    void setPowerMode(nRF24L01_PowerMode mode); // TODO returns nRF24L01_Status


    /**
     * @brief Sets the device mode.
     * @param mode Transmit or Receive.
     */
    void setMode(nRF24L01_Mode mode); // TODO returns nRF24L01_Status


    /**
     * @brief Enables or disables high sensitivity mode (LNA gain).
     * @param on True to enable, false to disable.
     */
    void setHighSensitivity(bool on); // TODO returns nRF24L01_Status


    /**
     * @brief Temporary test function for debugging.
     * @note Will be removed later.
     */
    void test(); // remove later


    void setPayloadSize(nRF24L01_PayloadSize size, nRF24L01_Pipe pipe); // TODO comment


    void setTX_adress(const uint8_t* address,uint8_t length); // TODO Comment


    void setRX_Address(nRF24L01_Pipe pipe, const uint8_t* address, uint8_t length); // TODO Comment


    void testConnection(); // TODO Comment

    void setDynamicPayload(bool on); // TODO Comment
};


