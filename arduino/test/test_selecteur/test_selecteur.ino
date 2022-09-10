//Buttons from sequence choices
int pinBut1 = 9;
int pinBut2 = 10;
int pinBut3 = 11;
int pinBut4 = 12;


void setup() {
  // put your setup code here, to run once:


  pinMode ( pinBut1, INPUT_PULLUP );
  pinMode ( pinBut2, INPUT_PULLUP );
  pinMode ( pinBut3, INPUT_PULLUP );
  pinMode ( pinBut4, INPUT_PULLUP );

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  boolean newBut1 = digitalRead(pinBut1);
  boolean newBut2 = digitalRead(pinBut2);
  boolean newBut3 = digitalRead(pinBut3);
  boolean newBut4 = digitalRead(pinBut4);

  if (newBut1) Serial.print("1");
  else Serial.print("-");
  if (newBut2) Serial.print("2");
  else Serial.print("-");
  if (newBut3) Serial.print("3");
  else Serial.print("-");
  if (newBut4) Serial.print("4");
  else Serial.print("-");
  Serial.println(" : ");

  delay(300);

}
