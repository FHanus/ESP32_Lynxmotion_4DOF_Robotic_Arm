#include "globals.h"
#include "index_html.h"

RobotState g_robotState = RobotState::Standby;
GripperState g_gripperState = GripperState::Close;

int g_endEffectorAngle = 0;

float g_servo1Angle = 90.0f;
float g_servo2Angle = 110.0f;
float g_servo3Angle = 135.0f;
float g_servo4Angle = 90.0f;

WebServer g_server(80);

PositionRecord g_positionRecords[MAX_RECORDS];
int g_recordCount = 0;
int g_replayIndex = 0;

/// Using unique_ptr for each servo.
std::unique_ptr<Servo> g_servo1 = std::make_unique<Servo>();
std::unique_ptr<Servo> g_servo2 = std::make_unique<Servo>();
std::unique_ptr<Servo> g_servo3 = std::make_unique<Servo>();
std::unique_ptr<Servo> g_servo4 = std::make_unique<Servo>();
std::unique_ptr<Servo> g_servoEndEffector = std::make_unique<Servo>();

/**
 * @brief Convert the robot's current state to a human-readable string.
 * @param state RobotState to convert.
 * @return Corresponding string ("standby", "teach", "operate", or "unknown").
 */
static String RobotStateToString(RobotState state)
{
    switch (state) {
        case RobotState::Standby: return "standby";
        case RobotState::Teach:   return "teach";
        case RobotState::Operate: return "operate";
        default:                  return "unknown";
    }
}

/**
 * @brief Convert the gripper state to a human-readable string.
 * @param state GripperState to convert.
 * @return Corresponding string ("open", "close", or "unknown").
 */
static String GripperStateToString(GripperState state)
{
    switch (state) {
        case GripperState::Open:  return "open";
        case GripperState::Close: return "close";
        default:                  return "unknown";
    }
}

// -----------------------------------------------------------------------------
// Actuator Setup
// -----------------------------------------------------------------------------

bool SetupActuators()
{
    bool success = true;

    // Attach all servo objects to their respective pins.
    if (!g_servo1->attach(SERVO_PINS[0])) success = false;
    if (!g_servo2->attach(SERVO_PINS[1])) success = false;
    if (!g_servo3->attach(SERVO_PINS[2])) success = false;
    if (!g_servo4->attach(SERVO_PINS[3])) success = false;
    if (!g_servoEndEffector->attach(SERVO_PINS[4])) success = false;

    // Initialize end effector angle on startup.
    g_servoEndEffector->write(g_endEffectorAngle);

    return success;
}

// -----------------------------------------------------------------------------
// Web Server Setup and Handlers
// -----------------------------------------------------------------------------

bool SetupWebServer()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(g_wifiSSID, g_wifiPassword);

    Serial.print("Connecting to Wi-Fi");
    unsigned long startAttemptTime = millis();

    while ((WiFi.status() != WL_CONNECTED) && (millis() - startAttemptTime < 10000)) {
        Serial.print(".");
        delay(500);
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nFailed to connect to Wi-Fi.");
        return false;
    }

    Serial.println("\nConnected to Wi-Fi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Define all HTTP route handlers
    g_server.on("/", HandleRoot);
    g_server.on("/setState", HandleStateChange);
    g_server.on("/setGripperState", HandleGripperChange);
    g_server.on("/servoControl", HandleServoControl);
    g_server.on("/getServoAngles", HandleGetServoAngles);
    g_server.on("/recordPosition", HandleRecordPosition);

    // Fallback
    g_server.onNotFound(HandleRoot);

    // Start the server
    g_server.begin();
    return true;
}

void HandleRoot()
{
    String html = INDEX_HTML;
    // Replace placeholders with actual states
    html.replace("%CURRENT_STATE%", RobotStateToString(g_robotState));
    html.replace("%CURRENT_GRIPPER_STATE%", GripperStateToString(g_gripperState));

    g_server.send(200, "text/html", html);
}

void HandleStateChange()
{
    String stateArg = g_server.arg("state");

    digitalWrite(45, LOW);
    digitalWrite(46, LOW);
    digitalWrite(47, LOW);
    digitalWrite(48, LOW);

    if (stateArg == "standby") {
        g_robotState = RobotState::Standby;
        g_recordCount = 0;
        g_replayIndex = 0;
        digitalWrite(45, HIGH);  // Standby LED
    } 
    else if (stateArg == "teach") {
        g_robotState = RobotState::Teach;
        digitalWrite(46, HIGH);  // Teach LED
    } 
    else if (stateArg == "operate") {
        g_robotState = RobotState::Operate;
        digitalWrite(47, HIGH);  // Operate LED
    } 
    else {
        // Invalid state => turn on pin 48 for "unknown" or "error" LED
        g_server.send(400, "text/plain", "Invalid state.");
        digitalWrite(48, HIGH);
        return;
    }

    // Redirect back to the home page
    g_server.sendHeader("Location", "/");
    g_server.send(303);
}

