/* Blink */
// ledPin refers to ESP32 GPIO 23
// step-a 
const int ledPin1 = 23;
const int ledPin2 = 22;
// the setup function runs once when you press reset or power the board
void setup() {
// initialize digital pin ledPin as an output.
    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);
}
// the loop function runs over and over again forever
void loop() {
    digitalWrite(ledPin1, HIGH); // turn the LED on (HIGH is the voltage level)
    digitalWrite(ledPin2, LOW); // turn the LED on (HIGH is the voltage level)
    delay(1000); // wait for a second
    digitalWrite(ledPin1, LOW); // turn the LED off by making the voltage LOW
    digitalWrite(ledPin2, HIGH); // turn the LED off by making the voltage LOW
    delay(1000); // wait for a second
}
