/*
 * ++choi JSON format topic
 *    <AWS-IoT message sequenc>
 *        1. thing: publish ../update 
 *            {"state":"reported":{"temp":nn,"humid":nn,"led":true/false}}
 *        2. thing: subscribe ../update/delta
 *        3. RuleEngine: republish  ../update {"state":"desired":{"led":true/false}} if temp < 20 or temp > 25
 *            <control_ledON_rule>
 *            SQL: SELECT {'desired':{'led':"ON"}} as state  (?? true/false is not working ??) 
 *                 FROM '$aws/things/myTestThing/shadow/update' 
 *                 WHERE state.reported.temp < 20
 *            Action: republish .. '$$aws/things...'
 *            <control_ledOFF_rule>
 *            SQL: SELECT {'desired':{'led':"OFF"}} as state 
 *                 FROM '$aws/things/myTestThing/shadow/update' 
 *                 WHERE state.reported.temp > 25            
 *        4. shadow publish ../update/delta
 *            {"version":30,"timestamp":1573721838,"state":{"led":"ON"},"metadata":...}
 */
 
#include <AWS_IOT.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#include "DHT.h"

#define DHTPIN 16 //25       // what digital pin we're connected to
#define LEDPIN 13 //26       // LED     
#define BTNPIN 4      // button

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);
boolean led_state = false;

AWS_IOT hornbill;   // AWS_IOT instance

//char WIFI_SSID[]="myLGNet_Eagle_2";
//char WIFI_PASSWORD[]="NavVision6022!";
char WIFI_SSID[]="KAU-Guest";
char WIFI_PASSWORD[]= "";
char HOST_ADDRESS[]="a1i1a9xq4rpdt3-ats.iot.ap-northeast-2.amazonaws.com";
char CLIENT_ID[]= "espDH11";
char TOPIC_NAME_update[]= "$aws/things/myTestThing/shadow/update";
char TOPIC_NAME_delta[]= "$aws/things/myTestThing/shadow/update/delta";

int status = WL_IDLE_STATUS;
int tick=0,msgCount=0,msgReceived = 0;
char payload[512];
char rcvdPayload[512];

// subscribe ../update/delta
void callBackDelta(char *topicName, int payloadLen, char *payLoad)
{
    strncpy(rcvdPayload,payLoad,payloadLen);
    rcvdPayload[payloadLen] = 0;
    msgReceived = 1;
}

void setup() {

    Serial.begin(115200);
    delay(2000);

    while (status != WL_CONNECTED)
    {
        //++choi
        //  This is here to force the ESP32 to reset the WiFi and initialise correctly.
        Serial.print("WIFI status = ");
        Serial.println(WiFi.getMode());
        WiFi.disconnect(true);
        delay(1000);
        WiFi.mode(WIFI_STA);
        delay(1000);
        Serial.print("WIFI status = ");
        Serial.println(WiFi.getMode());
        // End silly stuff !!!
        //++choi
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(WIFI_SSID);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        // wait 5 seconds for connection:
        delay(5000);
    }

    Serial.println("Connected to wifi");

    if(hornbill.connect(HOST_ADDRESS,CLIENT_ID)== 0) // Connect to AWS using Host Address and Cliend ID
    {
        Serial.println("Connected to AWS");
        delay(1000);

        if(0==hornbill.subscribe(TOPIC_NAME_delta,callBackDelta))
        {
            Serial.println("Subscribe(../update/delta) Successfull");
        }
        else
        {
            Serial.println("Subscribe(../update/delta) Failed, Check the Thing Name and Certificates");
            while(1);
        }
    }
    else
    {
        Serial.println("AWS connection failed, Check the HOST Address");
        while(1);
    }
    delay(2000);

    dht.begin(); //Initialize the DHT11 sensor
    pinMode(LEDPIN, OUTPUT);
    pinMode(BTNPIN, INPUT);
    digitalWrite(LEDPIN, LOW);
}


void loop() {

// at first, handle subscribe messages..
    StaticJsonDocument<200> msg;      // reserve stack mem for handling json msg
    if(msgReceived == 1)
    {
        msgReceived = 0;
        Serial.print("Received Message(Update/Delta):");
        Serial.println(rcvdPayload);
        // Deserialize the JSON document
        if (deserializeJson(msg, rcvdPayload)) {    // if error
            Serial.print(F("deserializeJson() failed.. \n"));
            while(1);
        }

    // parsing delta msg
    // ex) {"version":63,"timestamp":1573803485,"state":{"led":"OFF"},
    //        "metadata":{"led":{"timestamp":1573803485}}}
        JsonObject state = msg["state"];
        String led = state["led"];
        if (led.equals("ON"))          // turn ON
            led_state = true;
        else if (led.equals("OFF"))    // turn OFF
            led_state = false;
        else { // invalid delta
            Serial.println("Invalid ../delta message..");
            while (1);
        }
        digitalWrite(LEDPIN, led_state);
        // publish report led ON {"state":{"reported":{"led":"ON"}}}
        sprintf(payload,"{\"state\":{\"reported\":{\"led\":\"%s\"}}}",(led_state?"ON":"OFF"));
        if(hornbill.publish(TOPIC_NAME_update,payload) == 0)   // Publish the message
        {        
            Serial.print("Publish Message: ");   
            Serial.println(payload);
        }
        else
        {
            Serial.print("Publish failed: ");
            Serial.println(payload);
        }
    }

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
    }
    else
    {

//  publish report msg
//    { "state": {
//                "reported": { "temp": nn, "humid": nn, "led": "ON" | "OFF" }
//               }
//    }
        sprintf(payload,"{\"state\":{\"reported\":{\"temp\":%.1f,\"humid\":%.1f,\"led\":\"%s\"}}}",
                            t,h,(led_state ? "ON" : "OFF" ));
        if(hornbill.publish(TOPIC_NAME_update,payload) == 0)   // Publish the message
        {        
            Serial.print("Publish Message: ");   
            Serial.println(payload);
        }
        else
        {
            Serial.print("Publish failed: ");
            Serial.println(payload);
        }
        // publish the temp and humidity every 5 seconds.
        vTaskDelay(5000 / portTICK_RATE_MS);
    } 
}
