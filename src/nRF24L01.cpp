#include "nRF24L01.h"

/* 
het zijn geen defines
  _       _
   \(._.)/
*/
#define nRF24L01_DEBUG



constexpr uint8_t BIT_0 = 0x01; //0000 0001 
constexpr uint8_t BIT_1 = 0x02; //0000 0010 
constexpr uint8_t BIT_2 = 0x04; //0000 0100 
constexpr uint8_t BIT_3 = 0x08; //0000 1000 
constexpr uint8_t BIT_4 = 0x10; //0001 0000 
constexpr uint8_t BIT_5 = 0x20; //0010 0000 
constexpr uint8_t BIT_6 = 0x40; //0100 0000 
constexpr uint8_t BIT_7 = 0x80; //1000 0000 


//Datasheef chapter 9.1 register map table
namespace CONFIG{ // register used to configure the chip
    inline constexpr uint8_t ADRESS      = 0x00  ;   
    inline constexpr uint8_t MASK_RX_DR  = BIT_6 ;       
    inline constexpr uint8_t MASK_TX_DS  = BIT_5 ;          
    inline constexpr uint8_t MASK_MAX_RT = BIT_4 ;          
    inline constexpr uint8_t EN_CRC      = BIT_3 ;      
    inline constexpr uint8_t CRCO        = BIT_2 ;      
    inline constexpr uint8_t PWR_UP      = BIT_1 ;      
    inline constexpr uint8_t PRIM_RX     = BIT_0 ;      
    inline constexpr uint8_t BIT_MASK    = BIT_7 ;   
    inline constexpr uint8_t RESET_VALUE = 0x08  ;   
}

namespace EN_AA{
    inline constexpr uint8_t ADRESS      = 0x01            ;                         
    inline constexpr uint8_t P5          = BIT_5           ;    
    inline constexpr uint8_t P4          = BIT_4           ;    
    inline constexpr uint8_t P3          = BIT_3           ;    
    inline constexpr uint8_t P2          = BIT_2           ;    
    inline constexpr uint8_t P1          = BIT_1           ;    
    inline constexpr uint8_t P0          = BIT_0           ;
    inline constexpr uint8_t ALL         = 0x1F            ;
    inline constexpr uint8_t BIT_MASK    = (BIT_6 | BIT_7) ;   
    inline constexpr uint8_t RESET_VALUE = 0x3F            ;    
}

namespace EN_RXADDR{               
    inline constexpr uint8_t ADRESS      = 0x02            ;   
    inline constexpr uint8_t P5          = BIT_5           ;   
    inline constexpr uint8_t P4          = BIT_4           ;   
    inline constexpr uint8_t P3          = BIT_3           ;   
    inline constexpr uint8_t P2          = BIT_2           ;   
    inline constexpr uint8_t P1          = BIT_1           ;   
    inline constexpr uint8_t P0          = BIT_0           ;   
    inline constexpr uint8_t BIT_MASK    = (BIT_6 | BIT_7) ;   
    inline constexpr uint8_t RESET_VALUE = 0x03            ;       
}

namespace SETUP_AW{
    inline constexpr uint8_t ADRESS      = 0x03                ;  
    inline constexpr uint8_t AW          = (BIT_1 | BIT_0)     ;  
    inline constexpr uint8_t AW_ILLEGAL     = 0x0              ;  
    inline constexpr uint8_t AW_3_BYTES     = 0x1              ;  
    inline constexpr uint8_t AW_4_BYTES     = 0x2              ;  
    inline constexpr uint8_t AW_5_BYTES     = 0x3              ;  
    inline constexpr uint8_t BIT_MASK    = (~(BIT_1 | BIT_0) ) ;  
    inline constexpr uint8_t RESET_VALUE = 0x03                ;  
}

namespace SETUP_RETR{
    inline constexpr uint8_t ADRESS      = 0x04                           ; 
    inline constexpr uint8_t ARD         = (BIT_7 | BIT_6 | BIT_5 | BIT_4); 
    inline constexpr uint8_t ARC         = (BIT_3 | BIT_2 | BIT_1 | BIT_0); 
    inline constexpr uint8_t RESET_VALUE = 0x03                           ; 
}

namespace RF_CH{
    inline constexpr uint8_t ADRESS            = 0x05  ; 
    inline constexpr uint8_t BIT_MASK    = BIT_7 ; 
    inline constexpr uint8_t RESET_VALUE = 0x02  ; 
}

