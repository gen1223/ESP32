// make tones by using pwm
// choi++20200919
//   freq -> pitch
//   duty -> volume (?)

// setting PWM properties
//const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
const int buzPin = 23;
const int duty = 128;

// variables
int sVal;

// notes
//enum Notes {C3=0, CS3, D3, DS3, E3, F3};
int nFrq[] = {/*131, 139, 147, 156, 165, 175*/ 
              262, 277, 294, 311, 330, 349};

void playNote(int note, int dur) {
    ledcSetup(ledChannel, nFrq[note], resolution);
    ledcWrite(ledChannel, duty);
    Serial.println(note);
    delay(dur);
}

void setup() {
    Serial.begin(115200);
// configure LED PWM functionalitites
//    ledcSetup(ledChannel, freq, resolution);
// attach the channel to the GPIOs
    ledcAttachPin(buzPin, ledChannel);
}

void loop(){
    if (Serial.available() > 0) {
        sVal = Serial.read();
        playNote(sVal-0x30, 250);
    }
}
