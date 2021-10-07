// music play station by using pwm
// choi++20200924
//   freq -> pitch
//   duty -> volume (?)

// setting PWM properties
//const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
const int buzPin = 23;
const int duty = 128;

// variables
int vNote=0, vDur=0;
const int dDur = 250; // default duration

// notes
//enum Notes {C3=0, CS3, D3, DS3, E3, F3};
int nFrq[] = {262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523};
int nDur[] = { 2000, 1500, 1000, 750, 500, 375, 250 };

void playNote(int note, int dur) {
    if (note == -1) {
        ledcSetup(ledChannel, 0, resolution);
        ledcWrite(ledChannel, 0);
    }
    else {
        ledcSetup(ledChannel, nFrq[note], resolution);
        ledcWrite(ledChannel, duty);
    }
    Serial.println(String(note)+","+String(dur));
    delay(nDur[dur]);
}

void setup() {
    Serial.begin(115200);
// attach the channel to the GPIOs
    ledcAttachPin(buzPin, ledChannel);
}

void loop(){
    if (Serial.available() > 0) {
        vNote = Serial.read();
        if (Serial.available() > 0) {
            vDur = Serial.read();
            if (vDur <= '6' && vDur >= '0')
                vDur -= '0';
            else vDur = dDur;
            if (vNote <= '9' && vNote >= '0')
                vNote -= '0';
            else if (vNote <= 'c' && vNote >= 'a')
                vNote = vNote - 'a' + 10;
            else /* if (vNote == ',') */
                vNote = -1;              // rest
            playNote(vNote, vDur);
        }
    }
}