namespace RF_SETUP{
    inline constexpr uint8_t ADRESS      = 0x06                            ; 
    inline constexpr uint8_t PLL_LOCK    = BIT_4                           ; 
    inline constexpr uint8_t DR          = BIT_3                           ; 
    inline constexpr uint8_t PWR         = (BIT_1 | BIT_2)                 ; 
    inline constexpr uint8_t LNA_HCURR   = BIT_0                           ; 
    inline constexpr uint8_t BIT_MASK    = (BIT_7 | BIT_6 | BIT_5 | BIT_4) ; 
    inline constexpr uint8_t RESET_VALUE = 0x0F                            ; 
}

namespace STATUS{
    inline constexpr uint8_t ADRESS      = 0x07                    ; 
    inline constexpr uint8_t BIT_MASK    = BIT_7                   ; 
    inline constexpr uint8_t RX_DR       = BIT_6                   ; 
    inline constexpr uint8_t TX_DS       = BIT_5                   ; 
    inline constexpr uint8_t MAX_RT      = BIT_4                   ; 
    inline constexpr uint8_t RX_P_NO     = (BIT_3 | BIT_2 | BIT_1) ; 
    inline constexpr uint8_t TX_FULL     = BIT_0                   ; 
    inline constexpr uint8_t RESET_VALUE = 0x70                    ; 
}

namespace OBSERVE_TX{
    inline constexpr uint8_t ADRESS      = 0x08                            ; 
    inline constexpr uint8_t PLOS_CNT    = (BIT_7 | BIT_6 | BIT_5 | BIT_4) ; 
    inline constexpr uint8_t ARC_CNT     = (BIT_3 | BIT_2 | BIT_1 | BIT_0) ; 
    inline constexpr uint8_t RESET_VALUE = 0x00                            ; 
}

namespace CD{
    inline constexpr uint8_t ADRESS         = 0x09     ; 
    inline constexpr uint8_t CD_BIT_MASK    = (~BIT_0) ; 
    inline constexpr uint8_t CD_RESET_VALUE = 0x00     ; 
}     

namespace RX_ADDR{
    inline constexpr uint8_t P0                = 0x0A                           ;  
    inline constexpr uint8_t P0_RESET_VALUE[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7} ;  
    inline constexpr uint8_t P1                = 0x0B                           ;  
    inline constexpr uint8_t P1_RESET_VALUE[5] = {0xC2,0xC2,0xC2,0xC2,0xC2}     ; 
    inline constexpr uint8_t P2                = 0x0C                           ;  
    inline constexpr uint8_t P2_RESET_VALUE    = 0xC3                           ;  
    inline constexpr uint8_t P3                = 0x0D                           ;  
    inline constexpr uint8_t P3_RESET_VALUE    = 0xC4                           ; 
    inline constexpr uint8_t P4                = 0x0E                           ;  
    inline constexpr uint8_t P4_RESET_VALUE    = 0xC5                           ;  
    inline constexpr uint8_t P5                = 0x0F                           ;  
    inline constexpr uint8_t P5_RESET_VALUE    = 0xC6                           ; 
}

namespace TX_ADDR{
    inline constexpr uint8_t ADRESS         = 0x10                        ;
    inline constexpr uint8_t RESET_VALUE[5] =  {0xE7,0xE7,0xE7,0xE7,0xE7} ;
}

namespace RX_PW{
    inline constexpr uint8_t P0              = 0x11            ;        
    inline constexpr uint8_t P0_RESET_VALUE  = 0x00            ;        
    inline constexpr uint8_t P0_BIT_MASK     = (BIT_6 | BIT_7) ;    
    inline constexpr uint8_t P1              = 0x12            ;        
    inline constexpr uint8_t P1_RESET_VALUE  = 0x00            ;        
    inline constexpr uint8_t P1_BIT_MASK     = (BIT_6 | BIT_7) ;        
    inline constexpr uint8_t P2              = 0x13            ;        
    inline constexpr uint8_t P2_RESET_VALUE  = 0x00            ;        
    inline constexpr uint8_t P2_BIT_MASK     = (BIT_6 | BIT_7) ;    
    inline constexpr uint8_t P3              = 0x14            ;        
    inline constexpr uint8_t P3_RESET_VALUE  = 0x00            ;        
    inline constexpr uint8_t P3_BIT_MASK     = (BIT_6 | BIT_7) ;    
    inline constexpr uint8_t P4              = 0x15            ;        
    inline constexpr uint8_t P4_RESET_VALUE  = 0x00            ;        
    inline constexpr uint8_t P4_BIT_MASK     = (BIT_6 | BIT_7) ;    
    inline constexpr uint8_t P5              = 0x16            ;        
    inline constexpr uint8_t P5_RESET_VALUE  = 0x00            ;        
    inline constexpr uint8_t P5_BIT_MASK     = (BIT_6 | BIT_7) ;    
}

