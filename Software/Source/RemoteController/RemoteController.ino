#include <stdio.h>

#include "SPI.h"
#include <IRremote.h>
#include "IRConfig.hpp"

#include "MCP23S17.h"

#include "epd1in54_V2.h"
#include "imagedata.h"
#include "epdpaint.h"

#include "BOKeypad.hpp"

// IR object for infrared transmission and reception
IRsend irSender(pinIrLedOut);

const uint8_t expanderCs = 15;
const uint8_t displayCs = 14;

MCP23S17 expander(expanderCs);
Epd display;

unsigned char image[1024];
Paint paint(image, 0, 0);

#define COLORED     0
#define UNCOLORED   1



void setup() {
  Serial.begin(9600);
  delay(100);

  irSender.begin(pinIrLedOut);

  SPI.begin();
  delay(100);

  setupGpioExpander();
  delay(100);

  setupDisplay();
  delay(100);
}

bool setupGpioExpander() {
  expander.begin();

  expander.pinMode(0, OUTPUT);
  expander.pinMode(1, OUTPUT);
  expander.pinMode(2, OUTPUT);
  expander.pinMode(3, OUTPUT);
  expander.pinMode(4, OUTPUT);
  expander.pinMode(5, OUTPUT);
  expander.pinMode(8, INPUT);
  expander.pinMode(9, INPUT);
  expander.pinMode(10, INPUT);
  expander.pinMode(11, INPUT);
  expander.pinMode(12, INPUT);
  expander.pinMode(13, INPUT);
  
  return expander.isConnected();
}

bool setupDisplay() {
  display.LDirInit();
  display.Clear();

  return true; 
}

void loop() {
  for (int outPin = 0; outPin < 6; outPin++) {
    expander.digitalWrite(outPin, 1);
    for (int inPin = 0; inPin < 6; inPin++) {
      bool assert = expander.digitalRead(inPin+8);
      if (assert) {
        BtnId id = gpioToBtnId(outPin, inPin);
        Serial.println(getBtnName(id));
        displayText(getBtnName(id));
      }
    }
    expander.digitalWrite(outPin, 0);
  }


  delay(100);
          // turn TV on
          irSender.sendSAMSUNG(ITConfig::IRCommands::tvOn, 32); 
          // turn amplifier on
          irSender.sendRaw_P(ITConfig::IRCommands::ampOn, ITConfig::IRCommands::ampOnLength, NEC_KHZ);
          break;
      // turn TV off
      irSender.sendSAMSUNG(ITConfig::IRCommands::tvOff, 32); 
      // turn amplifier off
      irSender.sendRaw_P(ITConfig::IRCommands::ampOff, ITConfig::IRCommands::ampOffLength, NEC_KHZ);
}


void displayText(char* text) {
  paint.SetWidth(200);
  paint.SetHeight(24);

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(30, 4, text, &Font16, COLORED);

  display.SetFrameMemoryPartial(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  display.DisplayPartFrame();
}