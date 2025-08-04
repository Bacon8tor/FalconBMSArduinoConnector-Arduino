
#include <FalconBMSArduinoConnector.h>

FalconBMSArduinoConnector bms;



void setup() {

  Serial.begin(115200);
  bms.begin(Serial);
}


void loop() {
  bms.update();
  if (bms.isConnected()) {
   
  }else{

  }
 
}
