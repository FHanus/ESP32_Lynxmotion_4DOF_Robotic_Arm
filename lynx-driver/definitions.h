#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Constants
static const int SERVO_PINS[5] = {5, 18, 19, 21, 22};

// Enumerations
enum State { STANDBY, TEACH, OPERATE };
enum GripperState { GRIPPER_OPEN, GRIPPER_CLOSE };

// Extern Global Variables
extern Servo servo1, servo2, servo3, servo4, endEffector;
extern State currentState;
extern GripperState currentGripperState;
extern WebServer server;
extern String currentAction;

// Function Prototypes
bool actuatorsSetup();
bool serverSetup();
void handleRoot();
void handleStateChange();
void handleGripperStateChange();
void handleControl();
void motorControlTask(void* parameter);
void clientHandleTask(void* parameter);

#endif // DEFINITIONS_H