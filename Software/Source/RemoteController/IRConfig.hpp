#include <avr/pgmspace.h>

namespace IRConfig {

namespace IRCommands {

namespace Tv {

constexpr uint16_t tvOn[] PROGMEM = {
  4523,4523, 552,1683, 552,1683, 552,1683, 552,552, 552,552, 552,552, 552,552, 552,552, 552,1683,
  552,1683, 552,1683, 552,552, 552,552, 552,552, 552,552, 552,552, 552,1683, 552,552, 552,552, 552,1683,
  552,1683, 552,552, 552,552, 552,1683, 552,552, 552,1683, 552,1683, 552,552, 552,552, 552,1683,
  552,1683, 552,552, 552,43993
};
constexpr uint8_t tvOnLength = sizeof(tvOn)/sizeof(tvOn[0]);

constexpr uint16_t tvOff[] PROGMEM = {
  4523,4523, 552,1683, 552,1683, 552,1683, 552,552, 552,552, 552,552, 552,552, 552,552, 552,1683,
  552,1683, 552,1683, 552,552, 552,552, 552,552, 552,552, 552,552, 552,552, 552,552, 552,552, 552,1683,
  552,1683, 552,552, 552,552, 552,1683, 552,1683, 552,1683, 552,1683, 552,552, 552,552, 552,1683, 
  552,1683, 552,552, 552,43993
};
constexpr uint8_t tvOffLength = sizeof(tvOff)/sizeof(tvOff[0]);

constexpr uint16_t tvUp[] PROGMEM = {
  4480,4420, 580,1620, 580,1670, 530,1670, 580,520, 580,520, 580,520, 580,520, 580,520, 580,1670,
  530,1670, 580,1620, 580,520, 580,570, 530,570, 530,570, 580,520, 580,520, 580,520, 580,520, 580,570,
  530,570, 530,1670, 580,1620, 580,520, 580,1670, 530,1670, 580,1620, 580,1670, 530,1670, 580,520,
  580,520, 580,1620, 580
};
constexpr uint8_t tvUpLength = sizeof(tvUp)/sizeof(tvUp[0]);

constexpr uint16_t tvDown[] PROGMEM = {
  4430,4470, 530,1670, 580,1620, 580,1670, 530,570, 530,570, 530,570, 580,520, 580,520, 580,1670,
  530,1670, 530,1670, 580,520, 580,570, 530,570, 530,570, 530,570, 530,1670, 580,520, 580,570, 530,570,
  530,570, 530,1670, 530,1670, 580,570, 530,570, 530,1670, 530,1670, 580,1670, 530,1670, 530,570,
  530,570, 530,1670, 580
};
constexpr uint8_t tvDownLength = sizeof(tvDown)/sizeof(tvDown[0]);

constexpr uint16_t tvLeft[] PROGMEM = {
  4430,4470, 530,1670, 530,1670, 530,1720, 530,570, 530,570, 530,570, 530,570, 530,570, 530,1670,
  580,1670, 530,1670, 530,570, 530,570, 580,520, 580,570, 530,570, 530,1670, 530,570, 530,1670, 580,570,
  530,570, 530,1670, 530,1670, 530,570, 580,570, 530,1670, 530,570, 530,1670, 530,1720, 530,570,
  530,570, 530,1670, 530
};
constexpr uint8_t tvLeftLength = sizeof(tvLeft)/sizeof(tvLeft[0]);

constexpr uint16_t tvRight[] PROGMEM = {
  4480,4420, 580,1620, 580,1670, 530,1670, 580,520, 580,520, 580,520, 580,520, 580,570, 530,1670,
  580,1620, 580,1620, 580,570, 530,570, 580,520, 580,520, 580,520, 580,520, 580,1670, 530,570, 580,520,
  580,520, 580,1620, 580,1670, 530,570, 580,1620, 580,520, 580,1620, 580,1670, 580,1620, 580,520,
  580,520, 580,1620, 580
};
constexpr uint8_t tvRightLength = sizeof(tvRight)/sizeof(tvRight[0]);

constexpr uint16_t tvEnter[] PROGMEM = {
  4480,4420, 580,1670, 530,1670, 580,1620, 580,520, 580,520, 580,570, 530,570, 580,520, 580,1620,
  580,1620, 580,1670, 530,570, 580,520, 580,520, 580,520, 580,520, 580,520, 580,570, 530,570, 580,1620,
  580,520, 580,1620, 580,1670, 530,570, 580,1620, 580,1620, 580,1670, 530,570, 580,1620, 580,520,
  580,520, 580,1670, 530
};
constexpr uint8_t tvEnterLength = sizeof(tvEnter)/sizeof(tvEnter[0]);

constexpr uint16_t tvHome[] PROGMEM = {
  4480,4420, 580,1670, 580,1620, 580,1620, 580,520, 580,520, 580,570, 530,570, 580,520, 580,1620,
  580,1620, 580,1670, 580,520, 580,520, 580,520, 580,520, 580,520, 580,1670, 580,520, 580,520, 580,1620,
  580,1670, 530,1670, 580,1620, 580,520, 580,520, 580,1670, 580,1620, 580,520, 580,520, 580,520,
  580,570, 530,1670, 580
};
constexpr uint8_t tvHomeLength = sizeof(tvHome)/sizeof(tvHome[0]);

constexpr uint16_t tvBack[] PROGMEM = {
  4480,4470, 530,1670, 580,1620, 580,1620, 580,570, 530,570, 580,520, 580,520, 580,520, 580,1620,
  580,1670, 580,1620, 580,520, 580,520, 580,520, 580,570, 530,570, 580,520, 580,520, 580,520, 580,1620,
  580,1670, 580,520, 580,1620, 580,520, 580,1670, 530,1670, 580,1620, 580,520, 580,520, 580,1670,
  580,520, 580,1620, 580
};
constexpr uint8_t tvBackLength = sizeof(tvBack)/sizeof(tvBack[0]);

};

namespace Amplifier {

constexpr uint16_t ampOn[] PROGMEM = {
  8930,4370, 630,1570, 630,1570, 630,470, 630,470, 630,470, 630,470, 630,470, 630,1620, 580,1620,
  630,470, 630,470, 630,470, 630,1570, 630,470, 630,470, 630,470, 630,470, 630,470, 630,470, 630,1570,
  630,1620, 580,520, 630,470, 630,1570, 630,1570, 630,1570, 630,1570, 630,470, 630,470, 630,1570, 
  630,1570, 630,470, 630
};
constexpr uint8_t ampOnLength = sizeof(ampOn)/sizeof(ampOn[0]);

constexpr uint16_t ampOff[] PROGMEM = {
  8880,4420, 580,1620, 580,1620, 580,520, 580,570, 530,570, 530,570, 530,570, 580,1620, 580,1620,
  580,520, 580,520, 580,520, 580,1620, 580,520, 580,520, 580,520, 580,570, 580,470, 580,570, 580,520,
  580,520, 580,520, 580,520, 580,1620, 580,1620, 580,1620, 580,1620, 580,1620, 580,1670, 530,1670,
  580,1620, 580,520, 580
};
constexpr uint8_t ampOffLength = sizeof(ampOff)/sizeof(ampOff[0]);

constexpr uint16_t ampVolUp[] PROGMEM = {
  8930,4370, 630,1570, 630,1570, 630,470, 630,520, 530,570, 580,520, 580,520, 580,1620, 580,1620,
  580,520, 580,520, 580,520, 580,1620, 630,470, 630,470, 630,470, 630,470, 630,1570, 630,1620, 580,520,
  580,1620, 580,520, 580,520, 580,520, 580,1620, 630,470, 630,470, 630,1570, 630,470, 630,1620,
  580,1620, 580,1620, 580
};
constexpr uint8_t ampVolUpLength = sizeof(ampVolUp)/sizeof(ampVolUp[0]);

constexpr uint16_t ampVolDown[] PROGMEM = {
  8880,4420, 580,1620, 580,1620, 580,520, 580,520, 580,520, 580,520, 580,520, 580,1620, 630,1570,
  630,520, 580,470, 630,520, 580,1620, 580,520, 580,520, 580,520, 580,1620, 580,1620, 580,1620, 630,470,
  630,1570, 630,520, 580,520, 580,520, 580,520, 580,520, 580,520, 580,1620, 580,520, 580,1620,
  630,1570, 630,1570, 630
}; 
constexpr uint8_t ampVolDownLength = sizeof(ampVolDown)/sizeof(ampVolDown[0]);

};

};

};