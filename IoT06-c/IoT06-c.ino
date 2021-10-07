// Upgrade Play Station (IoT04-c) with Music storage
// choi++20201008
// 1. write music into eeprom from serial monitor
//    format: "<ndndnd...>" .. save notes into eeprom
//       "<": start to save notes and durations, ">": end of notes
//       "nd": note+duration
//    format: "ndndnd.." .. just play notes without saving
// 2. play music from eeprom whenever power-off and power-on
//    eeprom(0),(1): 0xAA, 0x55.. validity signature
//    eeprom(2): number of note-duration pairs
//    eeprom(3)~: note-duration pairs

#include <EEPROM.h>       // lib to read and write from flash memory
#define EE_SIZE 100       // eeprom size in byte
// eeprom index
int eei = 0;              // index for 1st note
boolean fSave = false;    // = true when saving

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
int nFrq[] = {/*131, 139, 147, 156, 165, 175*/ 
              262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523};
int nDur[] = { 2000, 1500, 1000, 750, 500, 375, 250 };

void playNote(int note, int dur) {
    if (note == -1) {   // rest
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
// initialize EEPROM with predefined size
    EEPROM.begin(EE_SIZE);
// if there are saved notes, play them
    if (EEPROM.read(0) == 0xAA && EEPROM.read(1) == 0x55) // if valid
        playNoteE();
}

void loop(){
    if (Serial.available() > 0) {
        vNote = Serial.read();
        if (vNote == '<') {             // start to save
            fSave = true;
            EEPROM.write(0, 0xAA);
            EEPROM.write(1, 0x55);
            eei = 0;
            if (Serial.available() > 0)
                vNote = Serial.read();
        }
        if (vNote == '>') {             // stop to save
            fSave = false;
            EEPROM.write(2, eei);       // number of notes
            EEPROM.commit();
            playNoteE();                // play notes from eeprom
            if (Serial.available() > 0)
                vNote = Serial.read();
        }
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
            // while saving notes, donot play notes
            if (fSave) {
                EEPROM.write(3+(eei<<1), vNote);    // index = eei*2
                EEPROM.write(4+(eei<<1), vDur);
                eei++;
            }
            else
                playNote(vNote, vDur);
        }
    }
}

// play notes from eeprom
void playNoteE() {
    // check the number of notes in eeprom
    int n = EEPROM.read(2);
    for (int i=0; i<n; i++)
          playNote(EEPROM.read(3+(i<<1)), EEPROM.read(4+(i<<1)));
}
