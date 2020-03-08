#include <LiquidCrystal.h>
#include "ProgressBar.h"
#include "Arduino.h"

ProgressBar::ProgressBar(LiquidCrystal *lcd) {
  this->lcd = lcd;
  this->configureChars();
}

void ProgressBar::configureChars() {
  byte one[] = {
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
  byte two[] = {
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
  byte three[] = {
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
  byte four[] = {
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
  byte five[] = {
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
  byte six[] = {
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
    if (percentagePerSegment * (i + 1) < percentage) {
       lcd->write(6);
       continue;
    }
    float percentagePerLine = percentagePerSegment / 5;
    float remainder = percentage - i * percentagePerSegment;
    if (remainder < 0) {
      lcd->write(1);
      continue;
    }
    int character = max(1, remainder / percentagePerLine);
    Serial.println(character);
    lcd->write(character);
  }
}
