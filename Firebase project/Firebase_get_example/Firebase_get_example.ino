#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>

#define FIREBASE_HOST "  "
#define FIREBASE_AUTH "  "
#define WIFI_SSID "  "
#define WIFI_PASSWORD "  "

int data;
String want="  "; 원하는   데이터   이름
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
     String path =" "; // 목표로하는 객체를 포함하는 큰객체의 위치를 설정
     FirebaseObject object = Firebase.get(path);  // 지정한 위치의 데이터 값들을 다 불러옴
     data = object.getInt(want); // 지정한 위치에서 원하는 데이터 값을 설정 
     delay(1000);
}