namespace FIFO_STATUS{
    inline constexpr uint8_t ADRESS               = 0x17                    ; 
    inline constexpr uint8_t TX_REUSE             = BIT_6                   ; 
    inline constexpr uint8_t TX_FULL              = BIT_5                   ; 
    inline constexpr uint8_t TX_EMPTY             = BIT_4                   ; 
    inline constexpr uint8_t FIFO_STATUS_BIT_MASK = (BIT_7 | BIT_3 | BIT_2) ; 
    inline constexpr uint8_t RX_FULL              = BIT_1                   ; 
    inline constexpr uint8_t RX_EMPTY             = BIT_0                   ; 
    inline constexpr uint8_t RESET_VALUE          = 0x11                    ; 
}

namespace DYNPD{
    inline constexpr uint8_t ADRESS      = 0x1C            ; 
    inline constexpr uint8_t BIT_MASK    = (BIT_6 | BIT_7) ; 
    inline constexpr uint8_t DPL_P5      = BIT_5           ; 
    inline constexpr uint8_t DPL_P4      = BIT_4           ; 
    inline constexpr uint8_t DPL_P3      = BIT_3           ; 
    inline constexpr uint8_t DPL_P2      = BIT_2           ; 
    inline constexpr uint8_t DPL_P1      = BIT_1           ; 
    inline constexpr uint8_t DPL_P0      = BIT_0           ; 
    inline constexpr uint8_t RESET_VALUE = 0x00            ; 
}

namespace FEATURE{
    inline constexpr uint8_t ADRESS      = 0x1D                                   ; 
    inline constexpr uint8_t BIT_MASK    = (BIT_7 | BIT_6 | BIT_5 | BIT_4 | BIT_3); 
    inline constexpr uint8_t EN_DPL      = BIT_2                                  ; 
    inline constexpr uint8_t EN_ACK_PAYd = BIT_1                                  ; 
    inline constexpr uint8_t EN_DYN_ACK  = BIT_0                                  ; 
    inline constexpr uint8_t RESET_VALUE = 0x00                                   ; 
}

//Datasheet chapter SPI commands 8.x
namespace SPICOMMAND{
    inline constexpr uint8_t R_REGISTER           = 0x00 ;  //bit 7-5, AAAAA = register adress (000A AAAA)  used for reading a register
    inline constexpr uint8_t W_REGISTER           = 0x20 ;  //bit 7-5, AAAAA = register adress (001A AAAA)  used for writing a register
    inline constexpr uint8_t R_RX_PAYLOAD         = 0x61 ;  //0110 0001 
    inline constexpr uint8_t W_TX_PAYLOAD         = 0xA0 ;  //1010 0000 
    inline constexpr uint8_t FLUSH_TX             = 0xE1 ;  //1110 0001
    inline constexpr uint8_t FLUSH_RX             = 0xE2 ;  //1110 0010
    inline constexpr uint8_t REUSE_TX_PL          = 0xE3 ;  //1110 0011
    inline constexpr uint8_t ACTIVATE             = 0x50 ;  //0101 0000
    inline constexpr uint8_t R_RX_PL_WIDa         = 0x60 ;  //0110 0000
    inline constexpr uint8_t W_ACK_PAYLOAD        = 0xA8 ;  //1010 1PPP
    inline constexpr uint8_t W_TX_PAYLOAD_NOACK   = 0x58 ;  //1011 000
    inline constexpr uint8_t NOP                  = 0xFF ;  //1111 1111
    inline constexpr uint8_t REGISTER_BITMASK     = 0x1F ;  //0001 1111 ^ 000AAAAA the AAAAA from above
}

