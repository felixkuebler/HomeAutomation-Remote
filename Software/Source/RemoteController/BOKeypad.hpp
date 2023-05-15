#include <avr/pgmspace.h>



enum BtnId: uint8_t {
  TV,
  LIGHT,
  RADIO,
  DTV,
  DVD,
  CD,
  VMEM,
  RECORD,
  AMEM,
  NUM7,
  NUM8,
  NUM9,
  NUM4,
  NUM5,
  NUM6,
  NUM1,
  NUM2,
  NUM3,
  TEXT,
  NUM0,
  MENU,
  GREEN,
  UP,
  YELLOW,
  LEFT,
  GO,
  RIGHT,
  RED,
  DOWN,
  BLUE,
  LIST,
  VOLUP,
  EXIT,
  STOP,
  VOLDOWN,
  OFF,
  NUM_IDS,
  NONE = 255
};

constexpr char* btnNames[BtnId::NUM_IDS] PROGMEM = {
  "TV", "LIGHT", "RADIO", "DTV", "DVD", "CD", "VMEM", "RECORD", "AMEM", "7", "8", "9", "4", "5", "6", "1", "2", "3",
  "TEXT", "0", "MENU", "GREEN", "UP", "YELLOW", "LEFT", "GO", "RIGHT", "RED", "DOWN", "BLUE", "LIST", "VOL UP", "EXIT", "STOP", "VOL DOWN", "OFF"
};

constexpr uint8_t numGpioOut = 6;
constexpr uint8_t numGpioIn = 6;

constexpr BtnId btnIdMap[numGpioOut][numGpioIn] PROGMEM = {
  {BtnId::TV, BtnId::DTV, BtnId::VMEM, BtnId::NUM7, BtnId::NUM4, BtnId::NUM1},
  {BtnId::LIGHT, BtnId::DVD, BtnId::RECORD, BtnId::NUM8, BtnId::NUM5, BtnId::NUM2},
  {BtnId::RADIO, BtnId::CD, BtnId::AMEM, BtnId::NUM9, BtnId::NUM6, BtnId::NUM3},
  {BtnId::TEXT, BtnId::GREEN, BtnId::LEFT, BtnId::RED, BtnId::LIST, BtnId::STOP},
  {BtnId::NUM0, BtnId::UP, BtnId::GO, BtnId::DOWN, BtnId::VOLUP, BtnId::VOLDOWN},
  {BtnId::MENU, BtnId::YELLOW, BtnId::RIGHT, BtnId::BLUE, BtnId::EXIT, BtnId::OFF}
};

BtnId gpioToBtnId(uint8_t outPin, uint8_t inPin){
  if (outPin < numGpioOut && inPin < numGpioIn) {
    return pgm_read_word(&btnIdMap[outPin][inPin]);
  }
  return BtnId::NONE;
}

char* getBtnName(BtnId id) {
  if (id < BtnId::NUM_IDS) {
    return pgm_read_word(&btnNames[id]);
  }
  return nullptr;
}















