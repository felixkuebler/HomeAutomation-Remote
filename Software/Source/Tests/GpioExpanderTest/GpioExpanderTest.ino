//
//    FILE: MCP23S17_digitalWrite.ino
//  AUTHOR: Rob Tillaart
//    DATE: 2021-12-30
// PUPROSE: test MCP23017 library


#include "MCP23S17.h"
#include "SPI.h"

MCP23S17 MCP(15);   // SW SPI   address 0x00

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.print("MCP23S17_test version: ");
  Serial.println(MCP23S17_LIB_VERSION);
  delay(100);

  SPI.begin();
  bool b = MCP.begin();
  Serial.println(b ? "true" : "false");
  delay(100);

  MCP.pinMode16(0, 0x00);  // 0 = output , 1 = input
}


void loop() {
  Serial.println("TEST digitalWrite(0)");
  delay(100);
  for (int i = 0; i < 16; i++)
  {
    MCP.digitalWrite(0, i % 2);  // alternating HIGH/LOW
    Serial.print(i % 2);
    Serial.print(' ');
    delay(100);
  }
  Serial.println();
  Serial.println();
}

