#include <Adafruit_SSD1306.h>
#include "nRF24L01.h"

nRF24L01 radio(2, 3); // CE = 2, CSN = 3

const uint8_t address[5] = {'N', 'O', 'D', 'E', '1'};
inline constexpr uint8_t displayAdress = 0x3C;

Adafruit_SSD1306 display(128, 64, &Wire); // initialise i2c


void setup() {
    Serial.begin(9600);
    radio.begin();

    if (!display.begin(SSD1306_SWITCHCAPVCC, displayAdress)){
        Serial.println("SSD1306 allocation failed");
    }
    display.setTextSize(1);
    display.setTextColor(WHITE);

    radio.setPayloadSize(nRF24L01_PayloadSize_32_BYTES, nRF24L01_Pipe_P0);
    radio.setAdressWidth(nRF24L01_AdressWidth_5_Bytes);
    radio.setAirDataRate(nRF24L01_AirDataRate_1_Mbps);
    radio.setFrequency(76);
    radio.setPowerMode(nRF24L01_PowerMode_0_dBm);
    //radio.setDynamicPayload(true);
    radio.setRX_Address(nRF24L01_Pipe_P0, address, 5);
    radio.setRX_Pipe(nRF24L01_Pipe_P0, true);

    radio.setMode(nRF24L01_Mode_RECEIVE);
}

void loop() {
    if (radio.isDataAvaliable()) {
        String msg = radio.readData();
        Serial.print("Received: ");
        Serial.println(msg);
        display.clearDisplay(); // Clear display
        display.setCursor(0, 0); // Reset cursor position
        display.print(msg);
        display.display();
    }
}