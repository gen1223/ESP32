/* pause-switch using interrupt
 *  debouncing.. for both times press and release
 *  choi
 */
// the number of the LED pin
const int ledPin = 15; //16;    // 16 corresponds to GPIO16
const int ledPin2 = 13; //17;  // 17 corresponds to GPIO17
const int ledPin3 = 12; //5;   // 5 corresponds to GPIO5
const int buttonPin = 14; //4;  // the number of the pushbutton pin
const int debTime = 500;   // debouncing time

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int ledChannel1 = 1;
const int ledChannel2 = 2;
const int resolution = 8;
int pauseState = 0;
boolean inHandler = false; // semaphore
unsigned long startDeb = millis();  // debouncing time

// interrupt handler
void IRAM_ATTR detectButt() {
    Serial.println("Interrupt occurs!!!");
    // remove glitch
    if (inHandler) return; // if already in handler
    inHandler = true;   // set semaphore
    if (millis() - startDeb > debTime) {  // remove bounce
        startDeb = millis();
        pauseState ^= 1; // toggle flag
        Serial.println("pauseState= " + String(pauseState));
    }
    inHandler = false;
}

void setup() {
    Serial.begin(115200);
// configure LED PWM functionalitites
    ledcSetup(ledChannel, freq, resolution);
// attach the channel to the GPIOs
    ledcAttachPin(ledPin, ledChannel);
    ledcAttachPin(ledPin2, ledChannel1);
    ledcAttachPin(ledPin3, ledChannel2);
// initialize the pushbutton pin as an input
    pinMode(buttonPin, INPUT);
// Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
    attachInterrupt(digitalPinToInterrupt(buttonPin), detectButt, RISING);
}

void loop(){
// increase the LED brightness
    int j = 0, k = 0, jd = 1, kd = 1;
    for(int i = 0; i <= 1023; i++) {
        // changing the LED brightness with PWM
        ledcWrite(ledChannel, i/4);
        ledcWrite(ledChannel1, j/2);
        if ((j += jd) == 512) jd = -1, j = 511;
        ledcWrite(ledChannel2, k);
        if ((k += kd) >= 256) kd = -1, k = 255;
        else if (k < 0) kd = 1, k = 0;
/*    // pause if button pressed
        if (digitalRead(buttonPin)) {
            delay(1000);
            while (!digitalRead(buttonPin));
            delay(1000);
        } */
        // check pause-flag
        if (pauseState)
          while(pauseState) delay(5); // wait breaking pause
        else delay(5);
    }

// decrease the LED brightness
    j = 0, k = 0, jd = 1, kd = 1;
    for(int i = 1023; i >= 0; i--) {
        // changing the LED brightness with PWM
        ledcWrite(ledChannel, i/4);
        ledcWrite(ledChannel1, j/2);
        if ((j += jd) == 512) jd = -1, j = 511;
        ledcWrite(ledChannel2, k);
        if ((k += kd) >= 256) kd = -1, k = 255;
        else if (k < 0) kd = 1, k = 0;

        // check pause-flag
        if (pauseState)
          while(pauseState) delay(5); // wait breaking pause
        else delay(5);
    }
}
