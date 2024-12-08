#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include "motor-driver.h" 

static const int SERVO_PINS[5] = {2, 3, 4, 5, 1};

static const float L1 = 0.146f;
static const float L2 = 0.223f;
static const float L3 = 0.06f;

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
extern float targetAngle1, targetAngle2, targetAngle3, targetAngle4;
extern int angleEndEffector;

extern PositionRecord positionRecords[10];
extern int recordCount;
extern int currentReplayIndex;

bool actuatorsSetup();
bool serverSetup();
void handleRoot();
void handleStateChange();
void handleGripperStateChange();
void handleServoControl();
void handleGetServoAngles();
void motorControlTask(void* parameter);
void servoControlTask(void* parameter);
void clientHandleTask(void* parameter);

#endif // DEFINITIONS_H
