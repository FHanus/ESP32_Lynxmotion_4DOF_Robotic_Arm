#include "motor-driver.h"

bool actuatorsSetup() {
  bool success = true;

  if (!servo1.attach(SERVO_PINS[0])) success = false;
  if (!servo2.attach(SERVO_PINS[1])) success = false;
  if (!servo3.attach(SERVO_PINS[2])) success = false;
  if (!servo4.attach(SERVO_PINS[3])) success = false;
  if (!endEffector.attach(SERVO_PINS[4])) success = false;
  
  endEffector.write(angleEndEffector);
  return success;
}

bool recordPosition() {
  if (recordCount >= 50) {
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

void runReplayCycle() {
  static bool replaying = false;
  static unsigned long lastStepTime = 0;
  static int stepIndex = -1;

  // If not currently replaying, start now
  if (!replaying) {
    if (recordCount == 0) {
      // No recorded positions, do nothing
      return;
    }
    replaying = true;
    stepIndex = 0;
    lastStepTime = millis();
  }

  // Replay logic: move servos directly to recorded positions with a delay
  if (replaying && (millis() - lastStepTime > 2000)) {
    if (stepIndex < recordCount) {
      PositionRecord rec = positionRecords[stepIndex];
      currentAngle1 = rec.angle1;
      currentAngle2 = rec.angle2;
      currentAngle3 = rec.angle3;
      currentAngle4 = rec.angle4;
      angleEndEffector = (rec.eeState == GRIPPER_OPEN) ? 70 : 160;

      // Write angles directly
      servo1.write((int)currentAngle1);
      servo2.write((int)currentAngle2);
      servo3.write((int)currentAngle3);
      servo4.write((int)currentAngle4);
      endEffector.write(angleEndEffector);

      Serial.printf("Replaying Position %d\n", stepIndex + 1);
      stepIndex++;
      lastStepTime = millis();
    } else {
      Serial.println("Completed replaying positions.");
      replaying = false;
    }
  }
}
