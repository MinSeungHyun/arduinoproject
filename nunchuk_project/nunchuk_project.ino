#include <WiiChuck.h>
#include <Servo.h>

Accessory nunchuck1;
Servo one;
Servo two;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  nunchuck1.begin();
  if (nunchuck1.type == Unknown) {
    nunchuck1.type = NUNCHUCK;
  }
  one.attach(12,0,180);
  two.attach(11,0,180);
  
}

void loop() {
   nunchuck1.readData();    // Read inputs and update maps
    int Servo1Val = map(nunchuck1.values[0],0,255,-22,209);
   int Servo2Val = map( nunchuck1.values[10]>0?0:(nunchuck1.values[11]>0?255:128),0,255,0,180);
    one.write(Servo1Val);
    two.write(Servo2Val);
    Serial.println(nunchuck1.values[10]);
    
    Serial.println(Servo1Val);
 
    Serial.println(Servo2Val);
    Serial.println(" ");
    Serial.println(" ");
}


/*  
    values[1]=map(getJoyX(),0,255,0,255);
    values[2]=map(getJoyY(),0,255,0,255);
    values[3]=map(getRollAngle(),0,1024,0,256);
    values[4]=map(getPitchAngle(),0,1024,0,256);
    values[5]=map(getAccelX(),0,1024,0,256);
    values[6]=map(getAccelY(),0,1024,0,256);

    values[7]=map(getAccelZ(),0,1024,0,256);
    values[8]=0;
    values[9]=0;
    values[10]=0;
    values[11]=getButtonZ()?255:0;
    values[12]=getButtonC()?255:0;
    values[13]=0;
    values[14]=0;

    values[15]=0;
    values[16]=0;
    values[17]=0;

    values[18]=0;
    values[19]=0;
*/
