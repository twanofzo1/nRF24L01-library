#include "nRF24L01.h"

nRF24L01 radio(2, 3); // CE = 9, CSN = 10

const byte address[5] = {'N', 'O', 'D', 'E', '1'};

void setup() {
    Serial.begin(9600);
    radio.begin();

    radio.setPayloadSize(nRF24L01_PayloadSize_32_BYTES, nRF24L01_Pipe_P0);
    radio.setAdressWidth(nRF24L01_AdressWidth_5_Bytes);
    radio.setAirDataRate(nRF24L01_AirDataRate_1_Mbps);
    radio.setFrequency(76);
    radio.setPowerMode(nRF24L01_PowerMode_0_dBm);
    radio.setRetransmits(ARC_Retransmit_15_Times, ARD_Wait_uS_500);
    //radio.setDynamicPayload(true);
    radio.setTX_adress(address, 5);
    radio.setRX_Address(nRF24L01_Pipe_P0, address, 5);
    

    radio.setMode(nRF24L01_Mode_TRANSMIT);
}

void loop() {
    String msg = "hellohellohellohellohellohellooo";
    Serial.print("Sending: ");
    Serial.println(msg);

    nRF24L01_Status result = radio.send(msg);
    if (result == nRF24L01_Status_OK) {
        Serial.println("Send OK");
    } else {
        Serial.print("Send failed: ");
        Serial.println(result);
    }

    delay(1000);
}
