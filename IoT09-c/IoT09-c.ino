// choi++20201101 IoT09-c AWS-IoT bme280 / led pub/sub test
//                parse topic payload using ArduinoJson
//              merge IoT06-1

#include <AWS_IOT.h>
#include <WiFi.h>
#include <Arduino_JSON.h>   // refer JSONObject example for more information!!!
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
AWS_IOT hornbill;

const char* ssid = "KAU-Guest";
const char* password = "";
char HOST_ADDRESS[]="a1i1a9xq4rpdt3-ats.iot.ap-northeast-2.amazonaws.com";
char CLIENT_ID[]= "ChoiESP32";
char sTOPIC_NAME[]= "esp32/led";      // subscribe topic name
char pTOPIC_NAME[]= "esp32/bme280";   // publish topic name

//int status = WL_IDLE_STATUS;
int msgCount=0,msgReceived = 0;
char payload[512];
char rcvdPayload[512];
const int buttonPin = 15;   // the number of the pushbutton pin
const int ledPin = 16;      // led pin
unsigned long preMil = 0; 
const long intMil = 5000; 

void mySubCallBackHandler (char *topicName, int payloadLen, char *payLoad)
{
    strncpy(rcvdPayload,payLoad,payloadLen);
    rcvdPayload[payloadLen] = 0;
    msgReceived = 1;
}

void setup() {
    Serial.begin(115200);
    // Connect to Wi-Fi 
    //++choi for testing
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
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to wifi");
    if(hornbill.connect(HOST_ADDRESS,CLIENT_ID)== 0)
    {
        Serial.println("Connected to AWS");
        delay(1000);

        if(0==hornbill.subscribe(sTOPIC_NAME,mySubCallBackHandler))
        {
            Serial.println("Subscribe Successfull");
        }
        else
        {
            Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
            while(1);
        }
    }
    else
    {
        Serial.println("AWS connection failed, Check the HOST Address");
        while(1);
    }
    // initialize the pushbutton pin, led pin
    pinMode(buttonPin, INPUT);
    pinMode(ledPin, OUTPUT);
    delay(2000);

    // initialize bme280
    bool stat = bme.begin(0x76);  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!stat) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
}

void loop() {

    if(msgReceived == 1)
    {
        msgReceived = 0;
        Serial.print("Received Message:");
        Serial.println(rcvdPayload);
        // Parse JSON
        JSONVar myObj = JSON.parse(rcvdPayload);
        JSONVar state = myObj["state"];
        String led = (const char*) state["led"];
        Serial.print("LED will be ");
        Serial.println(led);
        if (led == "ON")
            digitalWrite(ledPin, HIGH);
        else if (led == "OFF")
            digitalWrite(ledPin, LOW);
    }
    if((millis()-preMil) > intMil)
    {
        // read the state of the pushbutton value
        if (digitalRead(buttonPin)) {

            int temp = bme.readTemperature();
            int humid = bme.readHumidity();
            int press = bme.readPressure() / 100.0F;
            preMil = millis();
//          sprintf(payload,"Hello from hornbill ESP32 : %d",msgCount++);
//          sprintf(payload,"ESP32-Button:001 Pressed!!");
            sprintf(payload,"{\"temp\":%d, \"humid\":%d, \"press\":%d}",temp,humid,press);
            
            if(hornbill.publish(pTOPIC_NAME,payload) == 0)
            {        
                Serial.print("Publish Message:");
                Serial.println(payload);
            }
            else
            {
                Serial.println("Publish failed");
            }
        }
    }  
}
