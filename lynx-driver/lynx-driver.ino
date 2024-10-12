#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include "index_html.h"

Servo servo1, servo2, servo3, servo4, endEffector;
static const int servoPins[5] = {5, 18, 19, 21, 22};

enum State { STANDBY, TEACH, OPERATE };
State currentState = STANDBY;

enum GripperState { GRIPPER_OPEN, GRIPPER_CLOSE };
GripperState currentGripperState = GRIPPER_OPEN;

const char *ssid = "VM1410617";
const char *password = "p8xcLsyjnb6j";

WebServer server(80);

String currentAction = "";

void handleRoot();
void handleStateChange();
void handleGripperStateChange(); 
void handleControl();            
void motorControlTask(void *parameter);

void setup() {
  Serial.begin(115200);

  servo1.attach(servoPins[0]);
  servo2.attach(servoPins[1]);
  servo3.attach(servoPins[2]);
  servo4.attach(servoPins[3]);
  endEffector.attach(servoPins[4]);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/setState", handleStateChange);
  server.on("/setGripperState", handleGripperStateChange);
  server.on("/control", handleControl);                   

  server.begin();

  xTaskCreatePinnedToCore(motorControlTask, "MotorControl", 10000, NULL, 1, NULL, 0);
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleStateChange() {
  String state = server.arg("state");
  if (state == "standby") {
    currentState = STANDBY;
  } else if (state == "teach") {
    currentState = TEACH;
  } else if (state == "operate") {
    currentState = OPERATE;
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleGripperStateChange() {
  String gripperState = server.arg("gripper");
  if (gripperState == "open") {
    currentGripperState = GRIPPER_OPEN;
  } else if (gripperState == "close") {
    currentGripperState = GRIPPER_CLOSE;
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

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

void motorControlTask(void *parameter) {
  while (true) {
    switch (currentState) {
      case STANDBY:
        Serial.println("State: Standby");
        break;
      case TEACH:
        Serial.println("State: Teach");
        break;
      case OPERATE:
        Serial.println("State: Operate");
        break;
    }

    switch (currentGripperState) {
      case GRIPPER_OPEN:
        Serial.println("Gripper State: Open");
        break;
      case GRIPPER_CLOSE:
        Serial.println("Gripper State: Close");
        break;
    }

    if (currentAction != "") {
      Serial.println("Action: " + currentAction);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}