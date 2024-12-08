#include "index-handlers.h"
#include "index-html.h"
#include "config.h"
#include "motor-driver.h"

extern float currentAngle1, currentAngle2, currentAngle3, currentAngle4;

String currentStateToString(State state) {
  switch (state) {
    case STANDBY:
      return "standby";
    case TEACH:
      return "teach";
    case OPERATE:
      return "operate";
    default:
      return "unknown";
  }
}

String currentGripperStateToString(GripperState gripperState) {
  switch (gripperState) {
    case GRIPPER_OPEN:
      return "open";
    case GRIPPER_CLOSE:
      return "close";
    default:
      return "unknown";
  }
}

bool serverSetup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to Wi-Fi");

  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
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

  server.on("/", handleRoot);
  server.on("/setState", handleStateChange);
  server.on("/setGripperState", handleGripperStateChange);
  server.on("/servoControl", handleServoControl);
  server.on("/getServoAngles", handleGetServoAngles);
  server.onNotFound(handleRoot);

  server.begin();
  return true;
}

void clientHandleTask(void* parameter) {
  // Not needed in the simplified version.
}

void handleRoot() {
  String html = index_html; 
  html.replace("%CURRENT_STATE%", currentStateToString(currentState));
  html.replace("%CURRENT_GRIPPER_STATE%", currentGripperStateToString(currentGripperState));
  server.send(200, "text/html", html);
}

void handleStateChange() {
  String state = server.arg("state");
  if (state == "standby") {
    currentState = STANDBY;
    recordCount = 0;
    currentReplayIndex = 0;
  } else if (state == "teach") {
    currentState = TEACH;
  } else if (state == "operate") {
    currentState = OPERATE;
  } else {
    server.send(400, "text/plain", "Invalid state.");
    return;
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleGripperStateChange() {
  String gripperState = server.arg("gripper");
  if (gripperState == "open") {
    currentGripperState = GRIPPER_OPEN;
    angleEndEffector = 70;
  } else if (gripperState == "close") {
    currentGripperState = GRIPPER_CLOSE;
    angleEndEffector = 160;
  } else {
    server.send(400, "text/plain", "Invalid gripper state.");
    return;
  }

  endEffector.write(angleEndEffector);

  if (currentState == TEACH) {
    if (recordPosition()) {
      Serial.println("Position recorded due to EE state change.");
    }
  }

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleServoControl() {
  if (!server.hasArg("servo") || !server.hasArg("action")) {
    server.send(400, "text/plain", "Bad Request");
    return;
  }

  int servoId = server.arg("servo").toInt();
  String action = server.arg("action");

  if (servoId < 1 || servoId > 4) {
    server.send(400, "text/plain", "Invalid servo ID.");
    return;
  }

  if (currentState != TEACH) {
    server.send(403, "text/plain", "Servo control only allowed in TEACH mode.");
    return;
  }

  float delta = 5.0f; // Smaller increment for smoother control.

  float* targetAngle = nullptr;
  switch (servoId) {
    case 1: targetAngle = &currentAngle1; break;
    case 2: targetAngle = &currentAngle2; break;
    case 3: targetAngle = &currentAngle3; break;
    case 4: targetAngle = &currentAngle4; break;
  }

  if (action == "increase") {
    *targetAngle += delta;
    if (*targetAngle > 180) *targetAngle = 180;
  } else if (action == "decrease") {
    *targetAngle -= delta;
    if (*targetAngle < 0) *targetAngle = 0;
  } else if (action == "stop") {
    // No action needed for stop here since we apply angles immediately.
  } else {
    server.send(400, "text/plain", "Invalid action.");
    return;
  }

  // Write updated angles immediately:
  servo1.write((int)currentAngle1);
  servo2.write((int)currentAngle2);
  servo3.write((int)currentAngle3);
  servo4.write((int)currentAngle4);

  server.send(200, "text/plain", "OK");
}

void handleGetServoAngles() {
  String json = "{";
  json += "\"angle1\":" + String(currentAngle1) + ",";
  json += "\"angle2\":" + String(currentAngle2) + ",";
  json += "\"angle3\":" + String(currentAngle3) + ",";
  json += "\"angle4\":" + String(currentAngle4);
  json += "}";
  server.send(200, "application/json", json);
}
