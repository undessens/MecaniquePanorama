void setup() {
  // put your setup code here, to run once:
  pinMode( A5, OUTPUT );
}

void loop() {

  digitalWrite(A5, LOW );
  delay ( 500);
  digitalWrite(A5, HIGH );
  delay ( 500);

}
