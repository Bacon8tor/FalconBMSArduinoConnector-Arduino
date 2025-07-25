#include <Wire.h>
#include <PCF8575.h>

#include <Joystick.h>


// PCF8575 I2C addresses (adjust as needed)
PCF8575 pcf_1(0x20);
PCF8575 pcf_2(0x22);

// Create Joystick with 32 buttons, no axes
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_GAMEPAD,
                   32, 0,  // Buttons, Hat switches
                   false, false, false, // X, Y, Z Axis
                   false, false, false, // Rx, Ry, Rz
                   false, false,        // Rudder, Throttle
                   false, false, false);// Accelerator, Brake, Steering

// Store previous button states
bool lastState[32];

void setup() {
#ifdef __AVR_ATmega32U4__
  while (!Serial);  // Wait for USB serial on Pro Micro
#endif

  Serial.begin(115200);
  Wire.begin();

  // Initialize PCF8575 expanders
  pcf_1.begin();
  pcf_2.begin();

  // Set all pins to input with pull-ups
  for (uint8_t i = 0; i < 16; i++) {
    pcf_1.write(i, HIGH);
    pcf_2.write(i, HIGH);
    lastState[i] = true;          // Default state = HIGH (not pressed)
    lastState[i + 16] = true;
  }
  // Start the Joystick
  Joystick.begin();


  
}

void checkButtons() {
  // Check PCF8575 #1 (buttons 0–15)
  for (uint8_t i = 0; i < 16; i++) {
    bool currentState = pcf_1.read(i);
    if (currentState != lastState[i]) {
      // Serial.print("[BUTTON] Pin ");
      // Serial.print(i);
      // Serial.print(" changed to ");
      // Serial.println(currentState == LOW ? "PRESSED" : "RELEASED");
      Joystick.setButton(i, !currentState); // Invert: LOW = pressed
      lastState[i] = currentState;
    }
  }

  // Check PCF8575 #2 (buttons 16–31)
  for (uint8_t i = 0; i < 16; i++) {
    uint8_t btnIndex = i + 16;
    bool currentState = pcf_2.read(i);
    if (currentState != lastState[btnIndex]) {
      // Serial.print("[BUTTON] Pin ");
      // Serial.print(btnIndex);
      // Serial.print(" changed to ");
      // Serial.println(currentState == LOW ? "PRESSED" : "RELEASED");
      Joystick.setButton(btnIndex, !currentState); // Invert: LOW = pressed
      lastState[btnIndex] = currentState;
    }
  }
}

void loop() {
 
  checkButtons();
  delay(5); // Small debounce and reduce I2C traffic
}
