/*
    Mecanique Panorama

    Created by Alex Bourgeois, based on Aurelien Conil
    github.com/undessens/mecanique_panorama
*/

/*    
ESP32 system

simple both ways counter from a turning wheel, using
optical IR sensor( pin 4, 27 )

relay , turning on and off according to wheel frequency
pin 25

baudrate 115200
*/ 

//light sensor
int recepteurInterieur = 4;
int recepteurExterieur = 27;

//Algorithm
int lastState1, lastState2;
long long int count= 0;
unsigned long lastTime;
float speed;
float speedRotationChangeFilterThreshold = 5.0;

// Relay command
int pinRelay = 25;
int relayModulo = 15;

void setup(){

  pinMode ( recepteurInterieur, INPUT);
  pinMode ( recepteurExterieur, INPUT);

  lastState1 = digitalRead(recepteurInterieur);
  lastState2 = -1;

  pinMode ( pinRelay, OUTPUT );
  
  //Toggle relay to test it
  digitalWrite(pinRelay, LOW); 
  digitalWrite(pinRelay, HIGH); 
  delay(250);
  digitalWrite(pinRelay, LOW); 
  digitalWrite(pinRelay, HIGH); 
  delay(250);
  digitalWrite(pinRelay, LOW); 
  
  Serial.begin(115200);
}

bool rotationDirection = false;
bool previousRotationDirection = false;

void loop(){
  
  int currentState1 = digitalRead(recepteurInterieur);
  int currentState2 = digitalRead(recepteurExterieur);

  // The wheel turned
  if (currentState1 != lastState1) {

    // if the first one changed before the other, we can define direction
    if (currentState1 != currentState2) {
      rotationDirection = false;
    } else {
      rotationDirection = true;
    }
    
    count++;

    //Compute speed
    float newSpeed = 1000/( millis() - lastTime );
    float alpha = 0.3;
    speed = newSpeed*alpha + speed*(1.0 - alpha);

    //transform speed to stay around 0 and 10
    speed = map(int(speed),0,40, 0, 15);

    //Filter out high speed rotation direction error
    if(speed >= speedRotationChangeFilterThreshold && (rotationDirection != previousRotationDirection)) {
        rotationDirection = previousRotationDirection;
    }

    //Set variables
    previousRotationDirection = rotationDirection;
    lastTime = millis();

    //Send data
    if(rotationDirection) {
      Serial.print("+");   
    }
    else {
      Serial.print("-"); 
    }
    Serial.println(char('0' + speed)); 

    updateRelay();
  }

  // Mettre à jour l'état actuel
  lastState1 = currentState1;
  lastState2 = currentState2;
}

void updateRelay(){
  if (count%relayModulo==0){
    digitalWrite(pinRelay, HIGH); 
  }
  else{
    digitalWrite(pinRelay,LOW); 
  }
}