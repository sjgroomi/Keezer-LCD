#include <HX711.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <LiquidCrystal.h>
#include <avr/wdt.h>
#include "ProgressBar.h"

#define DOUT_SINGLE 12
#define DOUT_DUAL 11
#define CLK 10
#define SWITCHED_5V 9
#define RS 8
#define ENABLE 7
#define D4 6
#define D5 5
#define D6 4
#define D7 3
#define INTERRUPT 2


int gains[] = {128, 128, 32};
long tares[] = {-391167, -352510, -99842};
float calibrationFactors[] = {-89700, -90000, -19268};
HX711 single;
HX711 dual;
LiquidCrystal lcd = LiquidCrystal(RS, ENABLE, D4, D5, D6, D7);
CustomChars customChars = CustomChars(&lcd);

void setup() {
  configureLowPower();
  
  Serial.begin(9600);
  lcd.begin(16,2);
  
  configureScales();
  configurePins();
  printTare();
  turnOnPower();
}

int loops = 0;
void loop() {
  if (loops > 10) {
    goToSleep();
    loops = 0;
  }
  loops++;
  updateWeights();
  delay(200);
}

void printTare() {
  Serial.println("Tare recommendation:");
  for (int i = 0; i < 3; i++) {
    HX711 s = scale(i);
    Serial.print(i);
    Serial.print(": ");
    Serial.println(s.read_average());
  }
}

void configureLowPower() {
  // disable ADC
  ADCSRA = 0; 
  power_all_disable();
  power_timer0_enable();
}

//CONFIGURATION

void configureScales() {
  single.begin(DOUT_SINGLE, CLK);
  single.set_scale(calibrationFactors[0]);
  single.set_offset(tares[0]);
  dual.begin(DOUT_DUAL, CLK);
}

void configurePins() {
  pinMode(SWITCHED_5V, OUTPUT);
  pinMode(INTERRUPT, INPUT_PULLUP);
}

//DISPLAY

void updateWeights() {
  Serial.println("Update weights");
  for (int i = 0; i < 3; i++) {
    displayWeightForKeg(i);
  }
}

const float fullUnits = 22.72;
void displayWeightForKeg(int keg) {
  int column;
  switch (keg) {
    case 0:
    column = 0;
    break;
    case 1:
    column = 5;
    break;
    case 2:
    column = 10;
    break;
  }
  HX711 hx711 = scale(keg);
  float units = hx711.get_units();// max(0, hx711.get_units());
  float percentage = units / fullUnits;
  customChars.progressBar(column, percentage);
  displaySmiley(column + 3, percentage);
  displayPints(column + 1, percentage);
}

HX711 scale(int index) {
  switch (index) {
    case 0:
    single.set_gain(128);
    return single;
    case 1:
    case 2:
    dual.set_gain(gains[index]);
    dual.set_scale(calibrationFactors[index]);
    dual.set_offset(tares[index]);
    return dual;
  }
}

void displaySmiley(int column, float percentage) {
  if (percentage < 0.33) {
    customChars.frown(column, 0);
  } else if (percentage < 0.66) {
    customChars.level(column, 0);
  } else {
    customChars.smile(column, 0);
  }
}

void displayPints(int column, float percentage) {
  lcd.setCursor(column, 0);
  char pints[2];
  dtostrf(round(percentage * 40), 2, 0, pints);
  int iPints = atoi(pints);
  char paddedPints[2];
  sprintf(paddedPints, "%02d", iPints);
  lcd.print(paddedPints);
  lcd.setCursor(column, 1);
  lcd.print("pts");
}

//SLEEP / WAKE

void button_pressed() {
  turnOnPower();
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(INTERRUPT));
}

void turnOnPower() {
  digitalWrite(SWITCHED_5V, HIGH);
}

void turnOffPower() {
  digitalWrite(SWITCHED_5V, LOW);
}

void goToSleep() {
  attachInterrupt(digitalPinToInterrupt(INTERRUPT), button_pressed, LOW);
  noInterrupts();
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleepActions();
  // turn off brown-out enable in software
  MCUCR = bit (BODS) | bit (BODSE);  // turn on brown-out enable select
  MCUCR = bit (BODS);        // this must be done within 4 clock cycles of above
  interrupts();
  sleep_cpu();
  wakeActions();
}

void sleepActions() {
  turnOffPower();
}


void wakeActions() {
}