void HandleGripperChange()
{
    String gripperArg = g_server.arg("gripper");

    if (gripperArg == "open") {
        g_gripperState = GripperState::Open;
        g_endEffectorAngle = 40;
    } else if (gripperArg == "close") {
        g_gripperState = GripperState::Close;
        g_endEffectorAngle = 160;
    } else {
        g_server.send(400, "text/plain", "Invalid gripper state.");
        return;
    }

    // Write the updated angle to the end effector servo
    g_servoEndEffector->write(g_endEffectorAngle);

    // Redirect back to home
    g_server.sendHeader("Location", "/");
    g_server.send(303);
}

void HandleServoControl()
{
    if (!g_server.hasArg("servo") || !g_server.hasArg("action")) {
        g_server.send(400, "text/plain", "Missing arguments.");
        return;
    }

    int servoId = g_server.arg("servo").toInt();
    String action = g_server.arg("action");

    // Validate servo ID (assuming only 1..4 for TEACH mode)
    if (servoId < 1 || servoId > 4) {
        g_server.send(400, "text/plain", "Invalid servo ID.");
        return;
    }

    // Only allow servo movement in TEACH mode
    if (g_robotState != RobotState::Teach) {
        g_server.send(403, "text/plain", "Servo control only allowed in TEACH mode.");
        return;
    }

    float delta = 5.0f; // Step size for each press
    float* targetAngle = nullptr;

    switch (servoId) {
        case 1: targetAngle = &g_servo1Angle; break;
        case 2: targetAngle = &g_servo2Angle; break;
        case 3: targetAngle = &g_servo3Angle; break;
        case 4: targetAngle = &g_servo4Angle; break;
        default: break;
    }

    if (action == "increase") {
        *targetAngle += delta;
        if (*targetAngle > 180.0f) *targetAngle = 180.0f;
    } else if (action == "decrease") {
        *targetAngle -= delta;
        if (*targetAngle < 0.0f) *targetAngle = 0.0f;
    } else if (action == "stop") {
        // No immediate action required
    } else {
        g_server.send(400, "text/plain", "Invalid action.");
        return;
    }

    // Write updated angles immediately
    g_servo1->write(static_cast<int>(g_servo1Angle));
    g_servo2->write(static_cast<int>(g_servo2Angle));
    g_servo3->write(static_cast<int>(g_servo3Angle));
    g_servo4->write(static_cast<int>(g_servo4Angle));

    g_server.send(200, "text/plain", "OK");
}

void HandleGetServoAngles()
{
    String json = "{";
    json += "\"servo1Angle\":" + String(g_servo1Angle) + ",";
    json += "\"servo2Angle\":" + String(g_servo2Angle) + ",";
    json += "\"servo3Angle\":" + String(g_servo3Angle) + ",";
    json += "\"servo4Angle\":" + String(g_servo4Angle);
    json += "}";

    g_server.send(200, "application/json", json);
}

void HandleRecordPosition()
{
    if (g_robotState != RobotState::Teach) {
        g_server.sendHeader("Location", "/");
        g_server.send(303);
        return;
    }

    if (!RecordCurrentPosition()) {
        g_server.sendHeader("Location", "/");
        g_server.send(303);
        return;
    }

    g_server.sendHeader("Location", "/");
    g_server.send(303);

    Serial.println("Waypoint recorded by user request.");
}

// -----------------------------------------------------------------------------
// Recording and Replaying Logic
// -----------------------------------------------------------------------------

bool RecordCurrentPosition()
{
    if (g_recordCount >= MAX_RECORDS) {
        Serial.println("Position record limit reached.");
        return false;
    }

    PositionRecord record;
    record.servo1Angle   = g_servo1Angle;
    record.servo2Angle   = g_servo2Angle;
    record.servo3Angle   = g_servo3Angle;
    record.servo4Angle   = g_servo4Angle;
    record.gripperState  = g_gripperState;

    g_positionRecords[g_recordCount] = record;
    g_recordCount++;

    Serial.print("Recorded position #");
    Serial.println(g_recordCount);

    return true;
}

void RunReplayCycle()
{
    static bool replaying = false;
    static unsigned long lastStepTime = 0;
    static int stepIndex = -1;

    if (!replaying) {
        if (g_recordCount == 0) {
            return;
        }
        replaying = true;
        stepIndex = 0;
        lastStepTime = millis();
    }

    if (replaying && (millis() - lastStepTime > 2000)) {
        if (stepIndex < g_recordCount) {
            PositionRecord rec = g_positionRecords[stepIndex];
            g_servo1Angle = rec.servo1Angle;
            g_servo2Angle = rec.servo2Angle;
            g_servo3Angle = rec.servo3Angle;
            g_servo4Angle = rec.servo4Angle;

            g_endEffectorAngle = (rec.gripperState == GripperState::Open) ? 40 : 160;

            g_servo1->write(static_cast<int>(g_servo1Angle));
            g_servo2->write(static_cast<int>(g_servo2Angle));
            g_servo3->write(static_cast<int>(g_servo3Angle));
            g_servo4->write(static_cast<int>(g_servo4Angle));
            g_servoEndEffector->write(g_endEffectorAngle);

            Serial.printf("Replaying position #%d\n", stepIndex + 1);

            stepIndex++;
            lastStepTime = millis();
        } else {
            Serial.println("Completed replaying all positions.");
            replaying = false;
        }
    }
}
