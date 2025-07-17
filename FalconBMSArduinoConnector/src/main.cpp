#include <FalconBMSArduinoConnector.h>
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8G2_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8G2_HAVE_HW_I2C
#include <Wire.h>
#endif

// Use Page Buffer instead of Full Buffer to save RAM

U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 5, /* dc=*/ 16, /* reset=*/ 17);	// Enable U8G2_16BIT in u8g2.h

FalconBMSArduinoConnector bms;

// Use char arrays instead of String to save RAM
char previousLines[5][27];  // 26 chars + null terminator

#if defined(ESP32)
const int ledPin = 2;
#else
const int ledPin = 13;
#endif

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  while (!Serial);
  bms.begin();

  u8g2.begin();
  u8g2.setFont(u8g2_font_6x13_tr);  // Fits 26x5 lines

  // Show startup message once
  u8g2.firstPage();
  do {
    u8g2.drawStr(0, 12, "U8G2 Library Init");
  } while (u8g2.nextPage());

  delay(1000);

  // Initialize previousLines with empty strings
  for (int i = 0; i < 5; i++) {
    previousLines[i][0] = '\0';
  }
}

void draw() {
  u8g2.clearBuffer();  // Clear internal RAM buffer

  for (int i = 0; i < 5; i++) {
    u8g2.drawStr(0, (i + 1) * 12, previousLines[i]);  // Adjust Y spacing as needed
  }

  u8g2.sendBuffer();  // Push buffer to screen
}


void loop() {
  bms.update();

  if (bms.isConnected()) {
    digitalWrite(ledPin, bms.isMasterCaution() ? HIGH : LOW);

    bool needsRedraw = false;
    for (int i = 0; i < 5; i++) {
      const char* currentLine = bms.dedLines[i];//.c_str();
      if (strcmp(currentLine, previousLines[i]) != 0) {
        strncpy(previousLines[i], currentLine, 26);
        previousLines[i][26] = '\0';
        needsRedraw = true;
      }
    }
    

    if (needsRedraw) {
    
    draw();
    
    //   u8g2.firstPage();
    //   do {
    //     for (int i = 0; i < 5; i++) {
    //       u8g2.drawStr(0, (i + 1) * 12, previousLines[i]);
    //     }
    //   } while (u8g2.nextPage());
    // }
  }
}
  else {
    digitalWrite(ledPin, LOW);

    // Display disconnected only once or when status changes to avoid flicker
    static bool wasConnected = true;
    if (wasConnected) {
      u8g2.firstPage();
      do {
        u8g2.drawStr(0, 12, "Disconnected");
      } while (u8g2.nextPage());
      wasConnected = false;
    }
  }

   // Tune delay as needed to reduce flicker and keep responsiveness
   delay(250); 
  }


