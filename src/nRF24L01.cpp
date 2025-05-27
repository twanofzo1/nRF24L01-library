#include "nRF24L01.h"

#define BIT_0 0x01 //0000 0001 
#define BIT_1 0x02 //0000 0010 
#define BIT_2 0x04 //0000 0100 
#define BIT_3 0x08 //0000 1000 
#define BIT_4 0x10 //0001 0000 
#define BIT_5 0x20 //0010 0000 
#define BIT_6 0x40 //0100 0000 
#define BIT_7 0x80 //1000 0000 
//9.1 register map table

//---------------------------------------------CONFIG--------------------------------------------------------
#define CONFIG                  0x00  
#define MASK_RX_DR              BIT_6     
#define MASK_TX_DS              BIT_5        
#define MASK_MAX_RT             BIT_4        
#define EN_CRC                  BIT_3    
#define CRCO                    BIT_2    
#define PWR_UP                  BIT_1    
#define PRIM_RX                 BIT_0    
#define CONFIG_BIT_MASK         BIT_7 

//---------------------------------------------EN_AA--------------------------------------------------------
#define EN_AA                   0x01   
#define ENAA_P5                 BIT_5 
#define ENAA_P4                 BIT_4 
#define ENAA_P3                 BIT_3 
#define ENAA_P2                 BIT_2 
#define ENAA_P1                 BIT_1 
#define ENAA_P0                 BIT_0 
#define EN_AA_BIT_MASK          (BIT_6 | BIT_7) 

//---------------------------------------------EN_RXADDR--------------------------------------------------------
#define EN_RXADDR               0x02  
#define ERX_P5                  BIT_5 
#define ERX_P4                  BIT_4 
#define ERX_P3                  BIT_3 
#define ERX_P2                  BIT_2 
#define ERX_P1                  BIT_1 
#define ERX_P0                  BIT_0 
#define EN_RXADDR_BIT_MASK      (BIT_6 | BIT_7)  

//---------------------------------------------SETUP_AW--------------------------------------------------------
#define SETUP_AW                0x03
#define AW                      (BIT_1 | BIT_0)  
#define AW_ILLEGAL              0x0 
#define AW_3_BYTES              0x1
#define AW_4_BYTES              0x2
#define AW_5_BYTES              0x3 
#define AW_BIT_MASK             (~(BIT_1 | BIT_0) ) // bits 7-2

//---------------------------------------------SETUP_RETR--------------------------------------------------------
#define SETUP_RETR              0x04
#define ARD                     (BIT_7 | BIT_6 | BIT_5 | BIT_4) // bits 7-4
#define ARC                     (BIT_3 | BIT_2 | BIT_1 | BIT_0) // bits 3-0

//---------------------------------------------RF_CH_ADDR--------------------------------------------------------
#define RF_CH                   0x05
#define RF_CH_BIT_MASK          BIT_7

//---------------------------------------------RF_SETUP--------------------------------------------------------
#define RF_SETUP                0x06
#define PLL_LOCK                BIT_4
#define RF_DR                   BIT_3
#define RF_PWR                  (BIT_1 | BIT_2) 
#define RF_PWR_18dBm            0x0
#define RF_PWR_12dBm            0x1
#define RF_PWR_0dBm             0x2
#define RF_PWR_6dBm             0x3
#define LNA_HCURR               BIT_0
#define RF_SETUP_BIT_MASK       (BIT_7 | BIT_6 | BIT_5 | BIT_4)

//---------------------------------------------STATUS--------------------------------------------------------
#define STATUS                  0x07
#define STATUS_BIT_MASK         BIT_7 
#define RX_DR                   BIT_6 
#define TX_DS                   BIT_5 
#define MAX_RT                  BIT_4 
#define RX_P_NO                 (BIT_3 | BIT_2 | BIT_1)
#define TX_FULL                 BIT_0 

//---------------------------------------------OBSERVE_TX--------------------------------------------------------
#define OBSERVE_TX              0x08
#define PLOS_CNT                (BIT_7 | BIT_6 | BIT_5 | BIT_4)   
#define ARC_CNT                 (BIT_3 | BIT_2 | BIT_1 | BIT_0)    

