#ifndef INDEX_HANDLERS_H
#define INDEX_HANDLERS_H

#include "definitions.h"

bool serverSetup();
void clientHandleTask(void* parameter);
void handleRoot();
void handleStateChange();
void handleGripperStateChange();
void handleServoControl(); 
void handleGetServoAngles();
void handleRecordPosition();

#endif // INDEX_HANDLERS_H
