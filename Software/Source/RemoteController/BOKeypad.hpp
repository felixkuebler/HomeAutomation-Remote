#include <avr/pgmspace.h>

namespace BOKeybad {

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

constexpr uint8_t numGpioOut = 6;
constexpr uint8_t numGpioIn = 6;

constexpr char* btnNames[BtnId::NUM_IDS] PROGMEM = {
  "TV", "LIGHT", "RADIO", "DTV", "DVD", "CD", "VMEM", "RECORD", "AMEM", "7", "8", "9", "4", "5", "6", "1", "2", "3",
  "TEXT", "0", "MENU", "GREEN", "UP", "YELLOW", "LEFT", "GO", "RIGHT", "RED", "DOWN", "BLUE", "LIST", "VOL UP", "EXIT", "STOP", "VOL DOWN", "OFF"
};

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

bool isDeviceSelectBtn(BtnId id) {
  return id < BtnId::NUM7;
}

bool isNumBtn(BtnId id) {
  return id > BtnId::AMEM && id < BtnId::MENU && id != BtnId::TEXT;
}

bool isNavigationBtn(BtnId id) {
  return id > BtnId::MENU && id < BtnId::LIST;
}

bool isMenuBtn(BtnId id) {
  return id == BtnId::TEXT || id == BtnId::MENU || id == BtnId::LIST || id == BtnId::EXIT || id == BtnId::STOP;
}

bool isUniqueBtn(BtnId id) {
  return id == BtnId::VOLUP || id == BtnId::VOLDOWN || id == BtnId::OFF;
}

};