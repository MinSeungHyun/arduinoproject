 #include "MPU9250.h"
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

#define FIREBASE_HOST "test-d8233.firebaseio.com"
#define FIREBASE_AUTH "rAKwO2dlujcf6L3WaqRzODbubxKqarB1m8QRSM6L"
#define WIFI_SSID "iptime"
#define WIFI_PASSWORD "park0319"

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire, 0x68);

int status;
float accelX, accelY, accelZ;
float gyroX, gyroY, gyroZ;
float magX, magY, magZ;
float angle;
int before_angle;
int skin;
int before_skin;

//--------------------------------------//

void setting() {
  accelX = IMU.getAccelX_mss();
  accelY = IMU.getAccelY_mss();
  accelZ = IMU.getAccelZ_mss();
  gyroX = IMU.getGyroX_rads();
  gyroY = IMU.getGyroY_rads();
  gyroZ = IMU.getGyroZ_rads();
  magX = IMU.getMagX_uT();
  magY = IMU.getMagY_uT();
  magZ = IMU.getMagZ_uT();
}
// 모든 데이터 변수화



void accelPrint() {
  Serial.print("X:");
  Serial.print(accelX);
  Serial.print("   Y:");
  Serial.print(accelY);
  Serial.print("   Z:");
  Serial.println(accelZ);
}
//가속도값 출력 함수



void gyroPrint() {
  Serial.print("X:");
  Serial.print(gyroX);
  Serial.print("   Y:");
  Serial.print(gyroY);
  Serial.print("   Z:");
  Serial.println(gyroZ);
}
//자이로값 출력 함수



void skinCheck() {
  if (-90 < accelZ && accelZ < -9) {
    skin = 1;
  }

  else if (-90 < accelX && accelX < -8) {
    skin = 2;
  }

  else if (9 < accelZ && accelZ < 90) {
    skin = 3;
  }

  else if (9 < accelX && accelX < 90) {
    skin = 4;
  }

  else if (-2 > accelY && accelY > -90) {
    skin = 5;
  }

  else if (8 < accelY && accelY < 90) {
    skin = 6;
  }
  Serial.print("Now skin: ");
  Serial.println(skin);
}
//현재  위쪽을 바라보고있는 면 출력



void spinData() {
  if (skin == 1) {
    if (gyroZ > 1 || gyroZ < -1) {
      angle += gyroZ;
    }
  }

  if (skin == 2) {
    if (gyroX > 1 || gyroX < -1) {
      angle += gyroX;
    }
  }

  if (skin == 3) {
    if (gyroZ > 1 || gyroZ < -1) {
      angle += (gyroZ * -1);
    }
  }

  if (skin == 4) {
    if (gyroX > 1 || gyroX < -1) {
      angle += (gyroX * -1);
    }
  }

  if (skin == 5) {
    if (gyroY > 1 || gyroY < -1) {
      angle += gyroY;
    }
  }

  if (skin == 6) {
    if (gyroY > 1 || gyroY < -1) {
      angle += (gyroY * -1);
    }
  }
  angle = (short)angle;
  Serial.print("angle:");
  Serial.print(angle);
  Serial.print("  ");
  if (before_angle != angle) {
    Firebase.setInt("/angel", angle);
  }
}
// 현재 각도 출력



void resetting() {
  if (before_skin != skin) {
    angle = 0;
    Firebase.setInt("/skin", skin);
    Serial.println("change skin");
  }
}
//면이 바뀔때마다 각도 초기화

//------------------------------------//

void setup() {
  // serial to display data
  Serial.begin(115200);
  // start imu
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1) {}
  }
  // start firebase
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

//------------------------------------//

void loop() {
  IMU.readSensor();
  setting();
  accelPrint();
  /*setting();
  skinCheck();
  resetting();
  spinData();
  before_angle = angle;
  before_skin = skin;*/
}


