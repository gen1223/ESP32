
// Joystick is connected to GPIO 34/35 (Analog ADC1_CH6/7)
const int joyPinX = 34;
const int joyPinY = 35;

// variable for storing the Joystick values
int xValue = 0, yValue = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
}
void loop() {
// Reading Joystick values
  xValue = analogRead(joyPinX);
  yValue = analogRead(joyPinY);
  String str = "(X,Y) = (" + String(xValue) + ',' + String(yValue) + ")\n";
  Serial.print(str);
  delay(500);
}
