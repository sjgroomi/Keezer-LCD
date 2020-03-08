#include <HX711.h>
#include <avr/sleep.h>
#include <LiquidCrystal.h>
#include <avr/wdt.h>
#include "ProgressBar.h"

#define DOUT_SINGLE 12
#define DOUT_DUAL 13
#define CLK 11
#define SWITCHED_5V 8
#define RS 9
#define ENABLE 7
#define D4 6
#define D5 5
#define D6 4
#define D7 3
#define INTERRUPT 2


int gains[] = {128, 128, 32};
long tares[] = {0, 0, 0};//long tares[] = {-391167, -352510, -99842};
float calibrationFactors[] = {-89700, -90000, -21650};
HX711 single;
HX711 dual;
LiquidCrystal lcd = LiquidCrystal(RS, ENABLE, D4, D5, D6, D7);
ProgressBar progressBar = ProgressBar(&lcd);
volatile int notSureWhatToCallThis = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  configureScales();
  configurePins();
  printTare();
}

void configureWatchdog() {
   // disable ADC
  ADCSRA = 0;  

  // clear various "reset" flags
  MCUSR = 0;     
  // allow changes, disable reset
  WDTCSR = bit (WDCE) | bit (WDE);
  // set interrupt mode and an interval 
  WDTCSR = bit (WDIE) | bit (WDP2) | bit (WDP1) | bit (WDP0);
  wdt_reset();  // pat the dog
}

ISR (WDT_vect) 
{
  wdt_disable();
}

void configureScales() {
  single.begin(DOUT_SINGLE, CLK);
  single.set_scale(calibrationFactors[0]);
  single.set_offset(tares[0]);
  dual.begin(DOUT_DUAL, CLK);
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

void configurePins() {
  pinMode(SWITCHED_5V, OUTPUT);
  pinMode(INTERRUPT, INPUT_PULLUP);
}

void turnOnPower() {
  digitalWrite(SWITCHED_5V, HIGH);
}

void turnOffPower() {
  digitalWrite(SWITCHED_5V, LOW);
}

void goToSleep() {
  attachInterrupt(digitalPinToInterrupt(INTERRUPT), button_pressed, LOW);
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleepActions();
  sleep_cpu();
  wakeActions();
}

void sleepActions() {
  turnOffPower();
}

void wakeActions() {

}

void loop() {
  wdt_enable(WDTO_2S);
  updateWeights();
  configureWatchdog();
  wdt_reset();
  if (isPowerOn()) {
    delay(5000);
  }
  goToSleep();
}

bool isPowerOn() {
  return digitalRead(SWITCHED_5V) == HIGH;
}

void updateWeights() {
  for (int i = 0; i < 3; i++) {
    displayWeightForKeg(i);
  }
}

const float fullUnits = 22.72;
void displayWeightForKeg(int keg) {
  switch (keg) {
    case 0:
    lcd.setCursor(0, 0);
    break;
    case 1:
    lcd.setCursor(9,0);
    break;
    case 2:
    lcd.setCursor(4, 1);
    break;
  }
  HX711 hx711 = scale(keg);
  float units = max(0, hx711.get_units());
  float percentage = units / fullUnits;
  progressBar.write(percentage, 7);
}

void button_pressed() {
  turnOnPower();
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(INTERRUPT));
}
