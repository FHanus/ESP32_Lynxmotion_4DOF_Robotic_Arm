#include "definitions.h"

// Global Variables Initialization
Servo servo1, servo2, servo3, servo4, endEffector;
State currentState = STANDBY;
GripperState currentGripperState = GRIPPER_OPEN;
WebServer server(80);
String currentAction = "";

// Servo angle variables
int angle1 = 90; 
int angle2 = 90; 
int angle3 = 90; 
int angle4 = 90; 
int angleEndEffector = 90; 
