#include <Arduino.h>
#include "globals.h"

void setup() 
{
    Serial.begin(115200);
    delay(100);

    if (!SetupActuators()) {
        Serial.println("Failed to initialize actuators.");
    }

    if (!SetupWebServer()) {
        Serial.println("Failed to start server.");
    }
}

void loop() 
{
    // Let the web server handle any incoming client requests
    g_server.handleClient();

    // If the robot is in OPERATE mode, continuously run the replay cycle
    if (g_robotState == RobotState::Operate) {
        RunReplayCycle();
    }

    delay(10);
}
