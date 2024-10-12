#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include "index_html.h"

Servo servo1, servo2, servo3, servo4, endEffector;
static const int servoPins[5] = {5, 18, 19, 21, 22};

enum State { STANDBY, TEACH, OPERATE };
State currentState = STANDBY;

const char *ssid = "VM1410617";
const char *password = "p8xcLsyjnb6j";

WebServer server(80);

void handleRoot();
void handleStateChange();
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
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}