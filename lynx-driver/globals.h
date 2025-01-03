#ifndef GLOBALS_H
#define GLOBALS_H

#include <memory>
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

static const int MAX_RECORDS = 50;

static const int SERVO_PINS[5] = {2, 3, 4, 5, 1};

static const char* g_wifiSSID     = "GDIP";
static const char* g_wifiPassword = "group-e-123";

enum class RobotState {
    Standby,
    Teach,
    Operate
};

enum class GripperState {
    Open,
    Close
};

struct PositionRecord {
    float servo1Angle;
    float servo2Angle;
    float servo3Angle;
    float servo4Angle;
    GripperState gripperState;
};

extern RobotState g_robotState;
extern GripperState g_gripperState;

extern int g_endEffectorAngle;
extern float g_servo1Angle;
extern float g_servo2Angle;
extern float g_servo3Angle;
extern float g_servo4Angle;

extern WebServer g_server;

extern PositionRecord g_positionRecords[MAX_RECORDS];
extern int g_recordCount;
extern int g_replayIndex;

extern std::unique_ptr<Servo> g_servo1;
extern std::unique_ptr<Servo> g_servo2;
extern std::unique_ptr<Servo> g_servo3;
extern std::unique_ptr<Servo> g_servo4;

extern std::unique_ptr<Servo> g_servoEndEffector;

/**
 * @brief Setup function for all actuators (servo attachments).
 * @return True if all attached, false otherwise.
 */
bool SetupActuators();

/**
 * @brief Setup the Wi-Fi server and handlers.
 * @return True if Wi-Fi connected and server started, false otherwise.
 */
bool SetupWebServer();

/**
 * @brief Handle the root ("/") endpoint by sending the main interface.
 */
void HandleRoot();

/**
 * @brief Change the robot's operating state: standby, teach, or operate.
 */
void HandleStateChange();

/**
 * @brief Open or close the gripper.
 */
void HandleGripperChange();

/**
 * @brief Increment or decrement angles of a given servo in TEACH mode.
 */
void HandleServoControl();

/**
 * @brief Send the current servo angles in JSON format.
 */
void HandleGetServoAngles();

/**
 * @brief Record the current servo angles into memory.
 */
void HandleRecordPosition();

/**
 * @brief Record the current servo positions (helper function).
 * @return True if successful, false if the max count is reached.
 */
bool RecordCurrentPosition();

/**
 * @brief Replay the stored positions in sequence when in OPERATE mode.
 */
void RunReplayCycle();

#endif // GLOBALS_H
