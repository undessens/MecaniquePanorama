/*
    Mecanique Panorama

    Created by Aurelien Conil
    github.com/undessens/mecanique_panorama


*/

/*    
simple both ways counter from a turning wheel, using
optical IR sensor ( pin A0, A1 ) and IR emitter A2

+

4 buttons, used as selector
pin 10, 11, 12, 13
+

relay , turning on and off according to wheel frequency
pin 8


*/ 

int pinIR = A2;

//light sensor
int pinSens1 = A0;
int pinSens2 = A1;

//Buttons from sequence choices
int pinBut1 = 9;
int pinBut2 = 10;
int pinBut3 = 11;
int pinBut4 = 12;

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
  pinMode ( pinBut1, INPUT_PULLUP );
  pinMode ( pinBut2, INPUT_PULLUP );
  pinMode ( pinBut3, INPUT_PULLUP );
  pinMode ( pinBut4, INPUT_PULLUP );
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
  
  boolean newBut1 = digitalRead(pinBut1);
  boolean newBut2 = digitalRead(pinBut2);
  boolean newBut3 = digitalRead(pinBut3);
  boolean newBut4 = digitalRead(pinBut4);
  
  // if no button is pressed
  if(currentChoice == 0){
     //check if any button is pressed
    
    
    if(newBut1){
      changeChoice(1);
    }
    else if(newBut2){
      changeChoice(2);
    }
    else if(newBut3){
      changeChoice(3);
    }
    else if(newBut4) {
      changeChoice(4);
    }
  
  }
  else{
    
   switch(currentChoice){

    /*if button is not pressed anymore, wait 400ms in changeChoice
     *  then recheak the digital pin, using updateChoiceButton's boolean return
     */
    
    case 1: if(!newBut1) changeChoice(0);
    break;
    case 2: if(!newBut2) changeChoice(0);
    break;
    case 3: if(!newBut3) changeChoice(0);
    break;
    case 4: if(!newBut4) changeChoice(0);
    break;
    
   } 
    
  }
  
  return ( newBut1 || newBut2 || newBut3 || newBut4 );
}


void changeChoice( int i ){
  
  if( i >= 0 && i < 10 ){
    
    if( i == 0 ){
       //Be sure that user is not going to press another button
      delay(400 );
      currentChoice = 0;
      if(!updateChoiceButton()){
        //nothing is pressed after 500 ms of released 
        Serial.print( "s"); 
        Serial.println(char('0'));
      }
   
    }
    else
    {
      Serial.print( "s"); 
      Serial.println(char('0' + i)); 
      currentChoice = i;
      delay(200);
    }
  
  }
   
}

void updateRelay(){


 if (count%relayModulo==0){

  //relayState = !relayState;
  //digitalWrite(pinRelay, relayState);
  digitalWrite(pinRelay, HIGH); 
 }
  else{
   digitalWrite(pinRelay,LOW); 
 }


}