// 6.1.7 Timing Information
namespace TIMINGS{
    inline constexpr uint8_t PWR_UP_DELAY_mS = 2; // 1.5 min
    inline constexpr uint8_t PWR_UP_DELAY_EXTERNAL_CLK_uS = 180; //150 uS
    inline constexpr uint8_t RX_TX_SETTLING_DELAY_uS = 160; // 130 uS 
    inline constexpr uint8_t TRANSITION_TX_CE_HIGH_uS = 15; // min 15 uS
}


//----------------------------------------------------SPI READ/WRITE-------------------------------


void nRF24L01::CE_High(){
    digitalWrite(CE_PIN,HIGH); // sets ce pin high
}
void nRF24L01::CSN_High(){
    digitalWrite(CSN_PIN,HIGH); // sets csn pin high
}
void nRF24L01::CE_Low(){
    digitalWrite(CE_PIN,LOW); // sets ce pin low
}
void nRF24L01::CSN_Low(){
    digitalWrite(CSN_PIN,LOW); // sets ce pin low
}
 

void nRF24L01::writeRegister(uint8_t adress, uint8_t value){
    beginTransaction();
    // sends the adress with the correct bitmask
    SPI.transfer(SPICOMMAND::W_REGISTER | (adress & SPICOMMAND::REGISTER_BITMASK));

    // sends the value   
    SPI.transfer(value);                                          
    endTransaction();
}

void nRF24L01::writeRegister(uint8_t adress, uint8_t* values , uint8_t length){
    beginTransaction();

    // set the adress with the write command and make shure the adress is within the bitmask
    SPI.transfer(SPICOMMAND::W_REGISTER | (adress & SPICOMMAND::REGISTER_BITMASK));

    // sends all values in the array to the adress  
    for (int i = 0; i < length; i++) {                       
        SPI.transfer(values[i]);  
    }   
    endTransaction();
}

uint8_t nRF24L01::readRegister(uint8_t adress) {
    beginTransaction();

    // set the adress with the read command and make shure the adress is within the bitmask
    SPI.transfer(SPICOMMAND::R_REGISTER | (adress & SPICOMMAND::REGISTER_BITMASK)); 

    // sends an dummy byte to only receive data  
    uint8_t buffer = SPI.transfer(SPICOMMAND::NOP);                      
    endTransaction();
    return buffer;
}

void nRF24L01::writebit(uint8_t adress, uint8_t bit, bool val){
    uint8_t reg = readRegister(adress); 
    // sets a bit on or off based on the (val) vraiable
    writeRegister(adress, val ? (reg | bit) : (reg & ~bit)); 
}

void nRF24L01::writebits(uint8_t adress, uint8_t mask, uint8_t bits){
    uint8_t reg = readRegister(adress); 
    // turns off all bits in the mask
    reg = reg & ~mask; 
    // sets the givven bits high
    writeRegister(adress, (reg | bits));  
}

void nRF24L01::beginTransaction(){
    //spi speed for nRF24L01 rated to max 10mhz arduino is not(8mhz)
    //most significant bit first
    //spi mode 0 
    SPI.beginTransaction(SPISettings(8000000 /*8mhz*/, MSBFIRST, SPI_MODE0));
    CSN_Low(); // enables spi for the chip
}

void nRF24L01::endTransaction() {
    SPI.endTransaction();
    CSN_High(); // disables spi for the chip
}




void nRF24L01::resetRegisters() {
    writeRegister(CONFIG::ADRESS     , CONFIG::RESET_VALUE);
    writeRegister(EN_RXADDR::ADRESS  , EN_RXADDR::RESET_VALUE);
    writeRegister(EN_AA::ADRESS      , EN_AA::RESET_VALUE);
    writeRegister(SETUP_RETR::ADRESS , SETUP_RETR::RESET_VALUE);
    writeRegister(RF_CH::ADRESS      , RF_CH::RESET_VALUE);
    writeRegister(RF_SETUP::ADRESS   , RF_SETUP::RESET_VALUE);
    writeRegister(RX_ADDR::P0        , const_cast<uint8_t*>(RX_ADDR::P0_RESET_VALUE), 5); //constexpr makes const uint8_t so cast to non const uint8_t
    writeRegister(STATUS::ADRESS     , STATUS::RESET_VALUE);
}

