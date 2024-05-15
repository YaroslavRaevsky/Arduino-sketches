void setup() {
  // put your setup code here, to run once:
//pinMode(22, INPUT_PULLUP);
pinMode(25, OUTPUT);
pinMode(26, OUTPUT);
}

void loop() {
//  if (digitalRead(22)){
  digitalWrite(25, HIGH);
  delay(500);
  digitalWrite(25, LOW);
  delay(500);
//  }
 // else{
  digitalWrite(26, HIGH);
  delay(500);
  digitalWrite(26, LOW);
  delay(500);
 // }
  // put your main code here, to run repeatedly:
}
