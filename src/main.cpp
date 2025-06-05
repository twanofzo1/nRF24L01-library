#include "nRF24L01.h"
nRF24L01 radio(9, 10); 


void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.setFrequency(0);
    radio.setMode(nRF24L01_Mode_TRANSMIT);
    radio.test();
}

void loop(){
    const char* data = "test";
    radio.send(data);
    delay(100);
}
/*

void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.setFrequency(0);
    radio.setMode(nRF24L01_Mode_RECEIVE);
}

void loop(){
    if (radio.isDataAvaliable()){
        uint8_t data = radio.readData();
        Serial.print(data);
        delay(1000);
    } 
    Serial.println("no data");
}*/