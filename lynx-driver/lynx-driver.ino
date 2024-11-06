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

  xTaskCreatePinnedToCore(servoControlTask, "Servo Control Task", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(motorControlTask, "Motor Control Task", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(clientHandleTask, "Client Handle Task", 4096, NULL, 1, NULL, 0);
}

void loop() {
}
