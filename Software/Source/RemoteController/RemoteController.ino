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

// interrupt handling request
// is set in IRS and executed in thread context
volatile bool irqOccured = false;


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
  // set compare default value to zeros
  gpioExpander.writeReg(MCP23S17_DEFVAL_B, 0x00);
  // set compare against default values
  gpioExpander.writeReg(MCP23S17_INTCON_B, 0xFF);
  // use interrupt pin as driven output
  gpioExpander.disableControlRegister(MCP23S17_IOCR_ODR);
  // set polarity of interrupt pin to active high
  gpioExpander.enableControlRegister(MCP23S17_IOCR_INTPOL);
  // enable interrupts for bank b
  gpioExpander.writeReg(MCP23S17_GPINTEN_B, 0x3F);
  // clear interrupt pending
  gpioExpander.readReg(MCP23S17_INTCAP_B);

  // configure interrupt pin
  pinMode(pinExpanderIrq, INPUT);
  // attach the interrupt handling of the mcu
  attachInterrupt(digitalPinToInterrupt(pinExpanderIrq), gpioExpanderIsr, RISING);

  // set all output gpios to high
  gpioExpander.write8(MCP23S17_PORTA, 0x3F);
}

bool setupDisplay() {
  display.LDirInit();
  display.Clear();

  return true; 
}

void loop() {
  // handle input button interrupt
  if (irqOccured) {
    // reset irq flag
    irqOccured = false;
    // disable interrupt while accessing the gpios of the expander
    detachInterrupt(digitalPinToInterrupt(pinExpanderIrq));

    for (uint8_t outPin = 0; outPin < BOKeybad::numGpioOut; outPin++) {
      // write single output gpio
      gpioExpander.write8(MCP23S17_PORTA, 0x01 << outPin);
      // read back all input gpios
      uint8_t inputMask = gpioExpander.read8(MCP23S17_PORTB);
      inputMask &= 0x3F;
      // check if any input is set
      if (inputMask) {
        // find which input is set
        uint8_t inputPinIdx = 0;
        while (inputPinIdx < BOKeybad::numGpioIn) {
          if (inputMask >> inputPinIdx & 0x01 == 0x01) {
            break;
          }
          inputPinIdx++;
        }
        // set active btn id
        activeBtnId = BOKeybad::gpioToBtnId(outPin, inputPinIdx);
      }
    } 
    // turn on all out gpios
    gpioExpander.write8(MCP23S17_PORTA, 0x3F);

    // clear expander interrupt
    while(digitalRead(pinExpanderIrq)) {
      gpioExpander.read8(MCP23S17_PORTB);
    }

    // enable interrupt again
    attachInterrupt(digitalPinToInterrupt(pinExpanderIrq), gpioExpanderIsr, RISING);
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

void gpioExpanderIsr() {
  // set irq flag to true for handling in main thread
  irqOccured = true;
}

void displayText(char* text) {
  paint.SetWidth(200);
  paint.SetHeight(24);

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(30, 4, text, &Font16, COLORED);

  display.SetFrameMemoryPartial(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  display.DisplayPartFrame();
}