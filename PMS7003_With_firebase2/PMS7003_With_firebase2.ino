#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>

#define FIREBASE_HOST "what-is-find-dust.firebaseio.com"
#define FIREBASE_AUTH "2PCrTkrrlx3XzAAcb3nQsehKQ6sXmpan4PD96b2y"
#define WIFI_SSID "iptime"
#define WIFI_PASSWORD "park0319"

int pm10, pm25;
int sum10 = 0, sum25 = 0;
int score = 0;
unsigned long time1 = 0;
unsigned long time2 = 0;


SoftwareSerial pms(D7, D8);   //아두이노 12번 핀 --> pms7003 TX핀, 아두이노 13번핀 --> pms7003 RX핀

void setup() {
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

  Serial.begin(9600);  //시리얼 출력을 확인하기위한 시리얼통신 시작
  pms.begin(9600);    //pms7003으로부터 데이터를 받기 위한 시리얼통신 시작
}

void loop() {
  
  time1 = millis();
  
  if (time1 - time2 >= 3000 || time2 == 0)
  {
    if (pms.available() >= 32) { //신호 받은 갯수가 32개 이상일때
      int chksum = 0; //체크섬 변수선언
      unsigned char pms_data[32];   //데이터 배열변수 선언
      for (int j = 0; j < 32 ; j++) { //데이터 32개를 받는 반복
        pms_data[j] = pms.read(); //데이터를 받음
      }

      int PM1_0 = (pms[10] << 8)   | pms[11];
      int PM2_5 = (pms[12] << 8)  | pms[13];;
      int PM10 = (pms[14] << 8)  | pms[15];;


      Serial.print("10ug/m^3:");
      Serial.println(PM10);
      Serial.print("2.5ug/m^3:");
      Serial.println(PM25);
      Serial.println("");
      Serial.print("1.0ug/m^3:");
      Serial.println(PM1_0);
      score = getScore();
      Serial.println(score);
      push(score);

    }
  }
}

int getScore() {
  int s = -1;

  if (pm10 >= 151 || pm25 >= 76 ) // 최악
    s = 7;

  else if (pm10 >= 101 || pm25 >= 51 ) // 매우 나쁨
    s = 6;

  else if (pm10 >= 76 || pm25 >= 38 ) // 상당히 나쁨
    s = 5;

  else if (pm10 >= 51 || pm25 >= 26 ) // 나쁨
    s = 4;

  else if (pm10 >= 41 || pm25 >= 21 ) // 보통
    s = 3;

  else if (pm10 >= 31 || pm25 >= 16 ) // 양호
    s = 2;

  else if (pm10 >= 16 || pm25 >= 9 ) // 좋음
    s = 1;

  else // 최고
    s = 0;

  return s;
}

int push(int a) {
  if (a == 0) {
    Firebase.setString("/Finddust", "Awesome");
  }
  else if (a == 1) {
    Firebase.setString("/Finddust", "verygood");
  }
  else if (a == 2) {
    Firebase.setString("/Finddust", "good");
  }
  else if (a == 3) {
    Firebase.setString("/Finddust", "soso");
  }
  else if (a == 4) {
    Firebase.setString("/Finddust", "bed");
  }
  else if (a == 5) {
    Firebase.setString("/Finddust", "Very bed");
  }
  else if (a == 6) {
    Firebase.setString("/Finddust", "good");
  }
  else if (a == 7) {
    Firebase.setString("/Finddust", "terrible");
  }
}
