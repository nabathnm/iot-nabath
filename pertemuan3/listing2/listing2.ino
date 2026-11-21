#include <ESP32Servo.h>
#define SERVOPIN 18
Servo myServo;

void setup() {
  // put your setup code here, to run once:
  myServo.attach(SERVOPIN);  // Sambungkan servo ke GPIO18
}

void loop() {
  // put your main code here, to run repeatedly:
  myServo.write(0);          // Putar ke sudut 0 derajat
  delay(1000);

  myServo.write(90);         // Putar ke sudut 90 derajat (tengah)
  delay(1000);

  myServo.write(180);        // Putar ke sudut 180 derajat
  delay(1000);
}
