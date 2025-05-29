## SPI Communication




## Transition Table
### chapter 6.1 datasheet

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

### Register map
### chapter 8 datasheet

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





