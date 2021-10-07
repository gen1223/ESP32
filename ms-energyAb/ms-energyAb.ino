
// available pins
// 2, 4, 13, 16, 25, 26, 14, 15, 5

void setup() {
  pinMode(4, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(4, HIGH);
  Serial.println("HIGH");
  delay(2000);
  digitalWrite(4, LOW);
  Serial.println("LOW");
  delay(2000);
}
