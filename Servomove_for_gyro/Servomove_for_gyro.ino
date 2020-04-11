#include <MPU9250.h>
#include <Wire.h>
#include <Servo.h>
int Xturn, Zturn;
Servo X;
Servo Z;
MPU9250 mpu;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  delay(2000);
  mpu.setup();
  X.attach(12, 0, 180);
  Z.attach(11, 0, 180);
}

void loop() {
  static uint32_t ms = millis();

  if ((millis() - ms) > 10)
  {
    mpu.update();
    mpu.print();
    Serial.print("Z-data  :  ");
    Zturn = map(mpu.getRoll(), -500, 500, -40, 250);
    Serial.println(Zturn);

    Serial.print("X-data  :  ");
    Xturn = map(mpu.getYaw(), -200, 200, -70, 215);
    Serial.println(Xturn);
    ms = millis();
  }

  X.write(Xturn);
  Z.write(Zturn);
}