void nRF24L01::hardReset() {
    resetRegisters();
    writeRegister(SETUP_AW::ADRESS    , SETUP_AW::RESET_VALUE);
    writeRegister(RX_ADDR::P1         , const_cast<uint8_t*>(RX_ADDR::P1_RESET_VALUE),5);
    writeRegister(RX_ADDR::P2         , RX_ADDR::P2_RESET_VALUE);
    writeRegister(RX_ADDR::P3         , RX_ADDR::P3_RESET_VALUE);
    writeRegister(RX_ADDR::P4         , RX_ADDR::P4_RESET_VALUE);
    writeRegister(RX_ADDR::P5         , RX_ADDR::P5_RESET_VALUE);
    writeRegister(TX_ADDR::ADRESS     , const_cast<uint8_t*>(TX_ADDR::RESET_VALUE),5);
    writeRegister(RX_PW::P0           , RX_PW::P0_RESET_VALUE);
    writeRegister(RX_PW::P1           , RX_PW::P1_RESET_VALUE);
    writeRegister(RX_PW::P2           , RX_PW::P2_RESET_VALUE);
    writeRegister(RX_PW::P3           , RX_PW::P3_RESET_VALUE);
    writeRegister(RX_PW::P4           , RX_PW::P4_RESET_VALUE);
    writeRegister(RX_PW::P5           , RX_PW::P5_RESET_VALUE);
    writeRegister(FIFO_STATUS::ADRESS , FIFO_STATUS::RESET_VALUE);
    writeRegister(DYNPD::ADRESS       , DYNPD::RESET_VALUE);
    writeRegister(FEATURE::ADRESS     , FEATURE::RESET_VALUE);
}




void nRF24L01::flushTX() {
    #ifdef nRF24L01_DEBUG
        Serial.println("nRF24L01: flushing TX");
    #endif 
    beginTransaction();
    SPI.transfer(SPICOMMAND::FLUSH_TX);
    endTransaction();
}

void nRF24L01::flushRX() {
    #ifdef nRF24L01_DEBUG
        Serial.println("nRF24L01: flushing RX");
    #endif 
    beginTransaction();
    SPI.transfer(SPICOMMAND::FLUSH_RX);
    endTransaction();
}




void nRF24L01::writeTX_Buffer(String& data) {
    #ifdef nRF24L01_DEBUG
        Serial.println("nRF24L01: flushing RX");
    #endif 
    beginTransaction();
    SPI.transfer(SPICOMMAND::W_TX_PAYLOAD);
    const char* str = data.c_str();
    size_t len = data.length();
    if (len > 32) len = 32;
    for (size_t i = 0; i < len; ++i) {
        SPI.transfer(str[i]);
    }
    endTransaction();
}


// In nRF24L01.cpp

String nRF24L01::readData() {
    if (!(readRegister(STATUS::ADRESS) & STATUS::RX_DR)) {
        #ifdef nRF24L01_DEBUG
            Serial.println("nRF24L01: RX_DR not set");
        #endif
        return ""; // No data available
    }

    uint8_t payloadLength = getDynamicPayloadLength(); // Get actual length
    if (payloadLength == 0 || payloadLength > 32) {
        flushRX(); // Corrupt or unexpected packet
        writebit(STATUS::ADRESS, STATUS::RX_DR, true); // clear RX_DR flag
        #ifdef nRF24L01_DEBUG
            Serial.println("nRF24L01: invalid payload");
        #endif
        return "";
    }
   

    beginTransaction();
    SPI.transfer(SPICOMMAND::R_RX_PAYLOAD);

    String result;
    result.reserve(payloadLength + 1); // accound for \0

    for (uint8_t i = 0; i < payloadLength; ++i) {
        char c = SPI.transfer(SPICOMMAND::NOP);
        result += c;
    }
    endTransaction();

    #ifdef nRF24L01_DEBUG
        Serial.println("nRF24L01: packet receivced successfully");
    #endif

    writebit(STATUS::ADRESS, STATUS::RX_DR, true); // clear RX_DR flag
    return result;
}




void nRF24L01::transmit_Payload(){
    CE_High();
    delayMicroseconds(TIMINGS::TRANSITION_TX_CE_HIGH_uS); // 10+ uS puls
    CE_Low();
}


nRF24L01::nRF24L01(){}

