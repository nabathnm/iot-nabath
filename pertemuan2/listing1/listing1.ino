void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(15, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  int status = digitalRead(15);
  if (status == LOW) {
  digitalWrite(2, HIGH);
  } else {
  digitalWrite(2, LOW);
  }
}
