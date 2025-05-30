#include "nRF24L01.h"




//------------------------------------------------ADRESSES------------------------------------------------
#define BIT_0 0x01 //0000 0001 
#define BIT_1 0x02 //0000 0010 
#define BIT_2 0x04 //0000 0100 
#define BIT_3 0x08 //0000 1000 
#define BIT_4 0x10 //0001 0000 
#define BIT_5 0x20 //0010 0000 
#define BIT_6 0x40 //0100 0000 
#define BIT_7 0x80 //1000 0000 

//Datasheed chapter 9.1 register map table

//---------------------------------------------CONFIG--------------------------------------------------------
#define CONFIG                  0x00                        // register used to configure the chip
#define MASK_RX_DR              BIT_6     
#define MASK_TX_DS              BIT_5        
#define MASK_MAX_RT             BIT_4        
#define EN_CRC                  BIT_3    
#define CRCO                    BIT_2    
#define PWR_UP                  BIT_1    
#define PRIM_RX                 BIT_0    
#define CONFIG_BIT_MASK         BIT_7 
#define CONFIG_RESET_VALUE      0x08                        //0000 1000 reset values for each bit

//---------------------------------------------EN_AA--------------------------------------------------------
#define EN_AA                   0x01                        // register used to TODO
#define ENAA_P5                 BIT_5 
#define ENAA_P4                 BIT_4 
#define ENAA_P3                 BIT_3 
#define ENAA_P2                 BIT_2 
#define ENAA_P1                 BIT_1 
#define ENAA_P0                 BIT_0 
#define EN_AA_BIT_MASK          (BIT_6 | BIT_7) 
#define EN_AA_RESET_VALUE       0x3F //0011 1111

//---------------------------------------------EN_RXADDR--------------------------------------------------------
#define EN_RXADDR               0x02                        // register used to TODO
#define ERX_P5                  BIT_5 
#define ERX_P4                  BIT_4 
#define ERX_P3                  BIT_3 
#define ERX_P2                  BIT_2 
#define ERX_P1                  BIT_1 
#define ERX_P0                  BIT_0 
#define EN_RXADDR_BIT_MASK      (BIT_6 | BIT_7)  
#define EN_RXADDR_RESET_VALUE  0x03                         //0000 0011
//---------------------------------------------SETUP_AW--------------------------------------------------------
#define SETUP_AW                0x03                        // register used to TODO
#define AW                      (BIT_1 | BIT_0)  
#define AW_ILLEGAL              0x0 
#define AW_3_BYTES              0x1
#define AW_4_BYTES              0x2
#define AW_5_BYTES              0x3 
#define AW_BIT_MASK             (~(BIT_1 | BIT_0) ) // bits 7-2
#define SETUP_AW_RESET_VALUE    0x03                        //000 00011
//---------------------------------------------SETUP_RETR--------------------------------------------------------
#define SETUP_RETR              0x04                        // register used to TODO
#define ARD                     (BIT_7 | BIT_6 | BIT_5 | BIT_4) // bits 7-4
#define ARC                     (BIT_3 | BIT_2 | BIT_1 | BIT_0) // bits 3-0
#define SETUP_RETR_RESET_VALUE  0x03                        //0000 0011

//---------------------------------------------RF_CH_ADDR--------------------------------------------------------
#define RF_CH                   0x05                        // register used to TODO
#define RF_CH_BIT_MASK          BIT_7
#define RF_CH_RESET_VALUE       0x02                        //0000 0010
//---------------------------------------------RF_SETUP--------------------------------------------------------
#define RF_SETUP                0x06                        // register used to TODO
#define PLL_LOCK                BIT_4
#define RF_DR                   BIT_3
#define RF_PWR                  (BIT_1 | BIT_2) 
#define RF_PWR_18dBm            0x0
#define RF_PWR_12dBm            0x1
#define RF_PWR_0dBm             0x2
#define RF_PWR_6dBm             0x3
#define LNA_HCURR               BIT_0
#define RF_SETUP_BIT_MASK       (BIT_7 | BIT_6 | BIT_5 | BIT_4)
#define RF_SETUP_RESET_VALUE    0x0F                        //0000 1111
//---------------------------------------------STATUS--------------------------------------------------------
#define STATUS                  0x07                        // register used to TODO
#define STATUS_BIT_MASK         BIT_7 
#define STATUS_RX_DR                   BIT_6 
#define STATUS_TX_DS                   BIT_5 
#define STATUS_MAX_RT                  BIT_4 
#define STATUS_RX_P_NO                 (BIT_3 | BIT_2 | BIT_1)
#define STATUS_TX_FULL                 BIT_0 
#define STATUS_RESET_VALUE      0x70                        // clear all flags (RX_DR, TX_DS, MAX_RT)

