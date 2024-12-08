#include "definitions.h"
#include "motor-driver.h"
#include "index-handlers.h"

void setup() {
  Serial.begin(115200);
  
  if (!actuatorsSetup()) {
    Serial.println("Failed to initialize actuators.");
  }

  if (!serverSetup()) {
    Serial.println("Failed to start server.");
  }

  // No separate tasks needed for servo control now.
  // We'll handle control directly in handleServoControl and loop.

  // Just run the server in the loop.
}

void loop() {
  server.handleClient();

  // If in OPERATION mode, replay positions if needed
  if (currentState == OPERATE) {
    runReplayCycle();
  }

  delay(10);
}
