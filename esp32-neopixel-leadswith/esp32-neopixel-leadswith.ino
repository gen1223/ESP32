
/* J-Project
 *  2019.12.06 cb.choi
 *  - control Neopixel by reed switch
 */
 
#include <Adafruit_NeoPixel.h>

// pin configurations
#define NEOPIN  25 //26          // Neoppixel pin
#define NEONUM  24 //60          // number of Neopixels
#define LSWPIN  4  //13           // reed switch pin

// LED states
#define L_OFF   1           // led off
#define L_GON   2           // led going on
#define L_ON    3           // led off
#define L_GOFF  4           // led going off

// values
#define SWINTV  2000        // lead sw interval time
#define LKEEP   5000        // time to keep led on
#define LCGAPI   40         // led brightness change time gap 
#define LCGAPD   40         // led brightness change time gap 
#define RMAX    255         // red max
#define GMAX    255         // green max
#define BMAX    150         // blue max

// variables
boolean cSWstate = false;   // current reed switch state
int cLEDstate = L_OFF;      // current led state
unsigned long cTime;        // current time
int br = 0;                 // brightness
boolean ss;                 // switch status.. true == ON
int rCount = 0;             // for special effect

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEONUM, NEOPIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();            // initialize neopixels
// inital status for neopixel is unstable.. OK anyway if set OFF, after set ON 
  for (int i=0; i<NEONUM; i++)
      strip.setPixelColor(i, strip.Color(1,1,1));
  strip.show();
  delay(10);
  for (int i=0; i<NEONUM; i++)
      strip.setPixelColor(i, strip.Color(0,0,0));
  strip.show();
  delay(10);
  pinMode(LSWPIN, INPUT);
  Serial.begin(115200);
}

void loop() {
  
  ss = digitalRead(LSWPIN) ? false : true;
//  Serial.println("SW: "+String(ss));

  switch (cLEDstate) {
    
    case L_OFF:
        if (ss) {                   // if sw ON
            if (!cSWstate) {        // if not yet sw ON
                cTime = millis();   // start counting timer
                cSWstate = true;
                Serial.println("start timer");
            }
            else {                  // in counting timer
                if (millis()-cTime > SWINTV) {  // if delay expired
                    cLEDstate = L_GON;
                    br = 0;         // start from brightness 0
                    Serial.println("going to L_GON");
                }
            }
        }
        else                        // if sw OFF
            cSWstate = false;
        break;
    
    case L_GON:
        if (ss && cSWstate) {       // if sw ON
            for (int i=0; i<NEONUM ; i++)
              strip.setPixelColor(i, strip.Color(sstart(br), sstart(br), sstart(br) /*bb(br)*/ /*(BMAX*br)/RMAX)*/));
            strip.show();
            if (++br>RMAX) br = RMAX;
            delay(LCGAPI);
          //  Serial.print(".");
        }
        if (!ss) {                  // if sw OFF
            cSWstate = false;
            cTime = millis();
            cLEDstate = L_ON;
            rCount = 0;
            Serial.println("going to L_ON");
        }
        break;
        
    case L_ON:
        if (millis()-cTime > LKEEP) { // if delay expired
            cLEDstate = L_GOFF;
            Serial.println("going to L_GOFF");
        }
        else {
        // special effect!!!
        // if sw is ON / OFF three times, display rainbow
            if (ss) {
                if (!cSWstate) {
                    Serial.println("checking special-func counter");
                    cSWstate = true;
                    if (++rCount==5/*20*/) {
                         //theaterChaseRainbow(3000);
                         rainbowCycle(5);
                         rCount = 0;
                    }
                }
            }
            else {
                cSWstate = false;
            }
        }
        break;
        
    case L_GOFF:
        for (int i=0; i<NEONUM ; i++)
            strip.setPixelColor(i, strip.Color(br, br, br /*bb(br)*/ /*(BMAX*br)/RMAX)*/));
        strip.show();
        delay(LCGAPD);
        if (br--==0) {
            cLEDstate = L_OFF;
            Serial.println("going to L_OFF");
        }
        break;        
  }
}

// get blue brightness
// y(x) = (a/b) * x - c
//       a/b = 3/20, c = 3 
int bb(int br) {
    int res;
    return ((res = (br*4)/5 - 3) <= 0 ? 0 : res);
}

// slow start increasing-brightness
// y(x) = 1/3 * x  (0 <= x <= 100
// y(x) = (230/205) * x - 31   (100 <= x <= RMAX
int sstart(int br) {
    if (br <= 100) return (br/3);
    return ((222*br)/155 - 110); 
}

/* 
  //아래의 순서대로 NeoPixel을 반복한다.
  colorWipe(strip.Color(255, 0, 0), 50); //빨간색 출력
  colorWipe(strip.Color(0, 255, 0), 50); //녹색 출력
  colorWipe(strip.Color(0, 0, 255), 50); //파란색 출력
//  colorWipe(strip.Color(250, 250, 250), 100); delay(2000);
//  strip.begin(); strip.show(); delay(5000);
//  colorWipe(strip.Color(0, 0, 0), 50); delay(10000);

  theaterChase(strip.Color(127, 127, 127), 50); //흰색 출력
  theaterChase(strip.Color(127,   0,   0), 50); //빨간색 출력
  theaterChase(strip.Color(  0,   0, 127), 50); //파란색 출력

  //화려하게 다양한 색 출력
  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}
*/

//NeoPixel에 달린 LED를 각각 주어진 인자값 색으로 채워나가는 함수
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

//모든 LED를 출력가능한 모든색으로 한번씩 보여주는 동작을 한번하는 함수
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//NeoPixel에 달린 LED를 각각 다른색으로 시작하여 다양한색으로 5번 반복한다
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { 
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//입력한 색으로 LED를 깜빡거리며 표현한다
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//LED를 다양한색으로 표현하며 깜빡거린다
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     //256가지의 색을 표현
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0); 
        }
    }
  }
}

//255가지의 색을 나타내는 함수
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
 
