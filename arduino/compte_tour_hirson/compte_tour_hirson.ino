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
pin A5

bouton OFF : send a turn off message to computer
pin 8 ( 2nd wire to GND )


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
boolean state1 = false;
boolean state2 = false;
boolean direction = false; //clockwise, anti-clockwise
boolean previousDirection = false;
float speedDirectionChangeThreshold = 2.0;
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
  
  int sens1 = analogRead(pinSens1);
  int sens2 = analogRead(pinSens2);
  boolean newState1 = state1;
  boolean newState2 = state2;

  
  if(state1 && (sens1 < 600))
  newState1 = false;
  if(!state1 && (sens1 > 750))
  newState1 = true;
  if(state2 && (sens2 < 600))
  newState2 = false;
  if(!state2 && (sens2 > 750))
  newState2 = true;

  
  if ( newState1 &&  state2 && !state1 ){
    computeSpeed();
    direction = filterDirection(true);
    
    if(direction)
      Serial.print( "+"); 
    else
      Serial.print( "-"); 
    Serial.println(char('0' + speed)); 
    updateRelay();
  }
  if ( newState2 &&  state1 && !state2 ){
    computeSpeed();
    
    direction = filterDirection(false);
    
    if(direction)
      Serial.print( "+"); 
    else
      Serial.print( "-"); 
    Serial.println(char('0' + speed));

    updateRelay();
  }

  state1 = newState1; 
  state2 = newState2;
  
  updateChoiceButton();
}

boolean filterDirection(boolean dir) {
  //filtrage
  if(previousDirection != dir && speed>=speedDirectionChangeThreshold ) { //Impossible d'avoir un changement de direction sur une frame alors que l'on va vite.
       dir = !dir;
  }
  previousDirection = dir;
  return dir;
}

void computeSpeed() {
  float newSpeed = 1000/( millis() - time );
   float alpha = 0.3;
   speed = newSpeed*alpha + speed*(1.0 - alpha);
   //transform speed to stay around 0 and 10
   speed = map(int(speed),0,40, 0, 15);
   //if(speed > 20) speed = 20;
   time = millis(); 
   count --;
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
