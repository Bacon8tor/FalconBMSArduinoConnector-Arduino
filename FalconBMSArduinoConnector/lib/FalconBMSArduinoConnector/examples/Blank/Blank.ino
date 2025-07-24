
#include <FalconBMSArduinoConnector.h>

FalconBMSArduinoConnector bms;



void setup() {

  bms.begin();

}


void loop() {
  bms.update();
  if (bms.isConnected()) {
   
  }else{

  }
 
}
