#include "nRF24L01.h"

nRF24L01 radio(9, 10); // CE, CSN
/*
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setMode(nRF24L01_Mode_TRANSMIT);
}

void loop() {
  String payload = "test";
  radio.send(payload);
}
*/

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setMode(nRF24L01_Mode_RECEIVE);
}

void loop() {
  if (radio.isDataAvaliable()){
    String data = radio.readData();
    Serial.println(data);
  }
}
