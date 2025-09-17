/*
  Falcon BMS Arduino Connector - I2C Compatibility Demo

  This example demonstrates that the I2C slave provides EXACTLY the same
  interface as the original FalconBMSArduinoConnector library.

  You can take ANY existing BMS code and just change:
  - FalconBMSArduinoConnector bms; -> FBAC_I2C_Slave slave(0x10);
  - bms.function() -> slave.function()
  - bms.variable -> slave.variable

  Everything else works exactly the same!

  I2C Address: 0x10

  Created by: Bacon8tor
  Date: 2024
*/

#include <FBAC_I2C_Slave.h>

#define SLAVE_ADDRESS 0x10

// Create I2C slave - this replaces FalconBMSArduinoConnector bms;
FBAC_I2C_Slave slave(SLAVE_ADDRESS);

void setup() {
  Serial.begin(115200);

  // Initialize I2C slave - replaces bms.begin()
  slave.begin();

  Serial.println("=== I2C Compatibility Demo ===");
  Serial.println("Showing exact same interface as original BMS library");
  Serial.println();
}

void loop() {
  // Update slave - replaces bms.update()
  slave.update();

  if (slave.isConnected()) {  // Same as bms.isConnected()

    // ===== EXACT SAME FUNCTION CALLS AS ORIGINAL BMS LIBRARY =====

    // Warning light functions - ALL work exactly the same
    if (slave.isMasterCaution()) {  // Same as bms.isMasterCaution()
      Serial.println("MASTER CAUTION ON");
    }

    if (slave.isEngFire()) {        // Same as bms.isEngFire()
      Serial.println("ENGINE FIRE!");
    }

    if (slave.isConfig()) {         // Same as bms.isConfig()
      Serial.println("CONFIG WARNING");
    }

    if (slave.isFuelLow()) {        // Same as bms.isFuelLow()
      Serial.println("FUEL LOW");
    }

    // ===== EXACT SAME VARIABLE ACCESS AS ORIGINAL BMS LIBRARY =====

    Serial.print("Fuel Flow: ");
    Serial.print(slave.fuelFlow);           // Same as bms.fuelFlow
    Serial.println(" PPH");

    Serial.print("Internal Fuel: ");
    Serial.print(slave.internalFuel);       // Same as bms.internalFuel
    Serial.println(" lbs");

    Serial.print("RPM: ");
    Serial.print(slave.rpm);                // Same as bms.rpm
    Serial.println("%");

    Serial.print("Oil Pressure: ");
    Serial.print(slave.oilPress);           // Same as bms.oilPress
    Serial.println(" PSI");

    Serial.print("KIAS: ");
    Serial.print(slave.kias);               // Same as bms.kias
    Serial.println(" knots");

    Serial.print("Cabin Alt: ");
    Serial.print(slave.cabinAlt);           // Same as bms.cabinAlt
    Serial.println(" ft");

    // ===== EXACT SAME ARRAY ACCESS AS ORIGINAL BMS LIBRARY =====

    Serial.println("DED Lines:");
    for (int i = 0; i < 5; i++) {
      Serial.print("  Line ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(slave.dedLines[i]);    // Same as bms.dedLines[i]
    }

    // ===== EXACT SAME ECM ACCESS AS ORIGINAL BMS LIBRARY =====

    Serial.print("ECM Panel 0 Status: ");
    Serial.println(slave.getECMStatus(0));  // Same as bms.getECMStatus(0)

    // ===== EXACT SAME LIGHTING ACCESS AS ORIGINAL BMS LIBRARY =====

    Serial.print("Instrument Lighting: ");
    Serial.println(slave.getInstrLightStatus());  // Same as bms.getInstrLightStatus()

    Serial.println("---");

  } else {
    Serial.println("Waiting for master connection...");
  }

  delay(2000);  // Update every 2 seconds for demo
}

/*
  MIGRATION GUIDE - How to convert existing BMS code:

  OLD CODE (Direct serial connection):
  ====================================
  #include <FalconBMSArduinoConnector.h>

  FalconBMSArduinoConnector bms;

  void setup() {
    Serial.begin(115200);
    bms.begin(Serial);
  }

  void loop() {
    bms.update();

    if (bms.isConnected()) {
      bms.getLightBits(1);
      bms.getFuelFlow();

      if (bms.isMasterCaution()) {
        // Flash light
      }

      float fuel = bms.fuelFlow;
      // Display fuel
    }
  }

  NEW CODE (I2C slave):
  =====================
  #include <FBAC_I2C_Slave.h>

  FBAC_I2C_Slave slave(0x10);  // Only change: specify I2C address

  void setup() {
    Serial.begin(115200);
    slave.begin();              // Same function name
  }

  void loop() {
    slave.update();             // Same function name

    if (slave.isConnected()) {  // Same function name
      // No need to request data - master sends it automatically

      if (slave.isMasterCaution()) {  // Same function name
        // Flash light
      }

      float fuel = slave.fuelFlow;     // Same variable name
      // Display fuel
    }
  }

  THAT'S IT! Just change "bms" to "slave" and remove data request calls.
  Everything else works exactly the same!
*/