int val = 0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(17, OUTPUT);
}

void loop() {
    val = analogRead(35);
    digitalWrite(17, 0);
  //delayMicroseconds(val);
  delay(val);
  digitalWrite(17, 1);
  delay(val);
  Serial.println(val);
 // delayMicroseconds(val);
  // put your main code here, to run repeatedly:

}
