// set pin numbers
const int buttonPin = 15;  // the number of the pushbutton pin
const int touchPin = 4;
const int ledPin = 16;
// change with your threshold value
const int threshold = 20;
// variable for storing the touch pin value
int touchValue;
// variable for storing the pushbutton status
int buttonState = 0;


void setup() {
  Serial.begin(115200);
  delay(1000); // time to bring up serial monitor
  // initialize the LED pin as an output:
  pinMode (ledPin, OUTPUT);
  pinMode (touchPin, INPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  // read the touch value:
    touchValue = touchRead(touchPin);
  // read the state of the pushbutton
    buttonState = digitalRead(buttonPin);
    Serial.println(buttonState);
    Serial.print(touchValue);

  // check if the touchValue below the threshold
  // if it is, set ledPin to HIGH
  if(touchValue < threshold || buttonState == HIGH) {
    // turn LED on
    digitalWrite(ledPin, HIGH);
    Serial.println(" - LED on");
  }
  else {
    // turn LED off
    digitalWrite(ledPin, LOW);
    Serial.println(" - LED off");
  }
  delay(500);
}
