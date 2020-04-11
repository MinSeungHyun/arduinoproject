#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>
  
// Set these to run example.
#define FIREBASE_HOST "what-is-find-dust.firebaseio.com"
#define FIREBASE_AUTH "2PCrTkrrlx3XzAAcb3nQsehKQ6sXmpan4PD96b2y"
#define WIFI_SSID "iptime"
#define WIFI_PASSWORD "park0319"
 
String Sconcentration;
int finddust = A0; // variable resistor connected 
int concentration = 0; // The variable resistor value will be stored in sdata.
 
void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(finddust ,INPUT);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
 
  while (WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(500);
        }
  
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
 
void loop()
{
    concentration = analogRead(finddust);
    Sconcentration = String(concentration); 
    
    Serial.print("Data: ");
    Serial.println(Sconcentration); 
    Firebase.getString("/KR/company/Data/Alcohol", Sconcentration);
    
    Serial.print("finddust: ");
    Serial.println(Sconcentration);
    Firebase.getString("/KR/company/Data/finddust", Sconcentration);z
    
    delay(10000);            
}
