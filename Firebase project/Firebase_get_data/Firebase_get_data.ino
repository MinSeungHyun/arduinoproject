#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>

#define FIREBASE_HOST "what-is-find-dust.firebaseio.com"
#define FIREBASE_AUTH "2PCrTkrrlx3XzAAcb3nQsehKQ6sXmpan4PD96b2y"
#define WIFI_SSID "iptime"
#define WIFI_PASSWORD "park0319"

int pm10;
String want="-Lzv5mNxFDAXSU07aZEp";

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
     String path ="/pm10"; // 객체의 큰위치를 설정
     FirebaseObject object = Firebase.get(path);  // 지정한 위치의 데이터 값들을 다 불러옴
     pm10 = object.getInt(want); // 지정한 위치에서 원하는 데이터 값을 설정 
     Serial.println(pm10);
     delay(1000);
}
// 동아리 프로젝트를 할때 원하는 데이터값의 이름을 입력하고 그 데이터 값을 불러오는 것으로 설정할것 결론은 want의 값을 사용자가 직접입력할수있게 받으면됨.
