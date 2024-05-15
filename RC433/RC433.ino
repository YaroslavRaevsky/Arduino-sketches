#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);
  mySwitch.enableReceive(digitalPinToInterrupt(4));//connect the module pin to GPIO4
}

void loop() {
  if (mySwitch.available()) {
    Serial.print("Received ");
   Serial.println(mySwitch.getReceivedValue());//, mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
   Serial.print(" / ");
   Serial.print(mySwitch.getReceivedBitlength());//, mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
   Serial.print("bit, ");
   Serial.print(mySwitch.getReceivedDelay());//, mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
   //Serial.print("Raw ");
  // Serial.println(mySwitch.getReceivedRawdata());//, mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
   Serial.print("Protocol ");
   Serial.println(mySwitch.getReceivedProtocol());//, mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
    mySwitch.resetAvailable();
  }
}