nRF24L01::nRF24L01(uint8_t CE_PIN,uint8_t CSN_PIN) : CE_PIN(CE_PIN), CSN_PIN(CSN_PIN){} // sets ce and csn

nRF24L01::~nRF24L01(){
    writeRegister(
        CONFIG::ADRESS,
        readRegister(CONFIG::ADRESS) & ~CONFIG::PWR_UP
    ); // power down the chip to minimise power consumption
}




void nRF24L01::set_CE_Pin(uint8_t CE_PIN) {
    this->CE_PIN = CE_PIN; // sets ce
}
void nRF24L01::set_CSN_Pin(uint8_t CSN_PIN){
    this->CSN_PIN = CSN_PIN; // setc csn
}





void nRF24L01::begin(){

    #ifdef nRF24L01_DEBUG
        if (not CSN_PIN or not CE_PIN){ // check if csn or ce is defined
            if (not CSN_PIN){
                Serial.println("nRF24L01 begin() failed: CSN pin is not defined");
            }
            if (not CE_PIN){
                Serial.println("nRF24L01 begin() failed: CE pin is not defined");
            }
        }
    #endif

    // set csn and ce to output
    pinMode(CSN_PIN, OUTPUT);
    pinMode(CE_PIN, OUTPUT);
    
    SPI.begin();
    delay(1); // make shure spi started
    
    #ifdef nRF24L01_DEBUG
        testConnection();
    #endif

    flushRX();
    flushTX();

    resetRegisters(); // reset the important registers for a clean start

    writebit(CONFIG::ADRESS,CONFIG::PWR_UP,true);
    delay(TIMINGS::PWR_UP_DELAY_mS); // powerdown-> standby-I minimal 1.5ms 
}





bool nRF24L01::isDataAvaliable() {
    uint8_t status = readRegister(STATUS::ADRESS);
    bool available = (status & STATUS::RX_DR);
    #ifdef nRF24L01_DEBUG
        if (available) {
            Serial.println("nRF24L01: data is available");
        }
    #endif
    
    return available;
}




nRF24L01_Status nRF24L01::send(String& data) {
    writeTX_Buffer(data);
    transmit_Payload(); // CE pulse

    // Wait for TX_DS or MAX_RT
    unsigned long startTime = millis();
    while (true) {
        uint8_t status = readRegister(STATUS::ADRESS);
        if (status & STATUS::TX_DS) {
            writebit(STATUS::ADRESS, STATUS::TX_DS, true); // Clear
            break;
        }
        if (status & STATUS::MAX_RT) {
            writebit(STATUS::ADRESS, STATUS::MAX_RT, true); // Clear
            flushTX(); // Flush the failed payload
            return nRF24L01_Status::ERROR_MAX_RT_REACHED;
        }
        if (millis() - startTime > 95) {
            return nRF24L01_Status::ERROR_TX_FIFO_FULL;
        }
    }

    // Optional: Wait until TX FIFO is empty (not strictly needed)
    while (!(readRegister(FIFO_STATUS::ADRESS) & FIFO_STATUS::TX_EMPTY)) {
        delayMicroseconds(100);
    }

    return nRF24L01_Status::nRF24L01_Status_OK;
}




void nRF24L01::setFrequency(uint8_t channel) {
    if (channel > 125) return;
    writeRegister(RF_CH::ADRESS, channel);
}

void nRF24L01::setRetransmits(ARC_Retransmit arc, ARD_Wait_uS ard) {
    uint8_t value = (ard << 4) | arc;
    writeRegister(SETUP_RETR::ADRESS, value);
}

void nRF24L01::setPowerMode(nRF24L01_PowerMode mode) {
    writebits(RF_SETUP::ADRESS, RF_SETUP::PWR,(mode << 1) );
}

void nRF24L01::setMode(nRF24L01_Mode mode) {
    if (mode == nRF24L01_Mode_RECEIVE) {
        writebit(CONFIG::ADRESS, CONFIG::PRIM_RX, true);
        CE_High();
        delay(TIMINGS::RX_TX_SETTLING_DELAY_uS);
    } else {
        CE_Low();
        writebit(CONFIG::ADRESS, CONFIG::PRIM_RX, false);
        delay(TIMINGS::RX_TX_SETTLING_DELAY_uS);
    }
}

void nRF24L01::setHighSensitivity(bool on) { 
    writebit(RF_SETUP::ADRESS,RF_SETUP::LNA_HCURR,on);
}






