#include "definitions.h"

Servo servo1, servo2, servo3, servo4, endEffector;
State currentState = STANDBY;

GripperState currentGripperState = GRIPPER_CLOSE;

int angleEndEffector = 0;

WebServer server(80);

float currentAngle1 = 90.0f; 
float currentAngle2 = 90.0f; 
float currentAngle3 = 90.0f; 
float currentAngle4 = 90.0f; 

float targetAngle1 = 90.0f; 
float targetAngle2 = 90.0f; 
float targetAngle3 = 90.0f; 
float targetAngle4 = 90.0f; 

PositionRecord positionRecords[10];
int recordCount = 0;
int currentReplayIndex = 0;
