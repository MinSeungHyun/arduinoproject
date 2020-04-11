#include "PMS.h"
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>

#define FIREBASE_HOST "what-is-find-dust.firebaseio.com"
#define FIREBASE_AUTH "2PCrTkrrlx3XzAAcb3nQsehKQ6sXmpan4PD96b2y"
#define WIFI_SSID "iptime"
#define WIFI_PASSWORD "park0319"

PMS pms(Serial);
PMS::DATA data;

int pm10 = 0;
int pm25 = 0;
int score = 0;
unsigned long time1 = 0;
unsigned long time2 = 0;

void setup()
{
  // Debug console
  Serial.begin(9600);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
    if (pms.readUntil(data))
    {
      pm10 = data.PM_AE_UG_1_0;
      pm25 = data.PM_AE_UG_2_5;
      Serial.println(pm10);
      Serial.println(pm25);
      Serial.println( );
      score = getScore();
      Serial.println(score);
      push(score);
    }
}

int getScore() {
  int s = -1;
  if (pm10 >= 151 || pm25 >= 76 ) // 최악
    s = 4;
  else if (pm10 >= 81 || pm25 >= 36 ) // 나쁨
    s = 3;
  else if (pm10 >=31  || pm25 >= 17 ) // 보통
    s = 2;
  else if (pm10 >= 0 || pm25 >= 0 ) // 좋음
    s = 1;
    
  return s;
}

int push(int a) {
    
  if (a == 1) {
    Firebase.setString("/Finddust", "verygood");
    Firebase.setInt("/pm10", pm10);
    Firebase.setInt("/pm25", pm25);
  }

  else if (a == 2) {
    Firebase.setString("/Finddust", "soso");
    Firebase.setInt("/pm10", pm10);
    Firebase.setInt("/pm25", pm25);
  }
  else if (a == 3) {
    Firebase.setString("/Finddust", "bed");
    Firebase.pushInt("/pm10", pm10);
    Firebase.pushInt("/pm25", pm25);
  }
  else if (a == 4) {
    Firebase.pushString("/Finddust", "terrible");
    Firebase.pushInt("/pm10", pm10);
    Firebase.pushInt("/pm25", pm25);
  }
}

