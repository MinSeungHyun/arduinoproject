#include <SoftwareSerial.h>

SoftwareSerial HM10(3, 2); // Arduino의 RXD, TXD

void setup()  
{
  Serial.begin(9600);
  HM10.begin(9600);    // Bluetooth는 data rate을 115200으로 설정해야 함
}

void loop()
{
 if(HM10.available())
    Serial.write(HM10.read());
 
 if(Serial.available())
    HM10.write(Serial.read());
}
