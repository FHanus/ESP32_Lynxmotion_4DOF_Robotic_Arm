#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "definitions.h"

bool actuatorsSetup();

// For OPERATION mode:
bool recordPosition();
void runReplayCycle();

#endif // MOTOR_DRIVER_H
