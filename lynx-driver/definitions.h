#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Adjust these pins as per your actual wiring:
static const int SERVO_PINS[5] = {2, 3, 4, 5, 1};

enum State { STANDBY, TEACH, OPERATE };
enum GripperState { GRIPPER_OPEN, GRIPPER_CLOSE };

struct PositionRecord {
  float angle1;
  float angle2;
  float angle3;
  float angle4;
  GripperState eeState;
};

extern Servo servo1, servo2, servo3, servo4, endEffector;
extern State currentState;
extern GripperState currentGripperState;
extern WebServer server;

extern float currentAngle1, currentAngle2, currentAngle3, currentAngle4;
extern int angleEndEffector;

extern PositionRecord positionRecords[50];
extern int recordCount;
extern int currentReplayIndex;

bool actuatorsSetup();
bool serverSetup();
void handleRoot();
void handleStateChange();
void handleGripperStateChange();
void handleServoControl();
void handleGetServoAngles();

// Functions for operation mode (replay)
bool recordPosition();
void runReplayCycle();

#endif // DEFINITIONS_H
