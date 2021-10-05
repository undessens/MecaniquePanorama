
/*
 Mecanique Panorama
 
   Created by Aurelien Conil
   Test of optical sensor
   
   
*/

int pinIR = A2;

//light sensor
int pinSens1 = A0;
int pinSens2 = A1;

boolean state1 = false;
boolean state2 = false;

int pinRelay = A5;

void setup(){
  
  pinMode (  pinIR, OUTPUT);
  pinMode ( pinSens1, INPUT);
  pinMode ( pinSens2, INPUT);
  pinMode ( pinRelay, OUTPUT );
  digitalWrite(pinRelay, HIGH); 
  
  digitalWrite(pinIR, HIGH);
  
  Serial.begin(115200);
    
 
}

void loop(){
  
  int sens1 = analogRead(pinSens1);
  int sens2 = analogRead(pinSens2);

  
  if(state1 && (sens1 < 550))
  state1 = false;
  if(!state1 && (sens1 > 900))
  state1 = true;
  if(state2 && (sens2 < 550))
  state2 = false;
  if(!state2 && sens2 > 900)
  state2 = true;

  
  // test des capteurs en analogique

/*
  Serial.print(sens1);
  Serial.print(" ");
  Serial.print(sens2);
  Serial.print(" ");
  Serial.print(state1*1048 + 50);
  Serial.print(" ");
  Serial.println(state2*1048 + 50 );
*/
 Serial.print(sens1);
 Serial.print(" ");
 Serial.println(sens2);
/*
  if(bsens1 && !state1){
     
     Serial.println("sens1 UP"); 
  }
  if(!bsens1 && state1){
     
     Serial.println("sens1 down"); 
  }
  if(bsens2 && !state2){
     
     Serial.println("sens2 UP"); 
  }
  if(!bsens2 && state2){
     
     Serial.println("sens2 down"); 
  }
  
  

  
  state1 = bsens1;
  state2 = bsens2;
  */
  
}
