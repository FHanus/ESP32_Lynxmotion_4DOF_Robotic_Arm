#include "definitions.h"

// Global Variables Initialization
Servo servo1, servo2, servo3, servo4, endEffector;
State currentState = STANDBY;
GripperState currentGripperState = GRIPPER_OPEN;
WebServer server(80);
String currentAction = "";