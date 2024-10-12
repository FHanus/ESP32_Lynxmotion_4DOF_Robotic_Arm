#include "definitions.h"
#include "motor-driver.h"
#include "index-handlers.h"
#include "config.h"

void setup() {
  Serial.begin(115200);

  if (!actuatorsSetup()) {
    Serial.println("Error initializing actuators.");
  }

  if (!serverSetup()) {
    Serial.println("Error setting up server.");
  }

  // Motor control task pinned to core 0
  xTaskCreatePinnedToCore(motorControlTask, "MotorControl", 10000, NULL, 1, NULL, 0);

  // Client handle task pinned to core 1
  xTaskCreatePinnedToCore(clientHandleTask, "ClientHandle", 10000, NULL, 1, NULL, 1);
}

void loop() {
}