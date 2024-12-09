#include "definitions.h"

Servo servo1, servo2, servo3, servo4, endEffector;
State currentState = STANDBY;
GripperState currentGripperState = GRIPPER_CLOSE;
int angleEndEffector = 0;
WebServer server(80);

float currentAngle1 = 90.0f;
float currentAngle2 = 110.0f;
float currentAngle3 = 135.0f;
float currentAngle4 = 90.0f;

PositionRecord positionRecords[50];
int recordCount = 0;
int currentReplayIndex = 0;
