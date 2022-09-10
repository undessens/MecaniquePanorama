/*
    Mecanique Panorama

    Created by Aurelien Conil
    github.com/undessens/mecanique_panorama


*/

/*    
simple both ways counter from a turning wheel, using
optical IR sensor ( pin A0, A1 ) and IR emitter A2

+


relay , turning on and off according to wheel frequency
pin 8


*/ 

int pinIR = A2;

//light sensor
int pinSens1 = A0;
int pinSens2 = A1;

//Buttons turn off computer
int pinButOff = 8;

//Algorithm
int currentChoice = 0;
boolean sens1 = false;
boolean sens2 = false;
boolean direction = false; //clockwise, anti-clockwise
long long int count= 0;
unsigned long time;
float speed;

// Relay command
boolean relayState = false;
int pinRelay = A5;
int relayModulo = 15;



void setup(){
  
  pinMode (  pinIR, OUTPUT);
  pinMode ( pinSens1, INPUT);
  pinMode ( pinSens2, INPUT);
  pinMode ( pinButOff, INPUT_PULLUP );
  pinMode ( pinRelay, OUTPUT );
  
  digitalWrite(pinIR, HIGH);
  
  
  Serial.begin(115200);
  
}


void loop(){
  
  boolean newSens1 = analogRead(pinSens1) < 680;
  boolean newSens2 = analogRead(pinSens2) < 680;
  
  
  if ( newSens1 &&  sens2 && !sens1 ){
     

   float newSpeed = 1000/( millis() - time );
   float alpha = 0.3;
   speed = newSpeed*alpha + speed*(1.0 - alpha);
   //transform speed to stay around 0 and 10
   speed = map(int(speed),0,40, 0, 10);
   if(speed > 9) speed = 9;
   time = millis();
   Serial.print( "+"); 
   Serial.println(char('0' + speed)); 

    count --;
    updateRelay();
   
   
  }
  if ( newSens2 &&  sens1 && !sens2 ){
    
   float newSpeed = 1000/( millis() - time );
   float alpha = 0.3;
   speed = newSpeed*alpha + speed*(1.0 - alpha);
   //transform speed to stay around 0 and 10
   speed = map(int(speed),0,40, 0, 10);
   if(speed > 9) speed = 9;
   time = millis(); 
   count --;
   Serial.print("-");
   Serial.println(char('0' + speed));

   updateRelay();
  }

  sens1 = newSens1; 
  sens2 = newSens2;
  
  updateChoiceButton();

  
  
}

boolean updateChoiceButton(){
  
  boolean newButOff = digitalRead(pinButOff);

  if( newButOff == LOW ){

    delay(3000);
    if(digitalRead(pinButOff) == LOW ){
      Serial.println( "q1"); 
    }
  }
  

}


void updateRelay(){


 if (count%relayModulo==0){

  //relayState = !relayState;
  //digitalWrite(pinRelay, relayState);
  digitalWrite(pinRelay, LOW); 
 }
  else{
   digitalWrite(pinRelay,HIGH); 
 }


}
