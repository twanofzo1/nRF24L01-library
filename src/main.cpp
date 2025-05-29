#include <Arduino.h>
#include "nRF24L01.h"

nRF24L01 radio(7,8);

void setup() {
    Serial.begin(9600);
    radio.begin();
}

void loop() {
    radio.test();
    delay(1001);
}

