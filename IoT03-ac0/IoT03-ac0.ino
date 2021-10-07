// set pin numbers
const int touchPin = 4;
const int ledPin = 16;
const int buttonPin = 15;

// change with your threshold value
const int threshold = 20;
// variable for storing the touch pin value
int touchValue;
int buttonState = 0;
int ledState = 0;

void setup() {
  Serial.begin(115200);
  delay(1000); // time to bring up serial monitor
  // initialize the LED pin as an output:
  pinMode (ledPin, OUTPUT);
  pinMode (touchPin, INPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  touchValue = touchRead(touchPin);
  Serial.print(touchValue);
  // check if the touchValue below the threshold
  // if it is, set ledPin to HIGH
  if(touchValue < threshold) {
    // turn LED on
    digitalWrite(ledPin, HIGH);
    Serial.println(" - LED on");
    ledState = HIGH;
  }
  else {
    // turn LED off
    digitalWrite(ledPin, LOW);
    Serial.println(" - LED off");
    ledState = LOW;
  }
  // toggle pause state if button pressed
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
      delay(50);     // skip glitch
      do {
          buttonState = digitalRead(buttonPin);
      }
      while (buttonState == HIGH);  // do nothing while pressed
      ledState = ledState ^ 1;      // toggle led state
      digitalWrite(ledPin, ledState);
  }
  delay(100);
}
