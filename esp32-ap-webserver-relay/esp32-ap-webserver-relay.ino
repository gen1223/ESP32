/* Web Server controlling relays 
 *  ap mode..
 *  choi 191219 created
 *  WiFi.softAP(ssid, pwd, channel, hidden, connect#)
 *  -> is not working if pwd is too short (less than 8)
 */

#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

#define NUM_RELAYS  8       // no of relays
#define SIGLINE     6       
#define BLLINE      1
#define NEOPIN      25
#define NEONUM      6
#define BBL_DELAY   500     // butt blink delay
#define BBL_NUM     3       // no of butt blinks
#define SU_DELAY    200     // relay step-up delay 

int relayGPIOs[NUM_RELAYS] = {15, 13, 14, 2, 4, 5, 16, 26};

// Replace with your network credentials
const char* ssid = "ESP32-AP01";
const char* password = "123456789";   // !!choi softAP() is not working if password is shorter than 8

// Set web server port number to 80
WiFiServer server(80);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEONUM, NEOPIN, NEO_GRB + NEO_KHZ800);

// Variable to store the HTTP request
String header;
// Relay state
int RState[NUM_RELAYS] = {0,0,0,0,0,0,0,0};
// Blink state
int SState = 0;
int BState = 0;

void setup() {
    Serial.begin(115200);
    strip.begin();
    neoInit();
    
    // Set all relays to off when the program starts - if set to Normally Open (NO), 
    // the relay is off when you set the relay to HIGH
    for(int i=0; i<NUM_RELAYS; i++){
        pinMode(relayGPIOs[i], OUTPUT);
        digitalWrite(relayGPIOs[i], HIGH);
    }
   
    // Connect to Wi-Fi network with SSID and password
    Serial.print(F("Setting AP (Access Point)..."));
 
    // Remove the password parameter, if you want the AP (Access Point) to be open
    WiFi.softAP(ssid, password, 3, 0, 1);   // !!choi ssid, pwd, channel, hidden, connect#

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
  
    server.begin();
}

void loop(){
    WiFiClient client = server.available(); // Listen for incoming clients
    if (client) {             // If a new client connects
        Serial.println("New Client."); // print a message out in the serial port
        String currentLine = ""; // make a String to hold incoming data from the client
        while (client.connected()) { // loop while the client's connected
            if (client.available()) { // if there's bytes to read from the client,
                char c = client.read(); // read a byte, then
                Serial.write(c);        // print it out the serial monitor
                header += c;
                if (c == '\n') { // if the byte is a newline character
                // if the current line is blank, you got two newline characters in a row.
                // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0) {
                    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                    // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();
                        // turns the GPIOs on and off
                        for (int i=0; i<NUM_RELAYS; i++) {
                          if (header.indexOf("GET /"+String(i+1)+"/on") >= 0) {
                            Serial.println("Relay"+String(i+1)+" on");
                            //RState[i] = 1;
                            //digitalWrite(relayGPIOs[i], LOW);
                            relayONbelow(i);
                          } else if (header.indexOf("GET /"+String(i+1)+"/off") >= 0) {
                            Serial.println("Relay"+String(i+1)+" off");
                            //RState[i] = 0;
                            //digitalWrite(relayGPIOs[i], HIGH);
                            relayOFFabove(i);
                          } 
                        }
                        // turns Blink leds on and off
                        if (header.indexOf("GET /sigBlink/on") >= 0) {
                            Serial.println("SigBlink on");
                            SState = 1;
                        } else if (header.indexOf("GET /sigBlink/off") >= 0) {
                            Serial.println("SigBlink off");
                            SState = 0;
                        } 
                        if (header.indexOf("GET /buttBlink/on") >= 0) {
                            Serial.println("Butt Blink on");
                            BState = 1;
                        } else if (header.indexOf("GET /buttBlink/off") >= 0) {
                            Serial.println("Butt Blink off");
                            BState = 0;
                        } 

                        // Display the HTML web page
                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\"content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");
                        // CSS to style the on/off buttons
                        // Feel free to change the background-color and font-size attributes to fit your preferences
                        client.println("<style>html { font-family: Helvetica; display:inline-block; margin: 10px auto; text-align: left;}");
                        client.println(".buttonON { width:100px; background-color: #FE1F01; border-radius:0.5em; color: #FFFFFF; padding: 10px 10px;");
                        client.println("text-decoration: none; font-size: 30px; margin:0px 20px; cursor: pointer;}");
                        client.println(".buttonACT {background-color:#07F60B;}");
                        client.println(".buttonOFF {background-color:#BDBDBD;}</style></head>");
                        // Web Page Heading
//                        client.println("<body><h2>Energy System</h2>");
                        // Display current state, and ON/OFF buttons for Relays
                        // If RState[i] is on, it displays the ON button
                        for (int i=NUM_RELAYS-1; i>=0; i--) {
                          if (i==SIGLINE) {
                            if (RState[i]==1) {
                              client.println("<p><a href=\"/"+String(i+1)+"/off\"><button class=\"buttonON\">"+String(i+1)+"</button></a>");
                            } else {
                              client.println("<p><a href=\"/"+String(i+1)+"/on\"><button class=\"buttonON buttonOFF\">"+String(i+1)+"</button></a>");
                            }
/* always '/on'
                            if (SState==1) {
                              client.println("<a href=\"/sigBlink/off\"><button class=\"buttonON\">SIG</button></a></p>");
                            } else {
                              client.println("<a href=\"/sigBlink/on\"><button class=\"buttonON buttonOFF\">SIG</button></a></p>");
                            } */
                            client.println("<a href=\"/sigBlink/on\"><button class=\"buttonON buttonACT\">SIG</button></a></p>");
                          }
                          else if (i==BLLINE) {
                            if (RState[i]==1) {
                              client.println("<p><a href=\"/"+String(i+1)+"/off\"><button class=\"buttonON\">"+String(i+1)+"</button></a>");
                            } else {
                              client.println("<p><a href=\"/"+String(i+1)+"/on\"><button class=\"buttonON buttonOFF\">"+String(i+1)+"</button></a>");
                            }
/* always '/on'
                            if (BState==1) {
                              client.println("<a href=\"/buttBlink/off\"><button class=\"buttonON\">BLINK</button></a></p>");
                            } else {
                              client.println("<a href=\"/buttBlink/on\"><button class=\"buttonON buttonOFF\">BLINK</button></a></p>");
                            } */
                            client.println("<a href=\"/buttBlink/on\"><button class=\"buttonON buttonACT\">BLINK</button></a></p>");
                          }
                          else {
                            if (RState[i]==1) {
                              client.println("<p><a href=\"/"+String(i+1)+"/off\"><button class=\"buttonON\">"+String(i+1)+"</button></a></p>");
                            } else {
                              client.println("<p><a href=\"/"+String(i+1)+"/on\"><button class=\"buttonON buttonOFF\">"+String(i+1)+"</button></a></p>");
                            }
                          }
                        }
                        client.println("</body></html>");
                        // The HTTP response ends with another blank line
                        client.println();
                        // Break out of the while loop
                        break;
                    } else { // if you got a newline, then clear currentLine
                        currentLine = "";
                    }  
                } 
                else if (c != '\r') { // if you got anything else but a carriage return character,
                    currentLine += c; // add it to the end of the currentLine
                }
            }
        } // while
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }

    // operate blinking
    if (SState==1){
        sigBlink();
        SState = 0;
    }
    if (BState==1){
        buttBlink();
        BState = 0;
    }
}