void nRF24L01::setPayloadSize(nRF24L01_PayloadSize size, nRF24L01_Pipe pipe) { // chat
    uint8_t val = static_cast<uint8_t>(size);
    if (val > 32) val = 32;
    switch (pipe) {
        case nRF24L01_Pipe_ALL:
            for (uint8_t i = 0; i <= 5; ++i)
                writeRegister(RX_PW::P0 + i, val);
            break;
        default:
            writeRegister(RX_PW::P0 + static_cast<uint8_t>(pipe), val);
            break;
    }
}



void nRF24L01::setAdressWidth(nRF24L01_AdressWidth adresswidth){
    writebits(SETUP_AW::ADRESS,SETUP_AW::AW,adresswidth);
}

void nRF24L01::setAirDataRate(nRF24L01_AirDataRate rate){
    writebits(RF_SETUP::ADRESS,RF_SETUP::DR,rate);
}


/*
void nRF24L01::openReadingPipe(uint8_t pipe, const uint8_t* address, uint8_t len) {
    writeRegister(EN_RXADDR, readRegister(EN_RXADDR) | (1 << pipe));
    if (pipe <= 1) {
        writeRegister(RX_ADDR::P0 + pipe, address, len);
    } else {
        writeRegister(RX_ADDR_P0 + pipe, &address[0], 1); // Only LSB
    }
}*/

void nRF24L01::setRX_Pipe(nRF24L01_Pipe pipe, bool on) { // chat
    if (pipe == nRF24L01_Pipe_ALL) {
        writeRegister(EN_AA::ADRESS, on ? EN_AA::ALL : 0x00);
    } else {
        uint8_t bit = 1 << static_cast<uint8_t>(pipe);
        uint8_t reg = readRegister(EN_AA::ADRESS);
        writeRegister(EN_AA::ADRESS, on ? (reg | bit) : (reg & ~bit));
    }
}


void nRF24L01::setTX_adress(const uint8_t* address, uint8_t length) { // chat
    beginTransaction();
    SPI.transfer(SPICOMMAND::W_REGISTER | 0x10); // TX_ADDR register
    for (uint8_t i = 0; i < length; ++i) {
        SPI.transfer(address[i]);
    }
    endTransaction();
}

void nRF24L01::setRX_Address(nRF24L01_Pipe pipe, const uint8_t* address, uint8_t length) { // chat
    if (pipe > nRF24L01_Pipe_P5) return;

    beginTransaction();
    SPI.transfer(SPICOMMAND::W_REGISTER | (0x0A + pipe)); // RX_ADDR_P0–P5 = 0x0A–0x0F

    // Pipe 0 and 1 use full address, pipes 2–5 only use LSB
    if (pipe <= nRF24L01_Pipe_P1) {
        for (uint8_t i = 0; i < length; ++i) {
            SPI.transfer(address[i]);
        }
    } else {
        SPI.transfer(address[0]); // Only least significant byte
    }

    endTransaction();
}



void nRF24L01::testConnection(){
    uint8_t tmp = readRegister(SETUP_AW::ADRESS);
    writeRegister(SETUP_AW::ADRESS, 0x03); // test write (5-byte address width)
    if (readRegister(SETUP_AW::ADRESS) != 0x03){
        Serial.println("nRF24L01 begin() failed: SPI connection failed, please check your wiring");
        return;
    } else{
        Serial.println("SPI works");
    }
    writeRegister(SETUP_AW::ADRESS, tmp); // restore original
}

void nRF24L01::toggleFeatures() { // TODO chat
    beginTransaction();
    SPI.transfer(SPICOMMAND::ACTIVATE);
    SPI.transfer(0x73);
    endTransaction();
}


void nRF24L01::setDynamicPayload(bool on) {
    toggleFeatures();
    writebit(FEATURE::ADRESS, FEATURE::EN_DPL, on);
    writeRegister(DYNPD::ADRESS, on ? 0x3F : 0x00);
}

uint8_t nRF24L01::getDynamicPayloadLength() {
    beginTransaction();
    SPI.transfer(SPICOMMAND::R_RX_PL_WIDa); // Command to read payload width
    uint8_t length = SPI.transfer(SPICOMMAND::NOP); // Get the length byte
    endTransaction();
    return length;
}