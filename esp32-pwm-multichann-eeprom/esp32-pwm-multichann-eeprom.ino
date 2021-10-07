/* remember the last state to eeprom 
 *  pause-switch using interrupt
 *  debouncing.. for both times press and release
 *  choi
 */

#include <EEPROM.h>       // lib to read and write from flash memory

// the number of the LED pin
const int ledPin = 16;    // 16 corresponds to GPIO16
const int ledPin2 = 17;  // 17 corresponds to GPIO17
const int ledPin3 = 5;   // 5 corresponds to GPIO5
const int buttonPin = 4;  // the number of the pushbutton pin
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
// initialize EEPROM with predefined size
    EEPROM.begin(8);
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
    int i = 0, j = 0, id = 1, k = 0, jd = 1, kd = 1;
    // check if there is previous state
    if (EEPROM.read(0) == 0xAA & EEPROM.read(1) == 0x55) {
        i = EEPROM.read(2);
        j = EEPROM.read(3);
        k = EEPROM.read(4);
        id = EEPROM.read(5);
        jd = EEPROM.read(6);
        kd = EEPROM.read(7);
    }

    for( ; ; ) {
        // changing the LED brightness with PWM
        ledcWrite(ledChannel, i/4);
        if ((i += id) >= 1024) id = -1, i = 1023;
        else if (i < 0) id = 1, i = 0;
        ledcWrite(ledChannel1, j/2);
        if ((j += jd) >= 512) jd = -1, j = 511;
        else if (j < 0) jd = 1, j = 0;
        ledcWrite(ledChannel2, k);
        if ((k += kd) >= 256) kd = -1, k = 255;
        else if (k < 0) kd = 1, k = 0;
        // check pause-flag
        if (pauseState) {
            // save current state into flash
            Serial.println("Save current state");
            EEPROM.write(0, 0xAA);
            EEPROM.write(1, 0x55);
            EEPROM.write(2, i);
            EEPROM.write(3, j);
            EEPROM.write(4, k);
            EEPROM.write(5, id);
            EEPROM.write(6, jd);
            EEPROM.write(7, kd);
            EEPROM.commit();
            while(pauseState) delay(5); // wait breaking pause
        }
        else delay(5);
    }
}
