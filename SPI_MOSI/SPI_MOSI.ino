#include <SPI.h>

void setup() {
  pinMode(10, OUTPUT); // set the SS pin as an output
  SPI.begin();         // initialize the SPI library
}

void loop() {
 // SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
  digitalWrite(10, LOW);            // set the SS pin to LOW
  int value = analogRead(35);
 // short val_hex_first = 0xFF;
 // short val_hex_second = 0x00;
  short val_hex = 0x0000;
 // val_hex++;
  for (int i=1; i<=value;i++)
  {
    val_hex = val_hex + 0x0001;
  }
 SPI.transfer16(val_hex);
 delay(10); 
 digitalWrite(10, HIGH);           // set the SS pin HIGH

}