//---------------------------------------------OBSERVE_TX--------------------------------------------------------
#define OBSERVE_TX              0x08                        // register used to TODO
#define PLOS_CNT                (BIT_7 | BIT_6 | BIT_5 | BIT_4)   
#define ARC_CNT                 (BIT_3 | BIT_2 | BIT_1 | BIT_0)    
#define OBSERVE_TX_RESET_VALUE  0x00                        //0000 0000
//---------------------------------------------CD--------------------------------------------------------
#define CD                      BIT_0                       // register used to TODO
#define CD_BIT_MASK             (~BIT_0) //bits 7-1
#define CD_RESET_VALUE          0x00                        //0000 0000
//---------------------------------------------RX_ADDR_P0--------------------------------------------------------
#define RX_ADDR_P0              0x0A                        // register used to TODO
#define RX_ADDR_P0_RESET_VALUE  {0xE7, 0xE7, 0xE7, 0xE7, 0xE7}
#define RX_ADDR_P1              0x0B                        // register used to TODO
#define RX_ADDR_P1_RESET_VALUE  {0xC2,0xC2,0xC2,0xC2,0xC2}
#define RX_ADDR_P2              0x0C                        // register used to TODO
#define RX_ADDR_P2_RESET_VALUE  0xC3
#define RX_ADDR_P3              0x0D                        // register used to TODO
#define RX_ADDR_P3_RESET_VALUE  0xC4
#define RX_ADDR_P4              0x0E                        // register used to TODO
#define RX_ADDR_P4_RESET_VALUE  0xC5
#define RX_ADDR_P5              0x0F                        // register used to TODO
#define RX_ADDR_P5_RESET_VALUE  0xC6

//---------------------------------------------TX_ADDR--------------------------------------------------------
#define TX_ADDR 0x10
#define TX_ADDR_RESET_VALUE     {0xE7,0xE7,0xE7,0xE7,0xE7}

//---------------------------------------------RX_PW_Px--------------------------------------------------------
#define RX_PW_P0                0x11                        // register used to TODO
#define RX_PW_P0_RESET_VALUE    0x00                        //0000 0000            
#define RX_PW_P0_BIT_MASK       (BIT_6 | BIT_7) 
#define RX_PW_P1                0x12
#define RX_PW_P1_RESET_VALUE    0x00                        //0000 0000
#define RX_PW_P1_BIT_MASK       (BIT_6 | BIT_7) 
#define RX_PW_P2                0x13
#define RX_PW_P2_RESET_VALUE    0x00                        //0000 0000
#define RX_PW_P2_BIT_MASK       (BIT_6 | BIT_7) 
#define RX_PW_P3                0x14
#define RX_PW_P3_RESET_VALUE    0x00                        //0000 0000
#define RX_PW_P3_BIT_MASK       (BIT_6 | BIT_7) 
#define RX_PW_P4                0x15
#define RX_PW_P4_RESET_VALUE    0x00                        //0000 0000
#define RX_PW_P4_BIT_MASK       (BIT_6 | BIT_7) 
#define RX_PW_P5                0x16
#define RX_PW_P5_RESET_VALUE    0x00                        //0000 0000
#define RX_PW_P5_BIT_MASK       (BIT_6 | BIT_7) 

//---------------------------------------------FIFO_STATUS--------------------------------------------------------
#define FIFO_STATUS             0x17                        // register used to TODO
#define TX_REUSE                BIT_6
#define TX_FULL                 BIT_5
#define TX_EMPTY                BIT_4
#define FIFO_STATUS_BIT_MASK    (BIT_7 | BIT_3 | BIT_2)     // bits 7,3-2
#define RX_FULL                 BIT_1
#define RX_EMPTY                 BIT_0
#define FIFO_STATUS_RESET_VALUE 0x11                        //0001 0001
//---------------------------------------------DYNPDc--------------------------------------------------------
#define DYNPDc                  0x1C                        // register used to TODO
#define DYNPDc_BIT_MASK         (BIT_6 | BIT_7)     
#define DPL_P5                  BIT_5
#define DPL_P4                  BIT_4             
#define DPL_P3                  BIT_3
#define DPL_P2                  BIT_2
#define DPL_P1                  BIT_1
#define DPL_P0                  BIT_0
#define DYNPDc_RESET_VALUE      0x00                        //0000 0000

