# Chapter 6 datasheet
## 6.1 Transition Table

| From       | To         | Condition 1         | Condition 2         | Condition 3          |
| ---------- | ---------- | ------------------- | ------------------- | -------------------- |
| Undefined  | Power Down | `VDD >= 1.9V`       |                     |                      |
| Power Down | Standby-I  | `PWR_UP = 1`        | Wait 1.5 ms         |                      |
| Power Down | Undefined  | `VDD < 1.9V`        |                     |                      |
| Standby-I  | Power Down | `PWR_UP = 0`        |                     |                      |
| Standby-I  | Standby-II | `PRIM_RX = 0`       | `TX FIFO not empty` | `CE = 1`             |
| Standby-I  | RX Setting | `PRIM_RX = 1`       | `CE = 1`            |                      |
| RX Setting | RX Mode    | Wait 130 µs         |                     |                      |
| RX Mode    | Standby-I  | `CE = 0`            |                     |                      |
| Standby-I  | TX Setting | `PRIM_RX = 0`       | `TX FIFO not empty` | `CE = 1 for > 10 µs` |
| TX Setting | TX Mode    | Wait 130 µs         |                     |                      |
| TX Mode    | Standby-I  | TX finished         | `CE = 0`            |                      |
| Standby-II | TX Mode    | `TX FIFO not empty` | `CE = 1`            |                      |
| TX Mode    | Standby-II | `TX FIFO not empty` | `CE = 1`            |                      |
| TX Mode    | Standby-I  | `TX FIFO empty`     |                     |                      |

## 6.1.2 Power Down Mode

