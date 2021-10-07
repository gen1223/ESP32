void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(16, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(16, HIGH);
  Serial.println(digitalRead(16));
  delay(2000);
  digitalWrite(16, LOW);
  Serial.println(digitalRead(16));
  delay(2000);
}
