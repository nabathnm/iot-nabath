#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SERVOPIN 17
#define POTPIN 34
#define LEDREDPIN 19
#define LEDGREENPIN 18

Servo myServo;

LiquidCrystal_I2C lcd (0x27 ,16 ,2) ;

void setup() {
  Serial.begin(115200) ;
  myServo.attach(SERVOPIN);
  pinMode(LEDREDPIN, OUTPUT);
  pinMode(LEDGREENPIN, OUTPUT);
  lcd.init();
  lcd.backlight();

  lcd.setCursor (0 , 0) ;
  lcd.print("IoT Kelas E");
}

void loop() {
  int nilaiADC = analogRead(POTPIN);
  int nilaiPWM = map(nilaiADC, 0 ,4095 ,0 , 255);

  lcd.setCursor(0, 1) ;
  lcd.print(nilaiADC);

  if(POTPIN < 500) {
  analogWrite(LEDREDPIN, 4095);
  analogWrite(LEDGREENPIN, 4095);
  myServo.write(0);

  } else if(POTPIN <= 2000) {
  analogWrite(LEDREDPIN, 0);
  analogWrite(LEDGREENPIN, 4095);
  myServo.write(90);

  } else if(POTPIN > 2000) {
  analogWrite(LEDREDPIN, 2048);
  analogWrite(LEDGREENPIN, 2048);
  myServo.write(180);

  }

}
