#include "index-handlers.h"
#include "index-html.h"
#include "config.h" 

/**
 * @brief Sets up the Wi-Fi connection and web server.
 * @return true if successful, false otherwise.
 */
bool serverSetup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to Wi-Fi");

  unsigned long startAttemptTime = millis();

  // Attempt to connect for 10 seconds
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

  // Set up server routes
  server.on("/", handleRoot);
  server.on("/setState", handleStateChange);
  server.on("/setGripperState", handleGripperStateChange);
  server.on("/control", handleControl);
  server.on("/getJointAngles", handleGetJointAngles);

  server.begin();
  return true;
}

/**
 * @brief Task for handling incoming client requests.
 * @param parameter Task input parameter (unused).
 */
void clientHandleTask(void* parameter){
  while (true) {
    server.handleClient();  
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

/**
 * @brief Handles the root URL "/".
 */
void handleRoot() {
  server.send(200, "text/html", index_html);
}

/**
 * @brief Handles state change requests.
 */
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

/**
 * @brief Handles gripper state change requests.
 */
void handleGripperStateChange() {
  String gripperState = server.arg("gripper");
  if (gripperState == "open") {
    currentGripperState = GRIPPER_OPEN;
    // Set end effector to open position
    angleEndEffector = 180; // Example value, adjust as needed
  } else if (gripperState == "close") {
    currentGripperState = GRIPPER_CLOSE;
    // Set end effector to closed position
    angleEndEffector = 0; // Example value, adjust as needed
  } else {
    server.send(400, "text/plain", "Invalid gripper state.");
    return;
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

/**
 * @brief Handles control actions from the client.
 */
void handleControl() {
  if (server.hasArg("action")) {
    String action = server.arg("action");
    if (action == "stop") {
      currentAction = "";
    } else {
      currentAction = action;
    }
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

/**
 * @brief Handles requests to get joint angles.
 */
void handleGetJointAngles() {
  String json = "{";
  json += "\"theta1\":" + String(angle1) + ",";
  json += "\"theta2\":" + String(angle2) + ",";
  json += "\"theta3\":" + String(angle3) + ",";
  json += "\"theta4\":" + String(angle4) + ",";
  json += "\"thetaEndEffector\":" + String(angleEndEffector);
  json += "}";
  server.send(200, "application/json", json);
}