// set on relay below n
void relayONbelow(int n) {
    int i;
    for (i=n; i>=0; ) {
        if (RState[i]) break;
        i--;
    }
    for (i++; i<=n; i++) {
        RState[i] = 1;
        digitalWrite(relayGPIOs[i], LOW);
        delay(SU_DELAY);
    }
}

// set off relay above n
void relayOFFabove(int n) {
    int i;
    for (i=n; i<NUM_RELAYS; ) {
        if (!RState[i]) break;
        i++;
    }
    for (i--; i>=n; i--) {
        RState[i] = 0;
        digitalWrite(relayGPIOs[i], HIGH);
        delay(SU_DELAY);
    }
}

// inital status for neopixel is unstable.. OK anyway if set OFF, after set ON 
// choi
void neoInit() {
    for (int i=0; i<NEONUM; i++)
        strip.setPixelColor(i, strip.Color(1,1,1));
    strip.show();
    delay(10);
    for (int i=0; i<NEONUM; i++)
        strip.setPixelColor(i, strip.Color(0,0,0));
    strip.show();
    delay(10);
    strip.show();
}

// 
void sigBlink() {
      Serial.println("Sig Blink started..");
      theaterChaseRainbow(10);
      neoInit();
      Serial.println("Sig Blink ended..");   //
}

// blinking all ON-leds 
void buttBlink() {
      Serial.println("Butt Blink started..");
      for (int j=0; j<BBL_NUM; j++) {
        for (int i=0; i<NUM_RELAYS; i++) {
          if (RState[i])
            digitalWrite(relayGPIOs[i], HIGH);
        }
        delay(BBL_DELAY);
        for (int i=0; i<NUM_RELAYS; i++) {
          if (RState[i])
            digitalWrite(relayGPIOs[i], LOW);
        }
        delay(BBL_DELAY);
      }
      Serial.println("Butt Blink ended..");
}

//LED를 다양한색으로 표현하며 깜빡거린다
void theaterChaseRainbow(uint8_t wait) {
//  static int j = 0;
  for (int j=0; j < 256; j+=10) {     //256가지의 색을 표현
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
//    j += 10;
//    j = j % 255;
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
 
