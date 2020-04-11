#include <ESP8266WiFi.h> // ESP 8266 와이파이 라이브러리
#include <ESP8266HTTPClient.h> // HTTP 클라이언트
#include <Adafruit_NeoPixel.h> // 네오픽셀 라이브러리
#define NUMPIXELS 8 // 네오픽셀 LED 수
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, D2, NEO_GRB + NEO_KHZ800); // 네오픽셀 객체

String sido = "경기"; // 서울, 부산, 대구, 인천, 광주, 대전, 울산, 경기, 강원, 충북, 충남, 전북, 전남, 경북, 경남, 제주, 세종 중 입력
String key = "E1ASY2izLDAG2os%2FpzPRUaEKgAv1xx4mz9xeDxGF5ODpDcVKfJ3LMVS5vrJsg7bwJj%2FZlL7%2BZee6keuTZGxidQ%3D%3D";
String url = "http://openapi.airkorea.or.kr/openapi/services/rest/ArpltnInforInqireSvc/getCtprvnMesureSidoLIst?sidoName=" + sido + "&searchCondition=HOUR&pageNo=1&numOfRows=200&ServiceKey=" + key;

float so2, co, o3, no2, pm10, pm25 = 0; // 대기오염정보 데이터값
int score = 0; // 대기오염점수 0-최고 7-최악
int i=0;
unsigned long nowtime = 0;
unsigned long currenttime = 0;
int motion_sensor = D1;//PIR 핀번호
int motorpin=D3;//릴레이 핀번호

void setup()
{
  pinMode(motorpin,OUTPUT);
  // 시리얼 세팅
  Serial.begin(115200);
  // 네오픽셀 초기화
  pixels.begin();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, 0);
  }
  pixels.show();


  // 와이파이 접속
  WiFi.begin("iptime", "park0319"); // 공유기 이름과 비밀번호 AndroidHotspot2371 030224**

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) // 와이파이 접속하는 동안 "." 출력
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP()); // 접속된 와이파이 주소 출력
}


void loop() {
  currenttime = millis();
  
  if (currenttime - nowtime >= 7200000||i==0)
  {
    nowtime = currenttime;
    if (WiFi.status() == WL_CONNECTED) // 와이파이가 접속되어 있는 경우
    {
      WiFiClient client; // 와이파이 클라이언트 객체
      HTTPClient http; // HTTP 클라이언트 객체

      if (http.begin(client, url)) {  // HTTP
        // 서버에 연결하고 HTTP 헤더 전송
        int httpCode = http.GET();

        // httpCode 가 음수라면 에러
        if (httpCode > 0) { // 에러가 없는 경우
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString(); // 받은 XML 데이터를 String에 저장
            Serial.println(payload);//지역 설정 확인용!! 없어도 됨
            int cityIndex = payload.indexOf("양주시");
            so2 = getNumber(payload, "<so2Value>", cityIndex);
            co = getNumber(payload, "<coValue>", cityIndex);
            o3 = getNumber(payload, "<o3Value>", cityIndex);
            no2 = getNumber(payload, "<no2Value>", cityIndex);
            pm10 = getNumber(payload, "<pm10Value>", cityIndex);
            pm25 = getNumber(payload, "<pm25Value>", cityIndex);
          }

        } else {
          Serial.printf("[HTTP] GET... 실패, 에러코드: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
      } else {
        Serial.printf("[HTTP] 접속 불가\n");
      }
        score = getScore(); //score 변수에 대기오염점수 저장
        Serial.println(score); // 시리얼로 출력
        setLEDColor(score); // 점수에 따라 LED 색상 출력
      
    }
  }
  
  byte state=digitalRead(motion_sensor);
  
  if ( state == HIGH)
  {`
    digitalWrite(motorpin,HIGH);// 1채널 릴레이 ON
    delay(2000);
    digitalWrite(motorpin,LOW);// 1채널 릴레이 OFF
    delay(4000);
  }
  i=1;
}

float getNumber(String str, String tag, int from) {
  int f = str.indexOf(tag, from) + tag.length(); // 테그의 위치와 테그의 문자 길이의 합
  int t = str.indexOf("<", f); // 다음 테스시작위치
  String s = str.substring(f, t); // 테그 사이의 숫자를 문자열에 저장
  return s.toFloat(); // 문자를 소수로 변환 후 반환
}

int getScore() {
  int s = -1;
  if (pm10 >= 151 || pm25 >= 76 || o3 >= 0.38 || no2 >= 1.1 || co >= 32 || so2 > 0.6) // 최악
    s = 7;
  else if (pm10 >= 101 || pm25 >= 51 || o3 >= 0.15 || no2 >= 0.2 || co >= 15 || so2 > 0.15) // 매우 나쁨
    s = 6;
  else if (pm10 >= 76 || pm25 >= 38 || o3 >= 0.12 || no2 >= 0.13 || co >= 12 || so2 > 0.1) // 상당히 나쁨
    s = 5;
  else if (pm10 >= 51 || pm25 >= 26 || o3 >= 0.09 || no2 >= 0.06 || co >= 9 || so2 > 0.05) // 나쁨
    s = 4;
  else if (pm10 >= 41 || pm25 >= 21 || o3 >= 0.06 || no2 >= 0.05 || co >= 5.5 || so2 > 0.04) // 보통
    s = 3;
  else if (pm10 >= 31 || pm25 >= 16 || o3 >= 0.03 || no2 >= 0.03 || co >= 2 || so2 > 0.02) // 양호
    s = 2;
  else if (pm10 >= 16 || pm25 >= 9 || o3 >= 0.02 || no2 >= 0.02 || co >= 1 || so2 > 0.01) // 좋음
    s = 1;
  else // 최고
    s = 0;
  return s;
}

void setLEDColor(int s) {
  int color;
  if (s == 0||s == 1) // 좋음
    color = pixels.Color(0, 63, 255);
  
  else if (s==2 ||s == 3) // 보통
    color = pixels.Color(0, 255, 63);
  
  else if (s == 4 ||s == 5) // 나쁨
    color = pixels.Color(255, 127, 0);
  
  else // 최악
    color = pixels.Color(255, 0, 0);
  
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, color);
  }
  pixels.show();
}
