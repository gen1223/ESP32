// Joystick, Cds test
// choi++20200920
// Joystick is connected to GPIO 4/2 (Analog ADC2_CH0/2)
// Cds is connected to GPIO15 (Analog ADC2_CH3)
const int joyPinX = 4;
const int joyPinY = 2;
const int joySW = 22;
const int cdsPin = 15;

// variable for storing the Joystick values
int xValue = 0, yValue = 0, sValue = 0, cValue = 0;

void setup() {
  Serial.begin(115200);
  pinMode(joySW, INPUT);
  delay(1000);
}

void loop() {
// Reading Joystick values
  xValue = analogRead(joyPinX);
  yValue = analogRead(joyPinY);
  cValue = analogRead(cdsPin);
  sValue = digitalRead(joySW);
  String str = "(X,Y) = (" + String(xValue) + ',' + String(yValue) + ")\n";
  Serial.print(str);
  Serial.print("Switch = ");
  Serial.println(sValue?"ON":"OFF");
  Serial.print("Cds Value = ");
  Serial.println(cValue);
  delay(500);
}
