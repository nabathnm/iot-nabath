void setup() {
  // put your setup code here, to run once:
  ledcSetup(0, 5000, 8);   // Channel 0, frekuensi 5000 Hz, resolusi 8-bit
  ledcAttachPin(18, 0);    // Sambungkan GPIO18 ke channel 0
}

void loop() {
  // put your main code here, to run repeatedly:
  ledcWrite(0, 128);       // Duty cycle 50% (128 dari 255)
  ledcWrite(0, 0);         // Mati (0%)
  ledcWrite(0, 255);       // Penuh (100%)
}

