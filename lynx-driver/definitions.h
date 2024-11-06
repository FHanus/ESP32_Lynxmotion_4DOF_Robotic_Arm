#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Constants
static const int SERVO_PINS[5] = {40, 41, 38, 39, 42};

// Link lengths in metres
static const float L1 = 0.146f;
static const float L2 = 0.223f;
static const float L3 = 0.06f;

// Enumerations
enum State { STANDBY, TEACH, OPERATE };
enum GripperState { GRIPPER_OPEN, GRIPPER_CLOSE };

// Extern Global Variables
extern Servo servo1, servo2, servo3, servo4, endEffector;
extern State currentState;
extern GripperState currentGripperState;
extern WebServer server;

// Servo angle variables
extern float currentAngle1, currentAngle2, currentAngle3, currentAngle4;
extern float targetAngle1, targetAngle2, targetAngle3, targetAngle4;
extern int angleEndEffector;

// Function Prototypes
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
void forwardKinematics(float& x0, float& y0, float& x1, float& y1, float& x2, float& y2, float& x3, float& y3);

#endif // DEFINITIONS_H
