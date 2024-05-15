/*
  Mega analogWrite() test

  This sketch fades LEDs up and down one at a time on digital pins 2 through 13.
  This sketch was written for the Arduino Mega, and will not work on other boards.

  The circuit:
  - LEDs attached from pins 2 through 13 to ground.

  created 8 Feb 2009
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogWriteMega
*/

int sensorValue = 0; 

void setup() {
  pinMode(3, OUTPUT);
}

void loop() {
  sensorValue = analogRead(A2)/4;
    analogWrite(3, sensorValue);
    // pause between LEDs:
    delay(10);
}
