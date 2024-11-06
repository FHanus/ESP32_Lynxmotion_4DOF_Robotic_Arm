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

  xTaskCreatePinnedToCore(
    motorControlTask,         // Task
    "Motor Control Task",     // Name
    4096,                     // Stack size
    NULL,                     // Task input
    1,                        // Priority
    NULL,                     // Task handle
    1);                       // Core

  xTaskCreatePinnedToCore(
    servoControlTask,         // Task
    "Servo Control Task",     // Name
    4096,                     // Stack size
    NULL,                     // Task input
    1,                        // Priority
    NULL,                     // Task handle
    1);                       // Core n

  xTaskCreatePinnedToCore(
    clientHandleTask,         // Task
    "Client Handle Task",     // Name
    4096,                     // Stack size
    NULL,                     // Task input
    1,                        // Priority 
    NULL,                     // Task handle
    0);                       // Core 
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}
