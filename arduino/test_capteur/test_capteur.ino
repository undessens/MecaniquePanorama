
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

void setup(){
  
  pinMode (  pinIR, OUTPUT);
  pinMode ( pinSens1, INPUT);
  pinMode ( pinSens2, INPUT);
  
  digitalWrite(pinIR, HIGH);
  
  Serial.begin(115200);
    
 
}

void loop(){
  
  int sens1 = analogRead(pinSens1);
  int sens2 = analogRead(pinSens2);
  
  boolean bsens1 = sens1 < 999;
  boolean bsens2 = sens2 < 999;
  
  // test des capteurs en analogique
  Serial.print("sens1 : ");
  Serial.print(sens1);
  Serial.print(" --- sens2 : ");
  Serial.println(sens2);


//  if(bsens1 && !state1){
//     
//     Serial.println("sens1 UP"); 
//  }
//  if(!bsens1 && state1){
//     
//     Serial.println("sens1 down"); 
//  }
//  if(bsens2 && !state2){
//     
//     Serial.println("sens2 UP"); 
//  }
//  if(!bsens2 && state2){
//     
//     Serial.println("sens2 down"); 
//  }
  
  

  
  state1 = bsens1;
  state2 = bsens2;
  
}
