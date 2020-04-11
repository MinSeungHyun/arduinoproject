#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define FIREBASE_HOST "rotc-library.firebaseio.com"
#define FIREBASE_AUTH "WTJ4LWmEKYZiAH8XliXeXAu3gM1COVZ7MSXGO3p7"
#define WIFI_SSID "iptime"
#define WIFI_PASSWORD "park0319"


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
  Firebase.setString("/100/books/die/Serialcode","a123456789");
  Firebase.setString("/100/books/die/author","Bernard Werber");
  Firebase.setInt("/100/books/die/Book_loan_status",1);
}

