#define RED 3
#define ADC 7
int val = 0;

void setup() {
  pinMode(RED, OUTPUT);
}

void loop() {
  val = analogRead(ADC);
  for(int i = 0; i <= 255; i++) {
     analogWrite(RED, i);
     delayMicroseconds(val*10);
     delayMicroseconds(val*10);
     delayMicroseconds(val*10);
     delayMicroseconds(val*10);
     delayMicroseconds(val*10);
  }

  for(int i = 255; i >= 0; i--) {
     analogWrite(RED, i);
     delayMicroseconds(val*10);
     delayMicroseconds(val*10);
     delayMicroseconds(val*10);
     delayMicroseconds(val*10);
     delayMicroseconds(val*10);
  }
}