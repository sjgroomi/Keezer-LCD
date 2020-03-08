#include <LiquidCrystal.h>
#include "ProgressBar.h"
#include "Arduino.h"

#define ZERO byte(0)
#define ONE 1
#define TWO 2
#define FOUR 3
#define EIGHT 4
#define SMILE 5
#define LEVEL 6
#define FROWN 7

CustomChars::CustomChars(LiquidCrystal *lcd) {
  this->lcd = lcd;
  this->configureChars();
}

void CustomChars::configureChars() {
  byte zero[] = {
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
  };
  lcd->createChar(ZERO, zero);
  byte one[] = {
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b11111
  };
  lcd->createChar(ONE, one);
  byte two[] = {
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b11111,
    0b11111
  };
  lcd->createChar(TWO, two);
  byte four[] = {
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b11111,
    0b11111,
    0b11111,
    0b11111
  };
  lcd->createChar(FOUR, four);
  byte eight[] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
  };
  lcd->createChar(EIGHT, eight);
  byte smile[] = {
    B00000,
    B00000,
    B01010,
    B00000,
    B00000,
    B10001,
    B01110,
    B00000
  };
  lcd->createChar(SMILE, smile);
  byte level[] = {
    B00000,
    B00000,
    B01010,
    B00000,
    B00000,
    B11111,
    B00000,
    B00000
  };
  lcd->createChar(LEVEL, level);
  byte frown[] = {
    B00000,
    B00000,
    B01010,
    B00000,
    B00000,
    B01110,
    B10001,
    B00000
  };
  lcd->createChar(FROWN, frown);
}

void CustomChars::progressBar(int column, float percentage) {
  int bottom;
  int top;
  if (percentage > 0.466) {
    bottom = EIGHT;
    if (percentage < 0.533) {
      top = ZERO;
    } else if (percentage < 0.6) {
      top = ONE;
    } else if (percentage < 0.666) {
      top = TWO;
    } else if (percentage < 0.8) {
      top = FOUR;
    } else {
      top = EIGHT;
    }
  } else {
    top = ZERO;
    if (percentage > 0.2) {
      bottom = FOUR;
    } else if (percentage > 0.1) {
      bottom = TWO;
    } else {
      bottom = ONE;
    }
  }
  lcd->setCursor(column, 0);
  lcd->write(top);
  lcd->setCursor(column, 1);
  lcd->write(bottom);
}

void CustomChars::smile(int column, int row) {
  lcd->setCursor(column, row);
  lcd->write(SMILE);
}

void CustomChars::level(int column, int row) {
  lcd->setCursor(column, row);
  lcd->write(LEVEL);
}

void CustomChars::frown(int column, int row) {
  lcd->setCursor(column, row);
  lcd->write(FROWN);
}
