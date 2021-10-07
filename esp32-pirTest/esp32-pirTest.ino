int inputPIN = 36;    
int statusPIR = 0;    
int valueRead = 0;  
int ledPin = 16;
  
void setup() {  
   pinMode(inputPIN, INPUT);
   pinMode(ledPin, OUTPUT);   
   Serial.begin(115200);    
}  
  
void loop(){  
  valueRead = digitalRead(inputPIN);    
  if ((valueRead == HIGH)  &&  (statusPIR == LOW)) {    
      Serial.println("Motion detected!");    
      statusPIR = HIGH;    
   }  
  else{  
      if((valueRead == LOW) && (statusPIR == HIGH)){  
        Serial.println("Motion ended!");    
        statusPIR = LOW;  
      }  
   }
   digitalWrite(ledPin,statusPIR);  
   delay(1000);  
}  
