// choi++20201101 AWS-IoT pub/sub test

#include <AWS_IOT.h>
#include <WiFi.h>

AWS_IOT hornbill;

const char* ssid = "KAU-Guest";
const char* password = "";
char HOST_ADDRESS[]="a1i1a9xq4rpdt3-ats.iot.ap-northeast-2.amazonaws.com";
char CLIENT_ID[]= "ChoiESP32";
char sTOPIC_NAME[]= "esp32/buttAck";  // subscribe topic name
char pTOPIC_NAME[]= "esp32/button";   // publish topic name

int status = WL_IDLE_STATUS;
int msgCount=0,msgReceived = 0;
char payload[512];
char rcvdPayload[512];
const int buttonPin = 15;  // the number of the pushbutton pin
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
    // initialize the pushbutton pin as an input
    pinMode(buttonPin, INPUT);
    delay(2000);
}

void loop() {

    if(msgReceived == 1)
    {
        msgReceived = 0;
        Serial.print("Received Message:");
        Serial.println(rcvdPayload);
    }
    if((millis()-preMil) > intMil)
    {
        // read the state of the pushbutton value
        if (digitalRead(buttonPin)) {

            preMil = millis();
//          sprintf(payload,"Hello from hornbill ESP32 : %d",msgCount++);
            sprintf(payload,"esp32/button");
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
