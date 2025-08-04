#include <Arduino.h>
#include <FalconBMSArduinoConnector.h>
#include <LedControl.h>

LedControl lc = LedControl(11, 13, 10, 1);  // DIN, CLK, CS, 1 device

FalconBMSArduinoConnector bms;

void displayUHF(long val1, int val2);

void setup() {
  Serial.begin(115200);
  bms.begin(Serial);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.setScanLimit(0, 7);
  lc.clearDisplay(0);
}


void loop() {
  bms.update();
  if (bms.isConnected()) {
   bms.getuhfFreq();
   bms.getuhfPreset();
    displayUHF(bms.uhfFreq, bms.uhfPreset);

  }else{
    lc.clearDisplay(0);
  }
 
}

void displayUHF(long val1, int val2) {
  
 long number = val1;
  
  for (int i = 5; i >= 0; i--) {
    byte digit = number % 10;
    Serial.println(digit);
    lc.setDigit(0, i, digit, false);
    // Add decimal point after third digit (i == 2)
    bool dp = (i == 2);  // dp = true only at position 2
    lc.setDigit(0, i, digit, dp);
    number /= 10;
}

  // Display preset on digit 7
  lc.setDigit(0, 7, val2 % 10, false);  // Only last digit of preset
  return;
}
