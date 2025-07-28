#define FBAC_FF_1306 //SSD1306
//#define FBAC_FF_1107  //SH1107
#include <Arduino.h>
#include <FalconBMSArduinoConnector.h>

FalconBMSArduinoConnector bms;

#ifdef FBAC_FF_1306
char FuelFlow[6]; // +1 for null terminator
#define FF_CHAR_W 20
#define FF_CHAR_H 30
#define FF_H_CONST 0
#define FF_V_CONST 0

#define FFI_SCREEN_W 128
#define FFI_SCREEN_H 64
const unsigned short FFI_SCREEN_W_MID = FFI_SCREEN_W / 2;
const unsigned short FFI_SCREEN_H_MID = 40;

const unsigned short FF_POS_X_1 = int(FFI_SCREEN_W_MID - ((FF_CHAR_W * 7) / 2)) + ((FF_CHAR_W + 1) * 1) + FF_H_CONST;
const unsigned short FF_POS_X_2 = int(FFI_SCREEN_W_MID - ((FF_CHAR_W * 7) / 2)) + ((FF_CHAR_W + 1) * 2) + FF_H_CONST;
const unsigned short FF_POS_X_3 = int(FFI_SCREEN_W_MID - ((FF_CHAR_W * 7) / 2)) + ((FF_CHAR_W + 1) * 3) + FF_H_CONST;
const unsigned short FF_POS_X_4 = int(FFI_SCREEN_W_MID - ((FF_CHAR_W * 7) / 2)) + ((FF_CHAR_W + 1) * 4) + FF_H_CONST;
const unsigned short FF_POS_X_5 = int(FFI_SCREEN_W_MID - ((FF_CHAR_W * 7) / 2)) + ((FF_CHAR_W + 1) * 5) + FF_H_CONST;
const unsigned short FF_POS_Y = FFI_SCREEN_H_MID + FF_V_CONST;
#endif

void showStartupScreen() {
  u8g2_FuelFlow.firstPage();
  do {
    u8g2_FuelFlow.setFont(ffFont); // Nice readable default font
    u8g2_FuelFlow.setFontPosTop();
    u8g2_FuelFlow.setCursor(15, 18);
    u8g2_FuelFlow.print("FUELFLOW");
    
  } while (u8g2_FuelFlow.nextPage());

  delay(1000); // Hold startup screen for 2 seconds
}

void setFuelFlowValue(float flow) {
  int intFlow = constrain(round(flow), 0, 99999);
  snprintf(FuelFlow, sizeof(FuelFlow), "%05d", intFlow);
}

void drawFF() {
  if (FuelFlow[2] == 0) return;

  char FFh = FuelFlow[2];
  char FFhPriv = (FFh == '0') ? '9' : FFh - 1;
  char FFhNext = (FFh == '9') ? '0' : FFh + 1;
  char FFhTwoOver = (FFhNext == '9') ? '0' : FFhNext + 1;

  char FFtt = FuelFlow[0];
  char FFt = FuelFlow[1];

  short offset = short((FuelFlow[3] - '0') * FF_CHAR_H / 10);

  u8g2_FuelFlow.firstPage();
  do {
    u8g2_FuelFlow.setFont(ffFont);
    u8g2_FuelFlow.setFontPosCenter();

    // First two digits (no animation)
    u8g2_FuelFlow.setCursor(FF_POS_X_1, FFI_SCREEN_H_MID);
    u8g2_FuelFlow.print(FFtt);
    u8g2_FuelFlow.setCursor(FF_POS_X_2, FFI_SCREEN_H_MID);
    u8g2_FuelFlow.print(FFt);

    // Animate hundreds
    u8g2_FuelFlow.setCursor(FF_POS_X_3, FFI_SCREEN_H_MID + ((FF_CHAR_H + 1) * -2) + offset);
    u8g2_FuelFlow.print(FFhTwoOver);
    u8g2_FuelFlow.setCursor(FF_POS_X_3, FFI_SCREEN_H_MID + ((FF_CHAR_H + 1) * -1) + offset);
    u8g2_FuelFlow.print(FFhNext);
    u8g2_FuelFlow.setCursor(FF_POS_X_3, FFI_SCREEN_H_MID + offset);
    u8g2_FuelFlow.print(FFh);
    u8g2_FuelFlow.setCursor(FF_POS_X_3, FFI_SCREEN_H_MID + ((FF_CHAR_H + 1) * 1) + offset);
    u8g2_FuelFlow.print(FFhPriv);

    // Static trailing 00
    u8g2_FuelFlow.setCursor(FF_POS_X_4, FFI_SCREEN_H_MID);
    u8g2_FuelFlow.print('0');
    u8g2_FuelFlow.setCursor(FF_POS_X_5, FFI_SCREEN_H_MID);
    u8g2_FuelFlow.print('0');

  } while (u8g2_FuelFlow.nextPage());
}

void setup() {
  u8g2_FuelFlow.begin();  // Initialize display
  showStartupScreen();
  Serial.begin(115200);
  bms.begin(Serial);
}

void loop() {

    bms.update();

    if(bms.isConnected()){
        bms.getFuelFlow();
        setFuelFlowValue(bms.fuelFlow);
        drawFF();

    } else {
        u8g2_FuelFlow.firstPage();
        do {
            u8g2_FuelFlow.setFont(ffFont); // Nice readable default font
            u8g2_FuelFlow.setFontPosTop();
            u8g2_FuelFlow.setCursor(15, 18);
            u8g2_FuelFlow.print("00000");
            
        } while (u8g2_FuelFlow.nextPage());
    }  
}
