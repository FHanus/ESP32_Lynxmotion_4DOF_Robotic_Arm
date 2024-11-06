#ifndef INDEX_HANDLERS_H
#define INDEX_HANDLERS_H

#include "definitions.h"

// Function Prototypes
bool serverSetup();
void clientHandleTask(void* parameter);
void handleRoot();
void handleStateChange();
void handleGripperStateChange();
void handleControl();
void handleGetJointAngles();

#endif // INDEX_HANDLERS_H
