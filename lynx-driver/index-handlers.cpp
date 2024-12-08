#include "index-handlers.h"
#include "index-html.h"
#include "config.h"
#include "motor-driver.h"

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
  while (true) {
    server.handleClient();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
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
    angleEndEffector = 180;
  } else if (gripperState == "close") {
    currentGripperState = GRIPPER_CLOSE;
    angleEndEffector = 0;
  } else {
    server.send(400, "text/plain", "Invalid gripper state.");
    return;
  }

  if (currentState == TEACH) {
    if (recordPosition()) {
      Serial.println("Position recorded due to EE state change.");
    }
  }

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleServoControl() {
  if (server.hasArg("servo") && server.hasArg("action")) {
    int servoId = server.arg("servo").toInt();
    String action = server.arg("action");
    float delta = 10.0f; 

    if (servoId < 1 || servoId > 4) {
      server.send(400, "text/plain", "Invalid servo ID.");
      return;
    }

    if (currentState != TEACH) {
      server.send(403, "text/plain", "Servo control only allowed in TEACH mode.");
      return;
    }

    if (action == "increase") {
      switch (servoId) {
        case 1:
          targetAngle1 += delta;
          if (targetAngle1 > 180) targetAngle1 = 180;
          break;
        case 2:
          targetAngle2 += delta;
          if (targetAngle2 > 180) targetAngle2 = 180;
          break;
        case 3:
          targetAngle3 += delta;
          if (targetAngle3 > 180) targetAngle3 = 180;
          break;
        case 4:
          targetAngle4 += delta;
          if (targetAngle4 > 180) targetAngle4 = 180;
          break;
      }
    } else if (action == "decrease") {
      switch (servoId) {
        case 1:
          targetAngle1 -= delta;
          if (targetAngle1 < 0) targetAngle1 = 0;
          break;
        case 2:
          targetAngle2 -= delta;
          if (targetAngle2 < 0) targetAngle2 = 0;
          break;
        case 3:
          targetAngle3 -= delta;
          if (targetAngle3 < 0) targetAngle3 = 0;
          break;
        case 4:
          targetAngle4 -= delta;
          if (targetAngle4 < 0) targetAngle4 = 0;
          break;
      }
    } else {
      server.send(400, "text/plain", "Invalid action.");
      return;
    }

    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
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
