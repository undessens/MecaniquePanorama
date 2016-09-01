/*
    Mecanique Panorama

    Created by Aurelien Conil
    
simple both ways counter from a turning wheel, using
optical IR sensor



*/
int pinIR = A2;
int pinSens1 = A0;
int pinSens2 = A1;
boolean sens1 = false;
boolean sens2 = false;
boolean direction = false; //clockwise, anti-clockwise
int count= 0;
unsigned long time;
float speed;



void setup(){
  
  pinMode (  pinIR, OUTPUT);
  pinMode ( pinSens1, INPUT);
  pinMode ( pinSens2, INPUT);
  
  digitalWrite(pinIR, HIGH);
  
  
  Serial.begin(115200);
  
}


void loop(){
  
  boolean newSens1 = digitalRead(pinSens1);
  boolean newSens2 = digitalRead(pinSens2);
  
  
  if ( newSens1 &&  sens2 && !sens1 ){
     

   //count1++;
   
   //float newSpeed1 = 1000/( millis() - time1 );
   //float alpha = 0.3;
   //speed1 = newSpeed1*alpha + speed1*(1.0 - alpha);
   Serial.println( "+");   
   
   //time1 = millis(); 
  }
  if ( newSens2 &&  sens1 && !sens2 ){
     
    Serial.println("-");
  }

  
  
  
  sens1 = newSens1; 
  sens2 = newSens2;
  
}