//---------------------------------------------FEATUREc--------------------------------------------------------
#define FEATUREc                0x1D                        // register used to TODO
#define FEATUREc_BIT_MASK       (BIT_7 | BIT_6 | BIT_5 | BIT_4 | BIT_3) //bits 7-3
#define EN_DPL                  BIT_2
#define EN_ACK_PAYd             BIT_1
#define EN_DYN_ACK              BIT_0
#define FEATUREc_RESET_VALUE    0x00                        //0000 0000

//Datasheet chapter SPI commands 8.x

#define SPI_R_REGISTER           0x00                       //bit 7-5, AAAAA = register adress (000A AAAA)  used for reading a register
#define SPI_W_REGISTER           0x20                       //bit 7-5, AAAAA = register adress (001A AAAA)  used for writing a register
#define SPI_R_RX_PAYLOAD         0x61                       //0110 0001 
#define SPI_W_TX_PAYLOAD         0xA0                       //1010 0000 
#define SPI_FLUSH_TX             0xE1                       //1110 0001
#define SPI_FLUSH_RX             0xE2                       //1110 0010
#define SPI_REUSE_TX_PL          0xE3                       //1110 0011
#define SPI_ACTIVATE             0x50                       //0101 0000
#define SPI_R_RX_PL_WIDa         0x60                       //0110 0000
#define SPI_W_ACK_PAYLOADa       0xA8                       //1010 1PPP
#define SPI_W_TX_PAYLOAD_NOACKa  0x58                       //1011 000
#define SPI_NOP                  0xFF                       //1111 1111
#define SPI_REGISTER_BITMASK     0x1F                       //0001 1111 ^ 000AAAAA the AAAAA from above
#define DUMMY_BYTE               0xFF                       //byte means nothing but is used for reading adresses


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



void nRF24L01::beginTransaction(){
    //spi speed for nRF24L01 rated to max 10mhz arduino is not(8mhz)
    // 8mhz
    //most significant bit first
    // spi mode 0 
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
}

void nRF24L01::readRegister(uint8_t adress, uint8_t* buffer , uint8_t length) {
    beginTransaction();
    CSN_Low();
    SPI.transfer(SPI_R_REGISTER | (adress & SPI_REGISTER_BITMASK)); // set the adress with the read command and make shure the adress is within the bitmask
    for (int i = 0; i < length; i++) {                              // store the transfers in the buffer array
        buffer[i] = SPI.transfer(DUMMY_BYTE);                       // transfer dummy bytes to receive data without sending a command
    }
    CSN_High();
}

void nRF24L01::readRegister(uint8_t adress, uint8_t& buffer) {
    beginTransaction();
    CSN_Low();
    SPI.transfer(SPI_R_REGISTER | (adress & SPI_REGISTER_BITMASK)); // set the adress with the read command and make shure the adress is within the bitmask
    buffer = SPI.transfer(DUMMY_BYTE);                              // transfer dummy bytes to receive data without sending a command 
    CSN_High();
}


void nRF24L01::writeRegister(uint8_t adress, uint8_t value){
    beginTransaction();
    CSN_Low();
    SPI.transfer(SPI_W_REGISTER | (adress & SPI_REGISTER_BITMASK)); // set the adress with the write command and make shure the adress is within the bitmask
    SPI.transfer(value);                                            // send the value to the adress
    CSN_High();
}

void nRF24L01::writeRegister(uint8_t adress, uint8_t* values , uint8_t length){
    beginTransaction();
    CSN_Low();
    SPI.transfer(SPI_W_REGISTER | (adress & SPI_REGISTER_BITMASK));// set the adress with the write command and make shure the adress is within the bitmask
    for (int i = 0; i < length; i++) {                              
        SPI.transfer(values[i]);                                   // send the values from the array to the adress
    }   
    CSN_High();
}

