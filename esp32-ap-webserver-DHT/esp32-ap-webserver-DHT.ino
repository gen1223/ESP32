/* Web Server using DHT11, LED 
 *  ap mode..
 *  choi
 */

#include <WiFi.h>
#include "DHT.h"

#define LED1 12         //26
#define LED2 13         //27
#define DHTPIN 16       //25
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

// Replace with your network credentials
const char* ssid = "ESP32-AP001";
const char* password = "123456789";
//const char* ssid = "myLGNet_Eagle_2";
//const char* password = "NavVision6022!";
//const char* ssid = "KAU-Guest";
//const char* password = "";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
// Auxiliar variables to store the current output state
String LED1State = "off";
String LED2State = "off";

void setup() {
    Serial.begin(115200);
    dht.begin(); //Initialize the DHT11 sensor

    // Initialize the output variables as outputs
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    // Set outputs to LOW
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    // Connect to Wi-Fi network with SSID and password
    Serial.print(F("Setting AP (Access Point)..."));
 
    // Remove the password parameter, if you want the AP (Access Point) to be open
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
  
/*--
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP()); */
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
                      // checking if header is valid
                      // dXNlcjpna2RyaGQ= = 'user:gkdrhd' (user:pass) base64 encode
                      // Finding the right credential string, then loads web page
                      if(header.indexOf("dXNlcjpna2RyaGQ=") >= 0) {

                        // Reading temperature or humidity takes about 250 milliseconds!
                        // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
                        float h = dht.readHumidity();
                        // Read temperature as Celsius (the default)
                        float t = dht.readTemperature();
                        // Read temperature as Fahrenheit (isFahrenheit = true)
                        float f = dht.readTemperature(true);
                    
                        // Check if any reads failed and exit early (to try again).
                        if (isnan(h) || isnan(t) || isnan(f)) {
                            Serial.println("Failed to read from DHT sensor!");
                            while(1);
                        }
                      
                    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                    // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();
                        // turns the GPIOs on and off
                        if (header.indexOf("GET /led1/on") >= 0) {
                            Serial.println("LED1 on");
                            LED1State = "on";
                            digitalWrite(LED1, HIGH);
                        } else if (header.indexOf("GET /led1/off") >= 0) {
                            Serial.println("LED1 off");
                            LED1State = "off";
                            digitalWrite(LED1, LOW);
                        } else if (header.indexOf("GET /led2/on") >= 0) {
                            Serial.println("LED2 on");
                            LED2State = "on";
                            digitalWrite(LED2, HIGH);
                        } else if (header.indexOf("GET /led2/off") >= 0) {
                            Serial.println("LED2 off");
                            LED2State = "off";
                            digitalWrite(LED2, LOW);
                        }
                        // Display the HTML web page
                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\"content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");
                        // CSS to style the on/off buttons
                        // Feel free to change the background-color and font-size attributes to fit your preferences
                        client.println("<style>html { font-family: Helvetica; display:inline-block; margin: 0px auto; text-align: center;}");
                        client.println(".button { background-color: #4CAF50; border:none; color: white; padding: 16px 40px;");
                        client.println("text-decoration: none; font-size: 30px; margin:2px; cursor: pointer;}");
                        client.println(".button2 {background-color:#555555;}</style></head>");
                        // Web Page Heading
                        client.println("<body><h1>ESP32 Web Server</h1>");
                        // Display current temp, humid
                        client.println("<p>Current Temperature: " + String(t) + " &degC</p>");
                        client.println("<p>Current Humidity: " + String(h) + " %</p>");
                        // Display current state, and ON/OFF buttons for LED1
                        client.println("<p>Heater State: " + LED1State + "</p>");
                        // If the LED1State is off, it displays the ON button
                        if (LED1State=="off") {
                            client.println("<p><a href=\"/led1/on\"><button class=\"button\">ON</button></a></p>");
                        } else {
                            client.println("<p><a href=\"/led1/off\"><button class=\"button button2\">OFF</button></a></p>");
                        }
                        // Display current state, and ON/OFF buttons for LED2
                        client.println("<p>Dehumidifier State: " + LED2State + "</p>");
                        // If the LED2State is off, it displays the ON button
                        if (LED2State=="off") {
                            client.println("<p><a href=\"/led2/on\"><button class=\"button\">ON</button></a></p>");
                        } else {
                            client.println("<p><a href=\"/led2/off\"><button class=\"button button2\">OFF</button></a></p>");
                        }
                        client.println("</body></html>");
                        // The HTTP response ends with another blank line
                        client.println();
                        // Break out of the while loop
                        break;
                        // Wrong user or password, so HTTP request fails...
                      } else {
                        client.println("HTTP/1.1 401 Unauthorized");
                        client.println("WWW-Authenticate: Basic realm=\"Secure\"");
                        client.println("Content-Type: text/html");
                        client.println();
                        client.println("<html>Authentication failed</html>");
                      }
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
}
