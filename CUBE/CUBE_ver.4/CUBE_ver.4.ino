#include <MPU6050.h>
#include "I2Cdev.h"
#include "Wire.h"

#include <Thread.h>
#include <ThreadController.h>

#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

#define FIREBASE_HOST "test-d8233.firebaseio.com"
#define FIREBASE_AUTH "rAKwO2dlujcf6L3WaqRzODbubxKqarB1m8QRSM6L"
#define WIFI_SSID "iptime"
#define WIFI_PASSWORD "park0319"
#define sensitivity 1.4

MPU6050 IMU;

int status;
int16_t accelX, accelY, accelZ;
int16_t gyroX, gyroY, gyroZ;
float angle;

short before_angle;
short skin;
short before_skin;

// ThreadController that will controll all threads
ThreadController controll = ThreadController();

//His Thread (not pointer)
Thread fireThread = Thread();


void setting() {
  IMU.getMotion6(accelX*, accelY*, accelZ*, gyroX*, gyroY*, gyroZ*);
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
  if (-12 < accelZ && accelZ < -8) {
    skin = 1;
  }

  else if (-11 < accelX && accelX < -8) {
    skin = 2;
  }

  else if (8 < accelZ && accelZ < 11) {
    skin = 3;
  }

  else if (8 < accelX && accelX < 12) {
    skin = 4;
  }

  else if (-8 > accelY && accelY > -12) {
    skin = 5;
  }

  else if (8 < accelY && accelY < 11) {
    skin = 6;
  }
  Serial.print("Now skin: ");
  Serial.println(skin);
}
//현재  위쪽을 바라보고있는 면 출력



void spinData() {
  if (skin == 1) {
    if (gyroZ > sensitivity || gyroZ < sensitivity * -1) {
      angle += gyroZ;
    }
  }

  if (skin == 2) {
    if (gyroX > sensitivity || gyroX < sensitivity * -1) {
      angle += gyroX;
    }
  }

  if (skin == 3) {
    if (gyroZ > sensitivity || gyroZ < sensitivity * -1) {
      angle += (gyroZ * -1);
    }
  }

  if (skin == 4) {
    if (gyroX > sensitivity || gyroX < sensitivity * -1) {
      angle += (gyroX * -1);
    }
  }

  if (skin == 5) {
    if (gyroY > sensitivity || gyroY < sensitivity * -1) {
      angle += gyroY;
    }
  }

  if (skin == 6) {
    if (gyroY > sensitivity || gyroY < sensitivity * -1) {
      angle += (gyroY * -1);
    }
  }
  angle = (short)angle;
  Serial.print("angle:");
  Serial.print(angle);
  Serial.print("  ");
  /*if (before_angle != angle) {
    Firebase.setInt("/angel", angle);
    }*/
}
// 현재 각도 출력


void resetting() {
  if (before_skin != skin) {
    angle = 0;
    Serial.println("change skin");
  }
}
//면이 바뀔때마다 각도 초기화


// callback for fireThread
void sentFirebase() {

  if (before_skin != skin) {
    Firebase.setInt("/skin", skin);
    before_skin = skin;
  }
  if (before_angle != angle) {
    Firebase.setInt("/angle", angle);
    before_angle = angle;
  }

}

//-------------------------------------------------------------------------------------------//

void setup() {
  Serial.begin(115200);
  //chaek IMU
  IMU.initialize();
  Serial.println("Testing device connections...");
  Serial.println(IMU.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

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

// Configure Thread
fireThread.onRun(sentFirebase);
fireThread.setInterval(200);//스레드 실행 간격

controll.add(&fireThread); // & to pass the pointer to it
}

void loop() {
  // run ThreadController
  // this will check every thread inside ThreadController,
  // if it should run. If yes, he will run it;
  controll.run();// 스레드 실행
  setting();
  skinCheck();
  resetting();
  spinData();
}
