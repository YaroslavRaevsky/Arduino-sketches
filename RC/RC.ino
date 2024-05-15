//Simple example for receiving
//https://github.com/sui77/rc-switch/
#include <RCSwitch.h>
#define RELE 33 // подключен к пину 13
RCSwitch mySwitch = RCSwitch();

void IRAM_ATTR isr() {
int value = mySwitch.getReceivedValue();
Serial.print( mySwitch.getReceivedValue() );
}

void setup() {
Serial.begin(115200);
pinMode(RELE, OUTPUT);// объявляем пин к которому подключено реле на выход
digitalWrite(RELE, HIGH); // при включении, на цифровой пин отправляем высокий уровень
//attachInterrupt(2, isr, RISING);
mySwitch.enableReceive(digitalPinToInterrupt(4)); // Receiver on interrupt 0 => подключен к пину 2
}
void loop() {
if (mySwitch.available()) {
int value = mySwitch.getReceivedValue();
if (value == 0) {
Serial.print("Unknown encoding");
}
Serial.print("Received ");
Serial.print( mySwitch.getReceivedValue() );
Serial.print(" / ");
Serial.print( mySwitch.getReceivedBitlength() );
Serial.print("bit, ");
Serial.print("Protocol: ");
Serial.println( mySwitch.getReceivedProtocol() );
}
if ((mySwitch.getReceivedValue()== 10437378) || (mySwitch.getReceivedValue()== 16736113))// remote control button| "B"
{
digitalWrite(RELE, HIGH);
}
if ((mySwitch.getReceivedValue()== 10437380) || (mySwitch.getReceivedValue()== 5592368))// button "A" of the
{
digitalWrite(RELE, LOW);
}
mySwitch.resetAvailable();
}
