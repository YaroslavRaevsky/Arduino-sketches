const int SER = 16;
const int LATCH = 5;
const int CLK = 18;
// последовательность 
int LedOut1 = 0, LedOut2 = 0;
bool leds[16] = {1,0,1,0,1,1,1,0,0,0,1,1,1,0,1,0};
void setup() {
pinMode(SER, OUTPUT);
pinMode(LATCH, OUTPUT);
pinMode(CLK, OUTPUT);
}

void Shift16Leds()
{
int reg[8] = {128,64,32,16,8,4,2,1};
for (int i = 0; i<=7;i++) 
{
LedOut1 = LedOut1 + (leds[i]*reg[i]);
}
for (int i = 8; i<=15;i++) 
{
LedOut2 = LedOut2 + (leds[i]*reg[i-8]);
}
    digitalWrite(LATCH, LOW);
    shiftOut(SER, CLK, MSBFIRST, LedOut1);
    shiftOut(SER, CLK, MSBFIRST, LedOut2);
    digitalWrite(LATCH, HIGH);
    delay(1000);
    LedOut1 = 0;
    LedOut2 = 0;
    digitalWrite(LATCH, LOW);
    shiftOut(SER, CLK, MSBFIRST, 0);
    digitalWrite(LATCH, HIGH);
}

void loop() {
  Shift16Leds();
}