//---------------------------------------------CD--------------------------------------------------------
#define CD                      BIT_0
#define CD_BIT_MASK             (~BIT_0) //bits 7-1

//---------------------------------------------RX_ADDR_P0--------------------------------------------------------
#define RX_ADDR_P0              0x0A
#define RX_ADDR_P1              0x0B
#define RX_ADDR_P2              0x0C
#define RX_ADDR_P3              0x0D
#define RX_ADDR_P4              0x0E
#define RX_ADDR_P5              0x0F

//---------------------------------------------TX_ADDR--------------------------------------------------------
#define TX_ADDR 0x10

//---------------------------------------------RX_PW_Px--------------------------------------------------------
#define RX_PW_P0                0x11
#define RX_PW_P0_BIT_MASK       (BIT_6 | BIT_7) 
#define RX_PW_P1                0x12
#define RX_PW_P1_BIT_MASK       (BIT_6 | BIT_7) 
#define RX_PW_P2                0x13
#define RX_PW_P2_BIT_MASK       (BIT_6 | BIT_7) 
#define RX_PW_P3                0x14
#define RX_PW_P3_BIT_MASK       (BIT_6 | BIT_7) 
#define RX_PW_P4                0x15
#define RX_PW_P4_BIT_MASK       (BIT_6 | BIT_7) 
#define RX_PW_P5                0x16
#define RX_PW_P5_BIT_MASK       (BIT_6 | BIT_7) 

//---------------------------------------------FIFO_STATUS--------------------------------------------------------
#define FIFO_STATUS             0x17
#define TX_REUSE                BIT_6
#define TX_FULL                 BIT_5
#define TX_EMPTY                BIT_4
#define FIFO_STATUS_BIT_MASK    (BIT_7 | BIT_3 | BIT_2) // bits 7,3-2
#define RX_FULL                 BIT_1
#define RX_FULL                 BIT_0

//---------------------------------------------DYNPDc--------------------------------------------------------
#define DYNPDc                  0x1C
#define DYNPDc_BIT_MASK         (BIT_6 | BIT_7)     
#define DPL_P5                  BIT_5
#define DPL_P4                  BIT_4             
#define DPL_P3                  BIT_3
#define DPL_P2                  BIT_2
#define DPL_P1                  BIT_1
#define DPL_P0                  BIT_0

//---------------------------------------------FEATUREc--------------------------------------------------------
#define FEATUREc                0x1D
#define FEATUREc_BIT_MASK       (BIT_7 | BIT_6 | BIT_5 | BIT_4 | BIT_3) //bits 7-3
#define EN_DPL                  BIT_2
#define EN_ACK_PAYd             BIT_1
#define EN_DYN_ACK              BIT_0

// nRF24L01 SPI commands 8.x

#define SPI_R_REGISTER           0x00    //bit 7-5, AAAAA = register adress (000A AAAA)  
#define SPI_W_REGISTER           0x20    //bit 7-5, AAAAA = register adress (001A AAAA)  
#define SPI_R_RX_PAYLOAD         0x61    //0110 0001 
#define SPI_W_TX_PAYLOAD         0xA0    //1010 0000 
#define SPI_FLUSH_TX             0xE1    //1110 0001
#define SPI_FLUSH_RX             0xE2    //1110 0010
#define SPI_REUSE_TX_PL          0xE3    //1110 0011
#define SPI_ACTIVATE             0x50    //0101 0000
#define SPI_R_RX_PL_WIDa         0x60    //0110 0000
#define SPI_W_ACK_PAYLOADa       0xA8    //1010 1PPP
#define SPI_W_TX_PAYLOAD_NOACKa  0x58    //1011 000
#define SPI_NOP                  0xFF    //1111 1111

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

nRF24L01::nRF24L01(){

}

nRF24L01::nRF24L01(uint8_t CE_PIN, uint8_t CSN_PIN) : CE_PIN(CE_PIN), CSN_PIN(CSN_PIN){
    
}

