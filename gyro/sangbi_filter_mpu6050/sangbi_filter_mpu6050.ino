
#include "MPU6050.h"

 
#define pi 3.141592
#define RADIANS_TO_DEGREES 180/3.14159
#define fs 131.0;
MPU6050 mpu;
 
int16_t ax,ay,az;
int16_t gx,gy,gz;
int16_t mx,my,mz;
 
//자이로(각속도)
float gyro_x, gyro_y;
 
//최종 가속도,자이로 각도
float accel_x, accel_y;
float gyro_angle_x=0, gyro_angle_y=0;
 
//상보필터 거친 각도
float angle_x=0,angle_y=0,angle_z=0;
 
//자이로센서 바이어스값
float base_gx=0, base_gy=0, base_gz=0;
 
unsigned long pre_msec=0;
 
void calibrate(){  
   
  int loop =10;
  for (int i=0;i<loop;i++)
  {
    mpu.getMotion9(&ax,&ay,&az,&gx,&gy,&gz,&mx,&my,&mz);
    base_gx += gx;
    base_gy += gy;
    base_gz += gz;
    delay(80);
  }
  
  base_gx /=loop;
  base_gy /=loop;
  base_gz /=loop;
 
}
 
 
void setup() {
  Wire.begin();
 
  Serial.begin(19200);
  mpu.initialize();
  calibrate(); 
}
 
void loop() {
  //단위시간 변화량
  float dt = (millis()-pre_msec)/1000.0;
  pre_msec = millis();
  
  mpu.getMotion9(&ax,&ay,&az,&gx,&gy,&gz,&mx,&my,&mz);
 
  //가속도값 아크탄젠트->각도변환
  accel_x = atan(ay/sqrt(pow(ax,2) + pow(az,2)))*RADIANS_TO_DEGREES;
  accel_y = atan(-1*ax/sqrt(pow(ay,2) + pow(az,2)))*RADIANS_TO_DEGREES;
  
 
  
  //자이로 -32766~+32766을 실제 250degree/s로 변환
  //앞에서 계산한 오차의 평균값을 빼줌 
  gyro_x = (gx-base_gx)/fs;  
  gyro_y = (gy-base_gy)/fs;
 
  //변화량을 적분 
  gyro_angle_x = angle_x + dt*gyro_x;
  gyro_angle_y = angle_y + dt*gyro_y;
 
  //상보필터
  angle_x = 0.95*gyro_angle_x + 0.05*accel_x;
  angle_y = 0.95*gyro_angle_y + 0.05*accel_y;

  Serial.print(angle_y);
  Serial.print("   ");
  Serial.println(angle_x);
  
}

