#include "nRF24L01.h"

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
    digitalWrite(CE_PIN,HIGH);
}
void nRF24L01::CSN_High(){
    digitalWrite(CSN_PIN,HIGH);
}
void nRF24L01::CE_Low(){
    digitalWrite(CE_PIN,LOW);
}
void nRF24L01::CSN_Low(){
    digitalWrite(CSN_PIN,LOW);
}
 

void nRF24L01::writeRegister(uint8_t adress, uint8_t value){
    beginTransaction();
    CSN_Low();
    SPI.transfer(SPICOMMAND::W_REGISTER | (adress & SPICOMMAND::REGISTER_BITMASK)); // set the adress with the write command and make shure the adress is within the bitmask
    SPI.transfer(value);                                            // send the value to the adress
    CSN_High();
    endTransaction();
}

void nRF24L01::writeRegister(uint8_t adress, uint8_t* values , uint8_t length){
    beginTransaction();
    CSN_Low();
    SPI.transfer(SPICOMMAND::W_REGISTER | (adress & SPICOMMAND::REGISTER_BITMASK));// set the adress with the write command and make shure the adress is within the bitmask
    for (int i = 0; i < length; i++) {                              
        SPI.transfer(values[i]);                                   // send the values from the array to the adress
    }   
    CSN_High();
    endTransaction();
}

uint8_t nRF24L01::readRegister(uint8_t adress) {
    uint8_t buffer;
    beginTransaction();
    CSN_Low();
    SPI.transfer(SPICOMMAND::R_REGISTER | (adress & SPICOMMAND::REGISTER_BITMASK)); // set the adress with the read command and make shure the adress is within the bitmask
    buffer = SPI.transfer(SPICOMMAND::NOP);                              
    CSN_High();
    endTransaction();
    return buffer;
}

void nRF24L01::writebit(uint8_t adress, uint8_t bit, bool val){
    uint8_t reg = readRegister(adress); 
    writeRegister(adress, val ? (reg | bit) : (reg & ~bit)); 
}

void nRF24L01::writebits(uint8_t adress, uint8_t mask, uint8_t bits){
    uint8_t reg = readRegister(adress); 
    reg = reg & ~mask;
    writeRegister(adress, (reg | bits)); 
}



void nRF24L01::beginTransaction(){
    //spi speed for nRF24L01 rated to max 10mhz arduino is not(8mhz)
    //8mhz
    //most significant bit first
    //spi mode 0 
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
    CSN_Low();
}

void nRF24L01::endTransaction() {
    SPI.endTransaction();
    CSN_High();
}




void nRF24L01::resetRegisters() {
    writeRegister(CONFIG::ADRESS     , CONFIG::RESET_VALUE);
    writeRegister(EN_RXADDR::ADRESS  , EN_RXADDR::RESET_VALUE);
    writeRegister(EN_AA::ADRESS      , EN_AA::RESET_VALUE);
    writeRegister(SETUP_RETR::ADRESS , SETUP_RETR::RESET_VALUE);
    writeRegister(RF_CH::ADRESS      , RF_CH::RESET_VALUE);
    writeRegister(RF_SETUP::ADRESS   , RF_SETUP::RESET_VALUE);
    writeRegister(RX_ADDR::P0        , const_cast<uint8_t*>(RX_ADDR::P0_RESET_VALUE), 5); // van const uint8t[5] naar uint8*
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
    beginTransaction();
    SPI.transfer(SPICOMMAND::FLUSH_TX);
    endTransaction();
}

void nRF24L01::flushRX() {
    beginTransaction();
    SPI.transfer(SPICOMMAND::FLUSH_RX);
    endTransaction();
}




void nRF24L01::writeTX_Buffer(String& data) {
    flushTX();                      // Clear old data
    beginTransaction();             // Start SPI transaction

    SPI.transfer(SPICOMMAND::W_TX_PAYLOAD); // Write payload command
    const char* str = data.c_str();               

    size_t len = data.length();
    if (len > 31) len = 31;

    for (size_t i = 0; i < len; ++i) {
        SPI.transfer(str[i]);
    }
    SPI.transfer('\0'); // 32nd byte

    endTransaction();            // End SPI transaction
}


String nRF24L01::readData() {
    beginTransaction();
    SPI.transfer(SPICOMMAND::R_RX_PAYLOAD);

    String buffer;
    for (uint8_t i = 0; i < 32; ++i) {
        char c = SPI.transfer(SPICOMMAND::NOP);
        if (c == '\0') break;
        buffer += c;
    }

    writeRegister(STATUS::ADRESS, STATUS::RX_DR); // TODO look at

    endTransaction();
    return buffer;
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

    if (not CSN_PIN or not CE_PIN){ // check if csn or ce is defined
        if (not CSN_PIN){
            Serial.println("nRF24L01 begin() failed: CSN pin is not defined");
        }
        if (not CE_PIN){
            Serial.println("nRF24L01 begin() failed: CE pin is not defined");
        }
        return;
    }
    // set csn and ce to output
    pinMode(CSN_PIN, OUTPUT);
    pinMode(CE_PIN, OUTPUT);
    
    SPI.begin();
    beginTransaction(); // start spi transaction with the correct settings
    
    flushRX();
    flushTX();

    resetRegisters(); // reset the important registers for a clean start

    // power up the chip
    writebit(CONFIG::ADRESS,CONFIG::PWR_UP,true);
    delay(TIMINGS::PWR_UP_DELAY_mS); // powerdown-> standby-I minimal 1.5ms 

    endTransaction();
}





