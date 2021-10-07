//Code for the QRE1113 Analog board 
//Outputs via the serial terminal - Lower numbers mean more reflected 

int QRE1113_Pin = 2; //connected to analog pin

void setup(){ 
  Serial.begin(115200); 
} 
void loop(){ 
  int QRE_Value = analogRead(QRE1113_Pin); 
  Serial.println(QRE_Value); 
  delay(1000);
}
