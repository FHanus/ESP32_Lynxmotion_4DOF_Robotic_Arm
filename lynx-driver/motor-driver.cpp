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
        standbyState();
        break;
      case TEACH:
        teachState();
        break;
      case OPERATE:
        operationState();
        break;
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void standbyState(){
  Serial.print("\nState: Standby - Printing out client inputs only");

  switch (currentGripperState) {
      case GRIPPER_OPEN:
        Serial.print(" | Gripper State: Open");
        break;
      case GRIPPER_CLOSE:
        Serial.print(" | Gripper State: Close");
        break;
    }
  if (!currentAction.isEmpty()) {
    Serial.print(" | Action: " + currentAction);
  }
}

void teachState(){
  Serial.println("State: Teach");
  
}

void operationState(){
  Serial.println("State: Operate");
  
}