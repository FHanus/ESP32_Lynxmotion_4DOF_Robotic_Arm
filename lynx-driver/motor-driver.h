#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "definitions.h"

// Function Prototypes
bool actuatorsSetup();
void motorControlTask(void* parameter);
void standbyState();
void teachState();
void operationState();

#endif // MOTOR_DRIVER_H