void nRF24L01::resetRegisters() {
    writeRegister(CONFIG, CONFIG_RESET_VALUE);
    writeRegister(EN_RXADDR, EN_RXADDR_RESET_VALUE);
    writeRegister(EN_AA, EN_AA_RESET_VALUE);
    writeRegister(SETUP_RETR, SETUP_RETR_RESET_VALUE);
    writeRegister(RF_CH, RF_CH_RESET_VALUE);
    writeRegister(RF_SETUP, RF_SETUP_RESET_VALUE);
    uint8_t rx_addr_p0_reset_value[5] = RX_ADDR_P0_RESET_VALUE;
    writeRegister(RX_ADDR_P0, rx_addr_p0_reset_value, 5);
    writeRegister(STATUS, STATUS_RESET_VALUE);
}

void nRF24L01::hardReset() {
    resetRegisters();
    writeRegister(SETUP_AW, SETUP_AW_RESET_VALUE);
    uint8_t rx_addr_p1_reset_value[5] = RX_ADDR_P1_RESET_VALUE;
    writeRegister(RX_ADDR_P1, rx_addr_p1_reset_value,5);
    writeRegister(RX_ADDR_P2, RX_ADDR_P2_RESET_VALUE);
    writeRegister(RX_ADDR_P3, RX_ADDR_P3_RESET_VALUE);
    writeRegister(RX_ADDR_P4, RX_ADDR_P4_RESET_VALUE);
    writeRegister(RX_ADDR_P5, RX_ADDR_P5_RESET_VALUE);
    uint8_t tx_addr_reset_value[5] = TX_ADDR_RESET_VALUE;
    writeRegister(TX_ADDR, tx_addr_reset_value,5);
    writeRegister(RX_PW_P0, RX_PW_P0_RESET_VALUE);
    writeRegister(RX_PW_P1, RX_PW_P1_RESET_VALUE);
    writeRegister(RX_PW_P2, RX_PW_P2_RESET_VALUE);
    writeRegister(RX_PW_P3, RX_PW_P3_RESET_VALUE);
    writeRegister(RX_PW_P4, RX_PW_P4_RESET_VALUE);
    writeRegister(RX_PW_P5, RX_PW_P5_RESET_VALUE);
    writeRegister(FIFO_STATUS, FIFO_STATUS_RESET_VALUE);
    writeRegister(DYNPDc, DYNPDc_RESET_VALUE);
    writeRegister(FEATUREc, FEATUREc_RESET_VALUE);
}


void nRF24L01::flushTX() {
    CSN_Low();
    SPI.transfer(SPI_FLUSH_TX);
    CSN_High();
}

void nRF24L01::flushRX() {
    CSN_Low();
    SPI.transfer(SPI_FLUSH_RX);
    CSN_High();
}



nRF24L01::nRF24L01(){
    //TODO
}

nRF24L01::nRF24L01(uint8_t CE_PIN,uint8_t CSN_PIN) : CE_PIN(CE_PIN), CSN_PIN(CSN_PIN){
    //TODO
}

nRF24L01::~nRF24L01(){

}



void nRF24L01::begin(){
    beginTransaction();
    pinMode(CSN_PIN, OUTPUT);
    pinMode(CE_PIN, OUTPUT);

    digitalWrite(CSN_PIN, HIGH); 
    digitalWrite(CE_PIN, LOW); 

    SPI.begin();
    resetRegisters();

    uint8_t config;
    readRegister(CONFIG,config);
    writeRegister(CONFIG,config | PWR_UP);

    delay(3); // minimal 1.5ms
}


void nRF24L01::test(){ 
    writeRegister(RF_CH, 42);           
    uint8_t val; 
    readRegister(RF_CH,val);  

    if (val == 42) {
        Serial.println("PASS: SPI communication works");
    } else {
        Serial.print("FAIL: Expected 42, got  ");
        Serial.println(val);
    }

    writeRegister(STATUS, STATUS_RESET_VALUE); // Clear IRQs
    uint8_t status; 
    readRegister(STATUS,status);

    if ((status & 0x70) == 0) {
        Serial.println("PASS: IRQs cleared");
    } else {
        Serial.print("FAIL: STATUS still has flags: ");
        Serial.println(status, BIN);
    }

}