minimal current consumption  
register values available from the SPI are maintained  
Power down mode is entered by setting the PWR_UP bit in the CONFIG register low [see](#616-operational-modes-configuration)  


## 6.1.3 Standby Modes
### standby-I
By settting the `PWR_UP` bit in the `CONFIG` register to 1, the device enters `standby-I mode` [see](#616-operational-modes-configuration)  
`Standby-I mode` is used to minimize average current consumption while maintaining short start up times  
This is the mode the nRF24L01 returns to from `TX or RX mode` when `CE` is set low

### standby-II
extra clock buffers are active compared to `standby-I mode` and much more current is used compared to `standby-I mode`  
`Standby-II` occurs when `CE` is held high on a PTX device with `empty TX FIFO` [see](#616-operational-modes-configuration)  
If a new packet is uploaded to the `TX FIFO`, the `PLL` starts and the packet is transmitted  


## 6.1.4 RX mode
To enter this mode, the nRF24L01 must have the `PWR_UP` bit set high, `PRIM_RX` bit set high and the `CE` pin set high. [see](#616-operational-modes-configuration)     
If a valid packet is found (by a matching address and a valid CRC) the payload of the packet
is presented in a vacant slot in the `RX FIFO`.   
If the `RX FIFO` is full, the received packet is discarded.  
The nRF24L01 remains in `RX mode` until the `MCU` configures it to `standby-I mode` or `power down mode`    
The Carrier Detect (`CD`) is set high when an RF signal is detected in `RX mode`, otherwise `CD` is low  
The RF signal must be present for at least 128µs before the CD is set high  


## 6.1.5 TX mode
The TX mode is an active mode where the nRF24L01 transmits a packet. To enter this mode, the
nRF24L01 must have the PWR_UP bit set high, PRIM_RX bit set low, a payload in the TX FIFO and, a high
pulse on the CE for more than 10µs. [see](#616-operational-modes-configuration)  

The nRF24L01 stays in TX mode until it finishes transmitting a current packet :
If CE = 0 nRF24L01 returns to standby-I mode. If CE = 1, the next action is determined by the status of the TX FIFO. If the TX FIFO is
not empty the nRF24L01 remains in TX mode, transmitting the next packet. If the TX FIFO is empty the
nRF24L01 goes into standby-II mode [see](#61-transition-table)

It is important to never keep the nRF24L01 in TX mode for more than 4ms at a time
If the auto retransmit is enabled, the nRF24L01 is never in TX mode long enough to disobey this rule


## 6.1.6 Operational modes configuration
| Mode        | PWR_UP | PRIM_RX | CE                      | FIFO state                                         |
|-------------|--------|---------|-------------------------|----------------------------------------------------|
| RX mode     |   1    |   1     | 1                       |                                                    |
| TX mode     |   1    |   0     | 1                       | Data in TX FIFO. Will empty all levels in TX FIFOa.|
| TX mode     |   1    |   0     | minimum 10μs high pulse | Data in TX FIFO. Will empty one level in TX FIFOb  |
| Standby-I   |   1    |         | 0                       | No ongoing packet transmission                     |
| Standby-II  |   1    |   0     | 1                       | TX FIFO empty                                      |
| Power Down  |   0    |         |                         |                                                    |



## 6.1.7 Timing Information
|                                                   |               | 
|---------------------------------------------------|---------------|
|Power Down -> Standby mode                         |1.5ms          | 
|Power Down -> Standby mode(With external clock)    |150µs  max     | 
|Standby modes -> TX/RX mode                        |130µs max      | 
|Minimum CE high                                    |10µs min       | 
|Delay from CE pos. edge to CSN low                 |4µs min        | 


## 6.2 Air data rate 
The air data rate can be 1Mbps or 2Mbps  
The air data rate is set by the `RF_DR` bit in the `RF_SETUP` register.  
The 1Mbps data rate gives 3dB better receiver sensitivity compared to 2Mbps. High air data rate means lower average current consumption and reduced probability of
on-air collisions.  
For compatibility with nRF2401A, nRF24E1, nRF2402 and nRF24E2 the air data rate must be set to 1Mbps.  

## 6.3 RF chanel frequency
The channel occupies a bandwidth of 1MHz at 1Mbps and 2MHz at 2Mbps  
The resolution of the RF channel frequency setting is 1MHz.  
The RF channel frequency is set by the `RF_CH` register according to the following formula within 2.400GHz to 2.525GHz:  

    F0= 2400 + RF_CH [MHz]  

## 6.4 Power control (PA Control)
The PA control is set by the `RF_PWR` bits in the `RF_SETUP` register  
In `TX mode` PA control has four modes  


|SPI RF-SETUP (RF_PWR)  |RF output power    |DC current consumption |
|-----------------------|-------------------|-----------------------|
|11                     |  0dBm             |   11.3mA              |
|10                     | -6dBm             |   9.0mA               |
|01                     | -12dBm            |    7.5mA              |
|00                     | -18dBm            |    7.0mA              |

## 6.5 Low Noise Amplifier (LNA) gain
The gain in the (LNA) in the nRF24L01 receiver is controlled by the LNA gain setting.  
The LNA gain makes it possible to reduce the current consumption in RX mode with 0.8mA at the cost of
1.5dB reduction in receiver sensitivity making it harder to pick up weaker signals.  
The LNA gain has two steps and is set by the `LNA_HCURR` bit in the `RF_SETUP` register.  


## 6.6 RX/TX control
The RX/TX control is set by `PRIM_RX` bit in the `CONFIG` register and sets the nRF24L01 in transmit/receive.



# chapter 8
## SPI Communication
| Command Name         | Command Word (binary) | # Data Bytes | Operation / Description                                                                                     |
|----------------------|------------------------|--------------|------------------------------------------------------------------------------------------------------------|
| R_REGISTER           | 000A AAAA             | 1 to 5       | Read command and status registers. AAAAA = 5-bit Register Map Address.                                     |
| W_REGISTER           | 001A AAAA             | 1 to 5       | Write command and status registers. AAAAA = 5-bit Register Map Address. Only in power down/standby modes.  |
| R_RX_PAYLOAD         | 0110 0001             | 1 to 32      | Read RX-payload: 1–32 bytes. Payload is deleted from FIFO after read. Used in RX mode.                     |
| W_TX_PAYLOAD         | 1010 0000             | 1 to 32      | Write TX-payload: 1–32 bytes. Used in TX mode.                                                             |
| FLUSH_TX             | 1110 0001             | 0            | Flush TX FIFO, used in TX mode.                                                                            |
| FLUSH_RX             | 1110 0010             | 0            | Flush RX FIFO, used in RX mode. Not during acknowledge transmission.                                       |
| REUSE_TX_PL          | 1110 0011             | 0            | Reuse last transmitted payload. Retransmits as long as CE is high.                                         |
| ACTIVATE             | 0101 0000             | 1            | Write command + 0x73 activates special features (R_RX_PL_WID, W_ACK_PAYLOAD, W_TX_PAYLOAD_NOACK).          |
| R_RX_PL_WID          | 0110 0000             | 0            | Read RX-payload width for the top R_RX_PAYLOAD in the RX FIFO.                                             |
| W_ACK_PAYLOAD        | 1010 1PPP             | 1 to 32      | Used in RX mode. Write payload to be sent with ACK on PIPE PPP (000–101).                                  |
| W_TX_PAYLOAD_NOACK   | 1011 000              | 1 to 32      | Used in TX mode. Disables AUTOACK for this specific packet.                                                |
| NOP                  | 1111 1111             | 0            | No Operation. Can be used to read the STATUS register.                                                     |


# chapter 9
## 9.1 Register map

| Address | Name             | Purpose                                                                |
| ------: | ---------------- | ---------------------------------------------------------------------------------------------- |
|  `0x00` | **CONFIG**       | Turns the chip **on/off**, sets **TX or RX mode**, enables/disables **interrupts** and **CRC** |
|  `0x01` | **EN\_AA**       | Turns **Auto Acknowledge** ON/OFF for each data pipe (pipe 0–5)                                |
|  `0x02` | **EN\_RXADDR**   | Turns ON which data pipes (0–5) can **receive data**                                           |
|  `0x03` | **SETUP\_AW**    | Sets the **address width** (how many bytes: 3, 4, or 5)                                        |
|  `0x04` | **SETUP\_RETR**  | Controls **retries**: how many times and how long to wait between retransmits                  |
|  `0x05` | **RF\_CH**       | Sets the **radio frequency channel** (like WiFi channels)                                      |
|  `0x06` | **RF\_SETUP**    | Sets **data speed** (250kbps, 1Mbps, 2Mbps) and **power level** (low to high)                  |
|  `0x07` | **STATUS**       | Shows **interrupt flags** – tells if you got data, sent data, or failed                        |
|  `0x08` | **OBSERVE\_TX**  | Shows how many packets were **lost or retransmitted**                                          |
|  `0x09` | **RPD**          | Tells if the **radio saw a signal** (used to detect if another device is nearby)               |
|  `0x0A` | **RX\_ADDR\_P0** | Sets the **receive address for pipe 0** (5 bytes usually)                                      |
|  `0x0B` | **RX\_ADDR\_P1** | Sets the **receive address for pipe 1** (also 5 bytes)                                         |
|  `0x0C` | **RX\_ADDR\_P2** | Pipe 2 address – uses last byte added to pipe 1’s address                                      |
|  `0x0D` | **RX\_ADDR\_P3** | Pipe 3 address – same trick as pipe 2                                                          |
|  `0x0E` | **RX\_ADDR\_P4** | Pipe 4 address – same trick                                                                    |
|  `0x0F` | **RX\_ADDR\_P5** | Pipe 5 address – same trick                                                                    |
|  `0x10` | **TX\_ADDR**     | Address to **send data to** (5 bytes, usually same as RX\_ADDR\_P0 for auto-ack to work)       |
|  `0x11` | **RX\_PW\_P0**   | How many bytes of data to **receive on pipe 0** (1–32 max)                                     |
|  `0x12` | **RX\_PW\_P1**   | Same for pipe 1                                                                                |
|  `0x13` | **RX\_PW\_P2**   | Same for pipe 2                                                                                |
|  `0x14` | **RX\_PW\_P3**   | Same for pipe 3                                                                                |
|  `0x15` | **RX\_PW\_P4**   | Same for pipe 4                                                                                |
|  `0x16` | **RX\_PW\_P5**   | Same for pipe 5                                                                                |
|  `0x17` | **FIFO\_STATUS** | Shows if TX and RX queues are **full or empty**                                                |
|  `0x18` | **DYNPD**        | Turns ON **dynamic payload** (different size messages) for pipes                               |
|  `0x19` | **FEATURE**      | Enables **special features** like dynamic payloads and sending without ACK                     |



# chapter 13 
## terms

| Term   | Description                                 |
|--------|---------------------------------------------|
| ACK    | Acknowledgement                             |
| ART    | Auto Re-Transmit                            |
| CE     | Chip Enable                                 |
| CLK    | Clock                                       |
| CRC    | Cyclic Redundancy Check                     |
| CSN    | Chip Select NOT                             |
| ESB    | Enhanced ShockBurst™                        |
| GFSK   | Gaussian Frequency Shift Keying             |
| IRQ    | Interrupt Request                           |
| ISM    | Industrial-Scientific-Medical               |
| LNA    | Low Noise Amplifier                         |
| LSB    | Least Significant Bit                       |
| LSByte | Least Significant Byte                      |
| Mbps   | Megabit per second                          |
| MCU    | Microcontroller Unit                        |
| MISO   | Master In Slave Out                         |
| MOSI   | Master Out Slave In                         |
| MSB    | Most Significant Bit                        |
| MSByte | Most Significant Byte                       |
| PCB    | Printed Circuit Board                       |
| PID    | Packet Identity Bits                        |
| PLD    | Payload                                     |
| PRX    | Primary RX                                  |
| PTX    | Primary TX                                  |
| PWR_DWN| Power Down                                  |
| PWR_UP | Power Up                                    |
| RoHS   | Restriction of use of Certain Hazardous Substances |
| RX     | Receive                                     |
| RX_DR  | Receive Data Ready                          |
| SPI    | Serial Peripheral Interface                 |
| TX     | Transmit                                    |
| TX_DS  | Transmit Data Sent                          |