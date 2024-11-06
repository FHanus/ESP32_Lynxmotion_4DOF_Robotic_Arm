#include "motor-driver.h"
#include <math.h>

bool actuatorsSetup() {
  bool success = true;

  if (!servo1.attach(SERVO_PINS[0])) success = false;
  if (!servo2.attach(SERVO_PINS[1])) success = false;
  if (!servo3.attach(SERVO_PINS[2])) success = false;
  if (!servo4.attach(SERVO_PINS[3])) success = false;
  if (!endEffector.attach(SERVO_PINS[4])) success = false;

  currentAngle1 = 90.0f;
  currentAngle2 = 90.0f;
  currentAngle3 = 90.0f;
  currentAngle4 = 90.0f;

  targetAngle1 = 90.0f;
  targetAngle2 = 90.0f;
  targetAngle3 = 90.0f;
  targetAngle4 = 90.0f;

  angleEndEffector = 90;

  return success;
}

void motorControlTask(void* parameter) {
  while (true) {
    switch (currentState) {
      case TEACH:
        teachState();
        break;
      case OPERATE:
        operationState();
        break;
      case STANDBY:
        standbyState();
        break;
      default:
        standbyState();
        break;
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void servoControlTask(void* parameter) {
  const float step = 1.0f;

  while (true) {
    // Update servo 1
    if (currentAngle1 < targetAngle1) {
      currentAngle1 += step;
      if (currentAngle1 > targetAngle1) currentAngle1 = targetAngle1;
    } else if (currentAngle1 > targetAngle1) {
      currentAngle1 -= step;
      if (currentAngle1 < targetAngle1) currentAngle1 = targetAngle1;
    }
    servo1.write((int)currentAngle1);

    // Update servo 2
    if (currentAngle2 < targetAngle2) {
      currentAngle2 += step;
      if (currentAngle2 > targetAngle2) currentAngle2 = targetAngle2;
    } else if (currentAngle2 > targetAngle2) {
      currentAngle2 -= step;
      if (currentAngle2 < targetAngle2) currentAngle2 = targetAngle2;
    }
    servo2.write((int)currentAngle2);

    // Update servo 3
    if (currentAngle3 < targetAngle3) {
      currentAngle3 += step;
      if (currentAngle3 > targetAngle3) currentAngle3 = targetAngle3;
    } else if (currentAngle3 > targetAngle3) {
      currentAngle3 -= step;
      if (currentAngle3 < targetAngle3) currentAngle3 = targetAngle3;
    }
    servo3.write((int)currentAngle3);

    // Update servo 4
    if (currentAngle4 < targetAngle4) {
      currentAngle4 += step;
      if (currentAngle4 > targetAngle4) currentAngle4 = targetAngle4;
    } else if (currentAngle4 > targetAngle4) {
      currentAngle4 -= step;
      if (currentAngle4 < targetAngle4) currentAngle4 = targetAngle4;
    }
    servo4.write((int)currentAngle4);

    endEffector.write(angleEndEffector);

    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void standbyState() {
  Serial.println("State: Standby - Holding positions");
}

void operationState() {
  Serial.println("State: Operate");
}

void teachState() {
  Serial.println("State: Teach - Moving towards target angles");
}

Position forwardKinematics() {
  Position pos;
  return pos;
}
