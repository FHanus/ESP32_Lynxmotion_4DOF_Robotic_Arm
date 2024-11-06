#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "definitions.h"

struct Position {
  float x;
  float y;
};

struct AngularPosition {
  int angularOutput2;
  int angularOutput3;
  int angularOutput4;
};

bool actuatorsSetup();
void motorControlTask(void* parameter);
void standbyState();
void teachState();
void operationState();
void servoControlTask(void* parameter);
void setup();
void loop();

bool recordPosition();
void replayPositions();

#endif // MOTOR_DRIVER_H
