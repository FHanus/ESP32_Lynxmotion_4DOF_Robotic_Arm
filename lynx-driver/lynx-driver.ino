#include <Arduino.h>
#include "globals.h"

void setup() 
{
    Serial.begin(115200);
    delay(100);

    pinMode(45, OUTPUT);
    pinMode(46, OUTPUT);
    pinMode(47, OUTPUT);
    pinMode(48, OUTPUT);

    digitalWrite(45, HIGH);  // Standby LED ON
    digitalWrite(46, LOW);   // Teach LED OFF
    digitalWrite(47, LOW);   // Operate LED OFF
    digitalWrite(48, LOW);   // Unknown LED OFF

    // Setup servo actuators
    if (!SetupActuators()) {
        Serial.println("Failed to initialize actuators.");
    }

    // Setup the Wi-Fi server
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