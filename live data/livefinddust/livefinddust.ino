#include "PMS.h"
#include <ESP8266WiFi.h>

#define WIFI_SSID "iptime"
#define WIFI_PASSWORD "park0319"

WiFiServer server(80);

PMS pms(Serial);
PMS::DATA data;

int pm10 = 0;
int pm25 = 0;
int reqCount = 0;       

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

  server.begin();
}

void loop(){
    WiFiClient client = server.available();

  delay(500);
  
    if (client) {
    Serial.println("New client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          Serial.println("Sending response");

              if(pms.readUntil(data))
              {
                pm10 = data.PM_AE_UG_1_0;
                pm25 = data.PM_AE_UG_2_5;
                Serial.print("pm10: ");
                Serial.println(pm10);
                Serial.print("pm25: ");
                Serial.println(pm25);
              }
          // send a standard http response header
          // use \r\n instead of many println statements to speedup data send
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"  // the connection will be closed after completion of the response
            "Refresh: 20\r\n"        // refresh the page automatically every 20 sec
            "\r\n");
          client.print("<!DOCTYPE HTML>\r\n");
          client.print("<html>\r\n");
          client.print("Requests received: ");
          client.print(++reqCount);
          client.print("<br>\r\n");
          client.print("Finddust 1.0: ");
          client.print(data.PM_AE_UG_1_0);
          client.print("<br>\r\n");
          client.print("Finddust 2.0: ");
          client.print(data.PM_AE_UG_2_5);
          client.print("<br>\r\n");
          client.print("</html>\r\n");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1000);

    // close the connection:
    client.stop();
    Serial.println("Client disconnected");
  }


}





