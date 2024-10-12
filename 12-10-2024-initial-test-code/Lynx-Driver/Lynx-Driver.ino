#include <ESP32Servo.h>

Servo MyServo;

static const int servoPin = 5;           

void setup() {
  Serial.begin(115200);           
  MyServo.attach(servoPin);
}

void loop(){
  // Min position
  MyServo.write(0);  
  Serial.print("Servo Motor Position = ");
  Serial.println(0);          

  Serial.println("Waiting");
  delay(2500);

  // Centre position
  MyServo.write(90); 
  Serial.print("Servo Motor Position = ");
  Serial.println(90);  

  Serial.println("Waiting");
  delay(2500);
  
  // Max position
  MyServo.write(180); 
  Serial.print("Servo Motor Position = ");
  Serial.println(180);         

  Serial.println("Waiting");
  delay(2500);

  // Servo should not move beyond the max position
  MyServo.write(360);
  Serial.print("Servo Motor Position = ");
  Serial.println(360);          

  Serial.println("Waiting");
  delay(2500);
}
