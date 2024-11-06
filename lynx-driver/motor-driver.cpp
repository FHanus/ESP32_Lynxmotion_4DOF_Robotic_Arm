#include "motor-driver.h"
#include <math.h>

bool actuatorsSetup() {
  bool success = true;

  if (!servo1.attach(SERVO_PINS[0])) success = false;
  if (!servo2.attach(SERVO_PINS[1])) success = false;
  if (!servo3.attach(SERVO_PINS[2])) success = false;
  if (!servo4.attach(SERVO_PINS[3])) success = false;
  if (!endEffector.attach(SERVO_PINS[4])) success = false;

  // Initialize servo angles
  angle1 = 90;
  angle2 = 90;
  angle3 = 90;
  angle4 = 90;
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
  }
}

void servoControlTask(void* parameter) {
  while (true) {
    // Write servo angles based on global variables
    servo1.write(angle1);
    servo2.write(angle2);
    servo3.write(angle3);
    servo4.write(angle4);
    endEffector.write(angleEndEffector);
    
    vTaskDelay(pdMS_TO_TICKS(5)); // 20ms delay
  }
}

void standbyState(){
  Serial.print("State: Standby - Holding last servo positions");
  forwardKinematics();
  Serial.println(" ");
}

void operationState(){
  Serial.println("State: Operate");
  // Implement operation logic here
}

void teachState(){
  Serial.print("State: Teach");

  float delta = 0.005f; // Movement increment in metres

  // Tentative new positions
  Position testicek = forwardKinematics();

  Serial.print("| Current Pose:");
  Serial.print("  ");
  Serial.print(testicek.x);
  Serial.print("  ");
  Serial.print(testicek.y);
  Serial.print("  ");

  // Handle currentAction
  if (currentAction == "up") {
    Serial.print("-UP-");
    testicek.y += delta;
  } else if (currentAction == "down") {
    Serial.print("-DOWN-");
    testicek.y -= delta;
  } else if (currentAction == "left") {
    Serial.print("-LEFT-");
    //newPosX -= delta;
  } else if (currentAction == "right") {
    Serial.print("-RIGHT-");
    //newPosX += delta;
  } else if (currentAction == "backward") {
    Serial.print("-BACKWARD-");
    testicek.x -= delta;
  } else if (currentAction == "forward") {
    Serial.print("-FORWARD-");
    testicek.x += delta;
  }
  
  Serial.print("| Action:");
  Serial.print(currentAction);
  Serial.print(" | Updated Pose:");
  Serial.print("  ");
  Serial.print(testicek.x);
  Serial.print("  ");
  Serial.print(testicek.y);
  Serial.print("  ");

  AngularPosition moveItOutput;

  if (currentAction != "") {
      moveItOutput = moveIt(testicek);

      angle2 = moveItOutput.angularOutput2;
      angle3 = moveItOutput.angularOutput3;
      angle4 = moveItOutput.angularOutput4;
  }


  currentAction = ""; // Reset currentAction

  Serial.print("| Driving:");
  Serial.print("  ");
  Serial.print(angle2);
  Serial.print("  ");
  Serial.print(angle3);
  Serial.print("  ");
  Serial.print(angle4);
  Serial.print("  ");

  Serial.println(" ");
}

AngularPosition moveIt(Position movePosition){
  // Limit newPosX and newPosY to reachable workspace
  float maxReach = L1 + L2 + L3;
  if (movePosition.x > maxReach) movePosition.x = maxReach;
  if (movePosition.x < -maxReach) movePosition.x = -maxReach;
  if (movePosition.y > maxReach) movePosition.y = maxReach;
  if (movePosition.y < 0) movePosition.y = 0; // Can't go below base

  // Compute joint angles for tentative position
  float theta2, theta3, theta4;
  AngularPosition output;

  if (!inverseKinematics(movePosition, theta2, theta3, theta4)) {
    Serial.println("Target unreachable");
    // Do not update posX and posY
    output.angularOutput2 = angle2;
    output.angularOutput3 = angle3;
    output.angularOutput4 = angle4;
    return output;
  }

  // Convert angles to degrees
  output.angularOutput2 = (int)(theta2 * 180.0f / M_PI);
  output.angularOutput3 = (int)(theta3 * 180.0f / M_PI);
  output.angularOutput4 = (int)(theta4 * 180.0f / M_PI);

  Serial.print("| Angles:");
  Serial.print("  ");
  Serial.print(output.angularOutput2);
  Serial.print("  ");
  Serial.print(output.angularOutput3);
  Serial.print("  ");
  Serial.print(output.angularOutput4);
  Serial.print("  ");

  return output;
}

bool inverseKinematics(Position desiredPosition, float& theta1, float& theta2, float& theta3) {
  // Adjust for L3 (end effector length)
  float Xp = desiredPosition.x - L3;
  float Yp = desiredPosition.y;

  // Compute D
  float D = (Xp * Xp + Yp * Yp - L1 * L1 - L2 * L2) / (2.0f * L1 * L2);

  // Check if target is reachable
  if (fabs(D) > 1.0f) {
    // Target unreachable
    return false;
  }

  // Compute theta2
  theta2 = atan2(-sqrt(1.0f - D * D), D);

  // Compute theta1
  theta1 = atan2(Yp, Xp) - atan2(L2 * sin(theta2), L1 + L2 * cos(theta2));

  // Compute theta3 to keep end effector orientation
  theta3 = -(theta1 + theta2);

  return true;
}

Position forwardKinematics() {
  // Convert angles to radians
  float theta2_rad = angle2 * M_PI / 180.0f;
  float theta3_rad = angle3 * M_PI / 180.0f;
  float theta4_rad = angle4 * M_PI / 180.0f;
  
  // Compute FK
  float x1 = 0;
  float y1 = 0;
  
  float x2 = x1 + L1 * cos(theta2_rad);
  float y2 = y1 + L1 * sin(theta2_rad);
  
  float x3 = x2 + L2 * cos(theta2_rad + theta3_rad);
  float y3 = y2 + L2 * sin(theta2_rad + theta3_rad);
  
  float xPos = x3 + L3 * cos(theta2_rad + theta3_rad + theta4_rad);
  float yPos = y3 + L3 * sin(theta2_rad + theta3_rad + theta4_rad);

  Position pos;
  pos.x = xPos;
  pos.y = yPos;

  Serial.print(" | Angle 2: ");
  Serial.print(angle2);
  Serial.print(" Angle 3: ");
  Serial.print(angle3);
  Serial.print(" Angle 4: ");
  Serial.print(angle4);

  Serial.print(" | EE Pos - X: ");
  Serial.print(pos.x);
  Serial.print(" Y: ");
  Serial.print(pos.y);
  Serial.print("");

  return pos;
}