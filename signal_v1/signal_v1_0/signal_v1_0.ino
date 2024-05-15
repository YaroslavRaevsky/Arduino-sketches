#define S0_1 3
#define S1_1 21
#define S2_1 22
#define S3_1 23
#define EN_1 18
#define SIG_1 4
#define S0_2 26
#define S1_2 32
#define S2_2 33
#define S3_2 25
#define EN_2 34
#define SIG_2 19

//int c = 15;
bool s0, s1, s2, s3;
int Leds[16] = {0, 0, 0, 0, 5, 6, 7, 0, 9, 10, 11, 0, 0, 0, 0, 15};
int Pins[16];

void setup() {
Serial.begin(9600);
  pinMode(EN_1, OUTPUT);
  pinMode(S0_1, OUTPUT);
  pinMode(S1_1, OUTPUT);
  pinMode(S2_1, OUTPUT); 
  pinMode(S3_1, OUTPUT); 
  pinMode(SIG_1, OUTPUT);
  pinMode(EN_2, OUTPUT);
  pinMode(S0_2, OUTPUT);
  pinMode(S1_2, OUTPUT);
  pinMode(S2_2, OUTPUT); 
  pinMode(S3_2, OUTPUT); 
  pinMode(SIG_2, INPUT_PULLUP);
}

int MUXout(int c)
{ 
  if (c>0)
  {
  s0 = (c-1)%2;
  s1 = (c-1)/2%2;
  s2 = (c-1)/4%2;
  s3 = (c-1)/8%2;
  digitalWrite(EN_1, HIGH);
  digitalWrite(S0_1, s0);
  digitalWrite(S1_1, s1);
  digitalWrite(S2_1, s2);
  digitalWrite(S3_1, s3);
  digitalWrite(SIG_1, HIGH);
  digitalWrite(EN_1, LOW);
  delay(50);
  }
}

void MUXin()
{
  for (int i=0; i<=15; i++)
  {
  s0 = i%2;
  s1 = i/2%2;
  s2 = i/4%2;
  s3 = i/8%2;
  digitalWrite(EN_2, HIGH);
  digitalWrite(S0_2, s0);
  digitalWrite(S1_2, s1);
  digitalWrite(S2_2, s2);
  digitalWrite(S3_2, s3);
  digitalWrite(EN_2, LOW);
      delay(5);
  Pins[i] = (1-digitalRead(SIG_2))*(i+1);
 // Serial.println(digitalRead(SIG_2));
 //   delay(1000);
  }
}

void loop() {
/*  digitalWrite(EN_2, HIGH);
  digitalWrite(S0_2, 1);
  digitalWrite(S1_2, 1);
  digitalWrite(S2_2, 1);
  digitalWrite(S3_2, 1);
  digitalWrite(EN_2, LOW);
  Serial.println(digitalRead(SIG_2));
    delay(1000);
*/
  MUXin();
  for (int i=0; i<=15; i++)
  {
  MUXout(Pins[i]);
  delay(30);
  }
}