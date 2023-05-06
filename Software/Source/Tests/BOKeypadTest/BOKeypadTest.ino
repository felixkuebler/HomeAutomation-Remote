//
//    FILE: MCP23S17_digitalWrite.ino
//  AUTHOR: Rob Tillaart
//    DATE: 2021-12-30
// PUPROSE: test MCP23017 library


#include "MCP23S17.h"
#include "SPI.h"

MCP23S17 expander(15);   // SW SPI   address 0x00

void setup() {
  Serial.begin(9600);
  delay(100);

  SPI.begin();
  bool b = expander.begin();
  Serial.println(b ? "true" : "false");
  delay(100);

  Serial.print("Is connected: ");
  Serial.println(expander.isConnected());
  Serial.println();

  expander.pinMode8(0, 0x00);  // 0 = output , 1 = input
  expander.pinMode8(1, 0xFF);
}

void loop() {
  for (int outPin = 0; outPin < 6; outPin++) {
    expander.digitalWrite(outPin, 1);
    for (int inPin = 0; inPin < 6; inPin++) {
      bool assert = expander.digitalRead(inPin+8);
      if (assert) {
        Serial.print("Signal at [out=");
        Serial.print(outPin);
        Serial.print(", in=");
        Serial.print(inPin);
        Serial.println("] ");
      }
    }
    expander.digitalWrite(outPin, 0);
  }
  delay(100);
}