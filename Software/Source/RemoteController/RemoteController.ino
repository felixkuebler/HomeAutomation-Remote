
#include <IRremote.h>
#include "IRConfig.hpp"

#include "SPI.h"
#include "MCP23S17.h"
#include "BOKeypad.hpp"

#include "epd1in54_V2.h"
#include "epdpaint.h"

#define pinIrLedOut 3

#define pinExpanderCs 15
#define pinExpanderIrq 2

#define pinDisplayCs 14

// IR object for infrared transmission and reception
IRsend irSender(pinIrLedOut);

// GPIO expander object for button input
MCP23S17 gpioExpander(pinExpanderCs);

// Display object for e-ink dipslay
Epd display;

// stores the currently active device selector of the remote
volatile uint8_t currentDeviceSelectBtnId = BOKeybad::BtnId::TV;

// stores the last pushed button
// BtnId::NONE if already handled or on startup
volatile BOKeybad::BtnId activeBtnId = BOKeybad::BtnId::NONE;

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
  gpioExpander.begin();

  // use all gpios on port a for output
  gpioExpander.pinMode8(MCP23S17_PORTA, 0x00);
  // use all gpios on port b as inputs
  gpioExpander.pinMode8(MCP23S17_PORTB, 0x3F);

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

  return gpioExpander.isConnected();
}

bool setupDisplay() {
  // init and clear the display
  // this will refresh the hardware
  display.LDirInit();
  display.Clear();

  // display the default device selector
  displayText(BOKeybad::getBtnName(currentDeviceSelectBtnId));

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

  // check if a new button input was pressed
  if (activeBtnId != BOKeybad::BtnId::NONE) {

    Serial.print("Active Btn: ");  
    Serial.println(BOKeybad::getBtnName(activeBtnId));  

    // check if pressed button is device selector button
    if (BOKeybad::isDeviceSelectBtn(activeBtnId)) {
      // only change display text if different from current state
      if (activeBtnId != currentDeviceSelectBtnId){
        displayText(BOKeybad::getBtnName(activeBtnId));
      }
      // update current state
      currentDeviceSelectBtnId = activeBtnId;

      // different actions for the device selct buttons
      switch (currentDeviceSelectBtnId) {
        case BOKeybad::BtnId::TV : {
          // turn TV on
          irSender.sendSAMSUNG(ITConfig::IRCommands::tvOn, 32); 
          // turn amplifier on
          irSender.sendRaw_P(ITConfig::IRCommands::ampOn, ITConfig::IRCommands::ampOnLength, NEC_KHZ);
          break;
        }
      }
    }
    // check ifpressed button is a number button
    else if (BOKeybad::isNumBtn(activeBtnId)){
      // different actions for each number
      switch (currentDeviceSelectBtnId) {
        case BOKeybad::BtnId::NUM0 : {
          //send ir
          break;
        }
        case BOKeybad::BtnId::NUM1 : {
          //send ir
          break;
        }
      }
    }
    // check if special function button off
    else if (activeBtnId == BOKeybad::BtnId::OFF) {
      // turn TV off
      irSender.sendSAMSUNG(ITConfig::IRCommands::tvOff, 32); 
      // turn amplifier off
      irSender.sendRaw_P(ITConfig::IRCommands::ampOff, ITConfig::IRCommands::ampOffLength, NEC_KHZ);
    }
    // clear active button indicator to be set with the next btn press
    activeBtnId = BOKeybad::BtnId::NONE;
  }
}

void gpioExpanderIsr() {
  // set irq flag to true for handling in main thread
  irqOccured = true;
}

void displayText(char* text) {
  // canvas for paining the text
  uint8_t img[1024];
  Paint paint(img, 0, 0);

  const uint8_t heightDisplay = 200;
  const uint8_t widthDisplay = 200;

  const uint8_t heightCanvas = 24;
  const uint8_t widthCanvas = widthDisplay;

  const uint8_t posCanvasX = (widthCanvas - (Font24.Width*strlen(text)))/2;
  const uint8_t posCanvasY = 0;

  const uint8_t posDisplayX = 0;
  const uint8_t posDisplayY = (heightDisplay - Font24.Height)/2;

  // set the dimensions of the canvas
  paint.SetWidth(widthCanvas);
  paint.SetHeight(heightCanvas);

  // draw text
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(posCanvasX, posCanvasY, text, &Font24, COLORED);

  // update display
  display.SetFrameMemoryPartial(paint.GetImage(), posDisplayX, posDisplayY, paint.GetWidth(), paint.GetHeight());
  display.DisplayPartFrame();
}