bool nRF24L01::isDataAvaliable() {
    uint8_t status = readRegister(STATUS::ADRESS);
    bool available = (status & STATUS::RX_DR);
    if (available) {
        writebit(STATUS::ADRESS, STATUS::RX_DR,true); // clear RX_DR flag
    }
    return available;
}




void nRF24L01::send(String& data) {
    setMode(nRF24L01_Mode_TRANSMIT);
    flushTX();
    writeTX_Buffer(data);
    transmit_Payload();

    // wacht tot verzonden of gefaald
    while (true) {
        uint8_t status = readRegister(STATUS::ADRESS);
        if (status & STATUS::TX_DS || status & STATUS::MAX_RT) { // check if STATUS::TX_DS or STATUS::MAX_RT bits are set
            writeRegister(STATUS::ADRESS, STATUS::TX_DS | STATUS::MAX_RT); // clear flags
            break;
        }
    }
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
        writebit(CONFIG::ADRESS, CONFIG::PRIM_RX,true); // set the prim_rx bit to 1
        CE_High(); // enable listening
    } else {
        CE_Low(); // disable listening
    }
}

void nRF24L01::setHighSensitivity(bool on) { 
    writebit(RF_SETUP::ADRESS,RF_SETUP::LNA_HCURR,on);
}



void nRF24L01::test(){
    
    writeRegister(RF_CH::ADRESS, 42);           
    uint8_t val= readRegister(RF_CH::ADRESS); 

    if (val == 42) {
        Serial.println("PASS: SPI communication works");
    } else {
        Serial.print("FAIL: Expected 42, got  ");
        Serial.println(val);
    }

    writeRegister(STATUS::ADRESS, STATUS::RESET_VALUE); // Clear IRQs
    uint8_t status = readRegister(STATUS::ADRESS);
    

    if ((status & 0x70) == 0) {
        Serial.println("PASS: IRQs cleared");
    } else {
        Serial.print("FAIL: STATUS still has flags: ");
        Serial.println(status, BIN);
    }
}




void nRF24L01::setPayloadSize(nRF24L01_PayloadSize size, nRF24L01_Pipe pipe) {
    switch (pipe)
    {
    case nRF24L01_Pipe_ALL:
        writebits(RX_PW::P1,RX_PW::P0,size);
        writebits(RX_PW::P1,RX_PW::P1,size);
        writebits(RX_PW::P2,RX_PW::P2,size);
        writebits(RX_PW::P3,RX_PW::P3,size);
        writebits(RX_PW::P4,RX_PW::P4,size);
        writebits(RX_PW::P5,RX_PW::P5,size);
        break;
    case nRF24L01_Pipe_P0:
        writebits(RX_PW::P0,RX_PW::P0,size);
        break;
    case nRF24L01_Pipe_P1:
        writebits(RX_PW::P1,RX_PW::P1,size);
        break;
    case nRF24L01_Pipe_P2:
        writebits(RX_PW::P2,RX_PW::P2,size);
        break;
    case nRF24L01_Pipe_P3:
        writebits(RX_PW::P3,RX_PW::P3,size);
        break;
    case nRF24L01_Pipe_P4:
        writebits(RX_PW::P4,RX_PW::P4,size);
        break;
    case nRF24L01_Pipe_P5:
        writebits(RX_PW::P5,RX_PW::P5,size);
        break;
    default:
        return;
    }
}


void nRF24L01::setAdressWidth(nRF24L01_AdressWidth adresswidth){
    writebits(SETUP_AW::ADRESS,SETUP_AW::AW,adresswidth);
}

void nRF24L01::setAirDataRate(nRF24L01_AirDataRate rate){
    writebits(RF_SETUP::ADRESS,RF_SETUP::DR,rate);
}

void nRF24L01::setRX_Pipe(nRF24L01_Pipe pipe, bool on){
    switch (pipe)
    {
    case nRF24L01_Pipe_ALL:
        writebits(EN_AA::ALL,EN_AA::ALL,on ?EN_AA::ALL:(~EN_AA::ALL) );
        break;
    case nRF24L01_Pipe_P0:
        writebit(EN_AA::ADRESS,EN_AA::P0,on);
        break;
    case nRF24L01_Pipe_P1:
        writebit(EN_AA::ADRESS,EN_AA::P1,on);
        break;
    case nRF24L01_Pipe_P2:
        writebit(EN_AA::ADRESS,EN_AA::P2,on);
        break;
    case nRF24L01_Pipe_P3:
        writebit(EN_AA::ADRESS,EN_AA::P3,on);
        break;
    case nRF24L01_Pipe_P4:
        writebit(EN_AA::ADRESS,EN_AA::P4,on);
        break;
    case nRF24L01_Pipe_P5:
        writebit(EN_AA::ADRESS,EN_AA::P5,on);
        break;
    default:
        return;
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
    }
    writeRegister(SETUP_AW::ADRESS, tmp); // restore original
}