#include <avr/sleep.h>

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

// 30 seconds timeout
#define sleepTimeout 30000

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

// timer variable to keep track of the last time a button has been pressed
// use this time to send cpu into sleep mode
volatile unsigned long upTime = millis();

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
          irSender.sendSAMSUNG(IRConfig::IRCommands::Tv::tvOn, 32);
          delay(100);
          // turn amplifier on
          irSender.sendRaw_P(IRConfig::IRCommands::Amplifier::ampOn, IRConfig::IRCommands::Amplifier::ampOnLength, NEC_KHZ);
          break;
        }
      }
    }
    // check if pressed button is a number button
    else if (BOKeybad::isNumBtn(activeBtnId)){
       // handle navigation buttons for tv device
      if (currentDeviceSelectBtnId == BOKeybad::BtnId::TV) {
        // different actions for each number
        switch (activeBtnId) {
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
    }
    // check if pressed button is a navigation button
    else if (BOKeybad::isNavigationBtn(activeBtnId)){
      // handle navigation buttons for tv device
      if (currentDeviceSelectBtnId == BOKeybad::BtnId::TV) {
        switch (activeBtnId) {
          case BOKeybad::BtnId::GREEN : {
            //send ir
            break;
          }
          case BOKeybad::BtnId::UP : {
            //send ir
            //irSender.sendSAMSUNG(IRConfig::IRCommands::tvUp, 32);
            irSender.sendRaw_P(IRConfig::IRCommands::Tv::tvUp, IRConfig::IRCommands::Tv::tvUpLength, NEC_KHZ);
            break;
          }
          case BOKeybad::BtnId::YELLOW : {
            //send ir
            break;
          }
          case BOKeybad::BtnId::LEFT : {
            //send ir
            irSender.sendRaw_P(IRConfig::IRCommands::Tv::tvLeft, IRConfig::IRCommands::Tv::tvLeftLength, NEC_KHZ);
            //irSender.sendSAMSUNG(IRConfig::IRCommands::tvLeft, 32);
            break;
          }
          case BOKeybad::BtnId::GO : {
            //send ir
            irSender.sendRaw_P(IRConfig::IRCommands::Tv::tvEnter, IRConfig::IRCommands::Tv::tvEnterLength, NEC_KHZ);
            //irSender.sendSAMSUNG(IRConfig::IRCommands::tvEnter, 32);
            break;
          }
          case BOKeybad::BtnId::RIGHT : {
            //send ir
            irSender.sendRaw_P(IRConfig::IRCommands::Tv::tvRight, IRConfig::IRCommands::Tv::tvRightLength, NEC_KHZ);
            //irSender.sendSAMSUNG(IRConfig::IRCommands::tvRight, 32);
            break;
          }
          case BOKeybad::BtnId::RED : {
            //send ir
            break;
          }
          case BOKeybad::BtnId::DOWN : {
            //send ir
            irSender.sendRaw_P(IRConfig::IRCommands::Tv::tvDown, IRConfig::IRCommands::Tv::tvDownLength, NEC_KHZ);
            //irSender.sendSAMSUNG(IRConfig::IRCommands::tvDown, 32);
            break;
          }
          case BOKeybad::BtnId::BLUE : {
            //send ir
            break;
          }
        }
      }
    }
    // check if pressed button is a menu button
    else if (BOKeybad::isMenuBtn(activeBtnId)){
      // handle navigation buttons for tv device
      if (currentDeviceSelectBtnId == BOKeybad::BtnId::TV) {
        switch (activeBtnId) {
          case BOKeybad::BtnId::MENU : {
            //send ir
            irSender.sendRaw_P(IRConfig::IRCommands::Tv::tvHome, IRConfig::IRCommands::Tv::tvHomeLength, NEC_KHZ);
            //irSender.sendSAMSUNG(IRConfig::IRCommands::tvHome, 32);
            break;
          }
          case BOKeybad::BtnId::EXIT : {
            //send ir
            irSender.sendRaw_P(IRConfig::IRCommands::Tv::tvBack, IRConfig::IRCommands::Tv::tvBackLength, NEC_KHZ);
            //irSender.sendSAMSUNG(IRConfig::IRCommands::tvBack, 32);
            break;
          }
        }
      }
    }
    // check if pressed button is a unique button
    else if (BOKeybad::isUniqueBtn(activeBtnId)) {
      switch (activeBtnId) {
        case BOKeybad::BtnId::VOLUP : {
          // turn volume up
          irSender.sendRaw_P(IRConfig::IRCommands::Amplifier::ampVolUp, IRConfig::IRCommands::Amplifier::ampVolUpLength, NEC_KHZ);
          break;
        }
        case BOKeybad::BtnId::VOLDOWN : {
          // turn volume down
          irSender.sendRaw_P(IRConfig::IRCommands::Amplifier::ampVolDown, IRConfig::IRCommands::Amplifier::ampVolDownLength, NEC_KHZ);
          break;
        }
        case BOKeybad::BtnId::OFF : {
          // turn TV off
          irSender.sendSAMSUNG(IRConfig::IRCommands::Tv::tvOff, 32); 
          delay(100);
          // turn amplifier off
          irSender.sendRaw_P(IRConfig::IRCommands::Amplifier::ampOff, IRConfig::IRCommands::Amplifier::ampOffLength, NEC_KHZ);
          break;
        }
      }
    }

    // clear active button indicator to be set with the next btn press
    activeBtnId = BOKeybad::BtnId::NONE;
    // update current time for last button press timeout
    upTime = millis();
  }

  // if no btn has been pressed in some time
  // send the cpu into sleep mode to save power
  if (millis() - upTime > sleepTimeout) {
    enterSleepMode();
    upTime = millis();
  }  
}

void gpioExpanderIsr() {
  // set irq flag to true for handling in main thread
  irqOccured = true;
}

void enterSleepMode() {
    // configure sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    // enter sleep mode
    sleep_mode();
    // return here when the gpio interrupt has fired
    sleep_disable();
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