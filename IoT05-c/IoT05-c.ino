// choi++20201005 IoT05-Step C
// 1. Prepare 3 LEDs, led1, led2, led3 on GPIO 5, 17, 16
// 2. Connect PIR sensor.. VIN, GND, GPIO 36
// 3. every time when PIR detected, turn on each LED one by one
// 4. each LED kept on for 20sec  

#define timeSeconds 20

// Set GPIOs for LED and PIR Motion Sensor
int led[3] = {5, 17, 16};
const int motionSensor = 36;

// Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger[3] = {0, 0, 0};
boolean startTimer[3] = {false, false, false};

// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement() {
        static int id = 0;
        Serial.println("MOTION (" + String(id) + ") DETECTED!!!");
        digitalWrite(led[id], HIGH);
        startTimer[id] = true;
        lastTrigger[id] = millis();
        id = (++id >= 3) ? 0 : id;
}

void setup() {
// Serial port for debugging purposes
        Serial.begin(115200);
// PIR Motion Sensor mode INPUT_PULLUP
        pinMode(motionSensor, INPUT_PULLUP);
// Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
        attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);
// Set LED to LOW
        for (int i=0; i<3; i++) {
            pinMode(led[i], OUTPUT);
            digitalWrite(led[i], LOW);
        }
}

void loop() {
// Current time
        now = millis();
        // Turn off the LED after the number of seconds defined in the timeSeconds variable
        for (int i=0; i<3; i++) {
            if (startTimer[i] && (now - lastTrigger[i] > (timeSeconds*1000))) {
                    Serial.println("Motion (" + String(i) + ") stopped...");
                    digitalWrite(led[i], LOW);
                    startTimer[i] = false;
            }
        }
}
