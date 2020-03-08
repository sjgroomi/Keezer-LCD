#include <LiquidCrystal.h>
#include "ProgressBar.h"

ProgressBar::ProgressBar(LiquidCrystal *lcd) {
  this->lcd = lcd;
  this->configureChars();
}

void ProgressBar::configureChars() {
  uint8_t one[] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111
  };
  lcd->createChar(1, one);
  uint8_t two[] = {
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b11111
  };
  lcd->createChar(2, two);
  uint8_t three[] = {
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11111
  };
  lcd->createChar(3, three);
  uint8_t four[] = {
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11111
  };
  lcd->createChar(4, four);
  uint8_t five[] = {
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11111
  };
  lcd->createChar(5, five);
  uint8_t six[] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
  };
  lcd->createChar(6, six);
}

void ProgressBar::write(float percentage, int width) {
  float percentagePerSegment = 1.0 / width;
  for (int i = 0; i < width; i++) {
    if (percentagePerSegment * width > percentage) {
       lcd->write(6);
       return;
    }
    float percentagePerLine = percentagePerSegment / 5;
    float remainder = percentage - i * percentagePerSegment;
    int character = remainder / percentagePerLine;
    lcd->write(character);
  }
}
