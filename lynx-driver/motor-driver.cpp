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

  angleEndEffector = 0;
  endEffector.write(angleEndEffector);

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
    if (currentAngle1 < targetAngle1) {
      currentAngle1 += step;
      if (currentAngle1 > targetAngle1) currentAngle1 = targetAngle1;
    } else if (currentAngle1 > targetAngle1) {
      currentAngle1 -= step;
      if (currentAngle1 < targetAngle1) currentAngle1 = targetAngle1;
    }
    servo1.write((int)currentAngle1);

    if (currentAngle2 < targetAngle2) {
      currentAngle2 += step;
      if (currentAngle2 > targetAngle2) currentAngle2 = targetAngle2;
    } else if (currentAngle2 > targetAngle2) {
      currentAngle2 -= step;
      if (currentAngle2 < targetAngle2) currentAngle2 = targetAngle2;
    }
    servo2.write((int)currentAngle2);

    if (currentAngle3 < targetAngle3) {
      currentAngle3 += step;
      if (currentAngle3 > targetAngle3) currentAngle3 = targetAngle3;
    } else if (currentAngle3 > targetAngle3) {
      currentAngle3 -= step;
      if (currentAngle3 < targetAngle3) currentAngle3 = targetAngle3;
    }
    servo3.write((int)currentAngle3);

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
  
  recordCount = 0;
  currentReplayIndex = 0;
  targetAngle1 = currentAngle1;
  targetAngle2 = currentAngle2;
  targetAngle3 = currentAngle3;
  targetAngle4 = currentAngle4;
  
  Serial.println("All recorded positions have been cleared.");
}

void operationState() {
  Serial.println("State: Operate - Replaying positions");
  replayPositions();
}

void teachState() {
  Serial.println("State: Teach - Monitoring EE state changes");
}

bool recordPosition() {
  if (recordCount >= 10) {
    Serial.println("Position record limit reached.");
    return false;
  }

  PositionRecord newRecord;
  newRecord.angle1 = currentAngle1;
  newRecord.angle2 = currentAngle2;
  newRecord.angle3 = currentAngle3;
  newRecord.angle4 = currentAngle4;
  newRecord.eeState = currentGripperState;

  positionRecords[recordCount] = newRecord;
  recordCount++;

  Serial.print("Recorded Position ");
  Serial.println(recordCount);

  return true;
}

void replayPositions() {
  if (recordCount == 0) {
    Serial.println("No positions to replay.");
    return;
  }

  for (currentReplayIndex = 0; currentReplayIndex < recordCount; currentReplayIndex++) {
    PositionRecord rec = positionRecords[currentReplayIndex];

    targetAngle1 = rec.angle1;
    targetAngle2 = rec.angle2;
    targetAngle3 = rec.angle3;
    targetAngle4 = rec.angle4;

    Serial.print("Replaying Position ");
    Serial.println(currentReplayIndex + 1);

    bool reached = false;
    while (!reached) {
      reached = true;
      if (abs(currentAngle1 - targetAngle1) > 0.5f) reached = false;
      if (abs(currentAngle2 - targetAngle2) > 0.5f) reached = false;
      if (abs(currentAngle3 - targetAngle3) > 0.5f) reached = false;
      if (abs(currentAngle4 - targetAngle4) > 0.5f) reached = false;
      vTaskDelay(pdMS_TO_TICKS(50));
    }
    angleEndEffector = (rec.eeState == GRIPPER_OPEN) ? 180 : 0;

    vTaskDelay(pdMS_TO_TICKS(500));
  }

  Serial.println("Completed replaying positions.");
}