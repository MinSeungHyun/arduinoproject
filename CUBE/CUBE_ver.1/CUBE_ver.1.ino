#include <MPU9250.h>
/*#include <FirebaseArduino.h>
  #include <ArduinoJson.h>
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <SoftwareSerial.h>

  #define FIREBASE_HOST "test-d8233.firebaseio.com"
  #define FIREBASE_AUTH "rAKwO2dlujcf6L3WaqRzODbubxKqarB1m8QRSM6L"
  #define WIFI_SSID "iptime"
  #define WIFI_PASSWORD "park0319"*/

MPU9250 mpu;


int Xturn, Yturn, Zturn;
int upside, before_upside;
uint8_t i;

int Yaw() {
  //Zturn = map(mpu.getYaw(),-179,179,1,8);
  Zturn = mpu.getYaw();
  Serial.print("yaw  : ");
  Serial.print(Zturn);
  //Firebase.setInt("/Yaw", Zturn);
}

int Roll() {
  Xturn = mpu.getRoll();
  Serial.print("   Roll  : ");
  Serial.print(Xturn);
  //Firebase.setInt("/Roll", Xturn);
}

int Pitch() {
  Yturn = mpu.getPitch();
  Serial.print("  Pitch  : ");
  Serial.println(Yturn);
  //Firebase.setInt("/Pitch", Yturn);
}

int side() {
  if ((-15 < Xturn && Xturn < 5) && (-15 < Yturn && Yturn < 10)) {
    Serial.print("Now side:");
    Serial.print("1  ");
    upside = 1;
  }
  else if ((-105 < Xturn && Xturn < -82) && (-20 < Yturn && Yturn < 5)) {
    Serial.print("Now side:");
    Serial.print("2  ");
    upside = 2;
  }
  else if ((160 < Xturn || Xturn < -165) && (-15 < Yturn && Yturn < 18)) {
    Serial.print("Now side:");
    Serial.print("3  ");
    upside = 3;
  }
  else if ((80 < Xturn && Xturn < 115) && (-11 < Yturn && Yturn < 7)) {
    Serial.print("Now side:");
    Serial.print("4  ");
    upside = 4;
  }
  else if (((0 < Xturn && Xturn < 180) || (-180 < Xturn && Xturn < 0)) && (-89 < Yturn && Yturn < -65)) {
    Serial.print("Now side:");
    Serial.print("5  ");
    upside = 5;
  }
  else if (((0 < Xturn && Xturn < 180) || (-180 < Xturn && Xturn < 0)) && (64 < Yturn && Yturn < 92)) {
    Serial.print("Now side:");
    Serial.print("6  ");
    upside = 6;
  }

}
//---------------- -funtion--finish------------------ -//

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  delay(1000);
  mpu.setup();
  /*WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    }

    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);*/
}

void loop()
{
  static uint32_t prev_ms = millis();

  if ((millis() - prev_ms) > 16)
  {
    mpu.update();
    //mpu.print();
    //Serial.print(mpu.getGyro(i));
    Yaw();
    Roll();
    Pitch();
    side();
    /*if(before_upside!=upside){
       Firebase.setInt("/Nowside",upside);
       Serial.print("Sent_data success: ");
       Serial.print(upside);
       Serial.print("  ");
      }*/
    prev_ms = millis();
  }
  before_upside = upside;
}

