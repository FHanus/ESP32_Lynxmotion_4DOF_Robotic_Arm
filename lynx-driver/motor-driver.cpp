#include "motor-driver.h"

bool actuatorsSetup() {
  bool success = true;

  if (!servo1.attach(SERVO_PINS[0])) success = false;
  if (!servo2.attach(SERVO_PINS[1])) success = false;
  if (!servo3.attach(SERVO_PINS[2])) success = false;
  if (!servo4.attach(SERVO_PINS[3])) success = false;
  if (!endEffector.attach(SERVO_PINS[4])) success = false;

  return success;
}

void motorControlTask(void* parameter) {
  while (true) {
    switch (currentState) {
      case STANDBY:
        Serial.println("State: Standby");
        break;
      case TEACH:
        Serial.println("State: Teach");
        break;
      case OPERATE:
        Serial.println("State: Operate");
        break;
    }
    switch (currentGripperState) {
      case GRIPPER_OPEN:
        Serial.println("Gripper State: Open");
        break;
      case GRIPPER_CLOSE:
        Serial.println("Gripper State: Close");
        break;
    }
    if (!currentAction.isEmpty()) {
      Serial.println("Action: " + currentAction);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
