#include "definitions.h"
#include "motor-driver.h"
#include "index-handlers.h"
#include "config.h"

void setup() {
  Serial.begin(115200);

  if (!actuatorsSetup()) {
    Serial.println("Failed to initialize actuators.");
  }

  if (!serverSetup()) {
    Serial.println("Failed to initialize server.");
  }

  xTaskCreatePinnedToCore(
    servoControlTask,
    "Servo Control Task",
    10000,
    NULL,
    1,
    NULL,
    1
  );

  xTaskCreatePinnedToCore(
    motorControlTask,
    "Motor Control Task",
    10000,
    NULL,
    1,
    NULL,
    1
  );

  xTaskCreatePinnedToCore(
    clientHandleTask,
    "Client Handle Task",
    10000,
    NULL,
    1,
    NULL,
    0
  );
}


void loop() {
}