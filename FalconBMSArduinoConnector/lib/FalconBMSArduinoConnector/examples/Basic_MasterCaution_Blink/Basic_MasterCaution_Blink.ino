#include <FalconBMSArduinoConnector.h>

FalconBMSArduinoConnector bms;

#if defined(ESP32)
  const int ledPin = 2;  // Most ESP32 boards use GPIO2 for the onboard LED
#else
  const int ledPin = LED_BUILTIN;  // Fallback for Arduino Uno, Mega, Nano etc.
#endif

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  bms.begin();
}

void loop() {
  bms.update();
  if (bms.isConnected()) {
    digitalWrite(ledPin, bms.isMasterCaution() ? HIGH : LOW); // CHECKS WHETHER isMasterCaution true or false then sets high or low 
    
  } else {
    digitalWrite(ledPin, LOW);
  }
 
}
