#include <Arduino.h>
#include <SPI.h>


/**
 * @brief Output power levels (dBm) and current consumption.
 */
enum nRF24L01_PowerMode{
    
    /**
     * -18dBm
     * 7.0 mA dc consumption
     */
    nRF24L01_PowerMode_LOW,
    /**
     * -12dBm
     * 7.5 mA dc consumption
     */
    nRF24L01_PowerMode_MEDIUM,
    /**
     * -6dBm
     * 9.0 mA dc consumption
     */
    nRF24L01_PowerMode_HIGH,
    /**
     * 0dBm
     * 11.3 mA dc consumption
     */
    nRF24L01_PowerMode_MAX,
};
/**
 * @brief Operating modes of the chip.
 */
enum nRF24L01_Mode{
    /**
     * sets the mode of the receiver to transmit
     */
    RF24L01_Mode_TRANSMIT, 
    /**
     * sets the mode of the receiver to receive
     */
    RF24L01_Mode_RECEIVE
};

/**
 * @brief Auto Retransmit Count options (0–15 retransmits).
 */
enum ARC_Retransmit{ 
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
enum ARD_Wait_uS{ // idk
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


class nRF24L01
{
private:
    // variables
    uint8_t CE_PIN;
    uint8_t CSN_PIN;

    // functions
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
     * @brief Reads multiple bytes from a register.
     * @param address Register address.
     * @param length Number of bytes to read.
     * @return Pointer to buffer with data (caller must manage memory).
     */
    uint8_t* readRegister(uint8_t adress, uint8_t length);

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
    void writeTX_Buffer(uint8_t* data);
    /**
     * @brief Reads data from the RX payload buffer.
     * @param buffer Pointer to a buffer where data will be stored.
     */
    void readRX_Buffer(uint8_t* buffer);
    /**
     * @brief Starts the transmit operation (CE pulse).
     */
    void transmit();
public:
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
    void begin();
    /**
     * @brief resets all registers to their default state
     */
    void hardReset();
    /**
     * @brief checks if a package is received and ready for reading
     */
    bool isDataAvaliable(); // TODO
    /**
     * @brief reads received data
     * @note nRF24 chip must be in receiving mode
     * @return the received data
     */
    uint8_t readData(); // TODO
    /**
     * @brief sends a package of 8 bits
     * @note nRF24 must be in transmitting mode
     */
    void send(uint8_t& data); // TODO
    /**
     * @brief Sets the RF channel frequency.
     * @param channel Channel number (0–125). Anything higher will be clamped to 125.
     * @note 1Mbps = 1MHz spacing; 2Mbps = 2MHz spacing required.
     *
     * The nRF24 operates in the frequency range of 2400-2525 MHz.
     * The formula for the frequency is:
     * F0 = 2400 + channel [MHz].
     */
    void setFrequency(uint8_t chanel); // TODO 
    /**
     * @brief Sets auto retransmit count and delay.
     * @param arc Number of retransmits (ARC).
     * @param ard Delay between retransmits (ARD) in 250µs steps.
     */
    void setRetransmits(ARC_Retransmit arc, ARD_Wait_uS ard); // TODO 1
    /**
     * @brief Sets the RF output power mode.
     * @param mode Power level (LOW, MEDIUM, HIGH, MAX).
     */
    void setPowerMode(nRF24L01_PowerMode mode); // TODO 2
    /**
     * @brief Sets the device mode.
     * @param mode Transmit or Receive.
     */
    void setMode(nRF24L01_Mode mode); // TODO 3
    /**
     * @brief Enables or disables high sensitivity mode (LNA gain).
     * @param on True to enable, false to disable.
     */
    void setHighSensitivity(bool on); // TODO
    /**
     * @brief Temporary test function for debugging.
     * @note Will be removed later.
     */
    void test(); // remove later
};
