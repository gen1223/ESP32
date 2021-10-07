
#define CDS 4

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
            delay(2000);
            int vC = analogRead(CDS);
            Serial.println(vC);
}
