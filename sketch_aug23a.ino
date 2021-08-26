#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

WiFiClient client;
char lineEnd = '\r';
char newLine = '\n';

void setup() {
  Serial.begin(74880);
  Serial.println(F("start"));

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  WiFi.begin("XXXXX", "XXXXX");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");

  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());



}


void loop() {

  String content = getData();

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, content);

  if(doc["status"] != "ok" ) {

    // DISPALY ERROR
    displayData(doc["status"], "sorry", "data", "is", "missing :(");

  } else {

    String newDate = doc["updated"];

    String data0 = doc["priceData"][0]["t"] + ": " + String(doc["priceData"][0]["p"]);
    String data1 = doc["priceData"][1]["t"] + ": " + String(doc["priceData"][1]["p"]);
    String data2 = doc["priceData"][2]["t"] + ": " + String(doc["priceData"][2]["p"]);
    String data3 = doc["priceData"][3]["t"] + ": " + String(doc["priceData"][3]["p"]);


    // NOW DISPLAY
    displayData(newDate, data0, data1, data2, data3);

  }

  // WAIT A MINUTE!
  delay(60000);

}

String getData(){

  Serial.println("getData connect");

  int counter = 0;                  // count payload bytes
  int maxPayloadSize = 1200;        // max bytes of payload

  String oneLine = "";              // maybe you need something from header
  int oneLineLen = 1;               // if there's very short line, next comes content

  bool contentStarts = false;       // if true, chars go to content
  String content = "";              // the real stuff we're after
  int contentLen = maxPayloadSize;  // read from header, the initial value is quite random
  int contentCounter = 0;           // to know when content ends


  if (client.connect("172.16.1.13", 3335)) {
    client.flush();

    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /test HTTP/1.1");
    client.println();
    client.println();

    delay(3200);

    while (char c = client.read()) {

      if(contentStarts) {
        content += String(c);
        contentCounter ++;
      }

      // collect chars
      if (isAscii(c)) {  // tests if myChar is an Ascii character
        //Serial.print(String(c));
        oneLine += String(c);
        oneLineLen ++;

        if(c == newLine) {
          //Serial.println("lineStart>");
          // in case we need some headers
          if(oneLine.indexOf("Content-Length") >= 0) {
            String headerValue = oneLine.substring(oneLine.indexOf(":") + 1, oneLine.length());
            contentLen = headerValue.toInt();
          }

          oneLine = "";
        }
        if(c == lineEnd) {
          //Serial.println("<lineEnd: " + String(oneLineLen));
          if(oneLineLen < 4) {
            contentStarts = true;
          }
          oneLineLen = 0;
        }

      } else {
        //Serial.print(".");

      }

      counter ++;
      if(counter > maxPayloadSize || contentCounter > contentLen) {
        //Serial.println("counter " + String(counter) + " > maxPayloadSize " + String(maxPayloadSize));
        //Serial.println("contentCounter " + String(contentCounter) + " >= contentLen" + String(contentLen));
        break;
      }


    }

  }

  client.stop();
  return content;

}


void displayData(String newDate, String data0, String data1, String data2, String data3) {

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(32, 16);
  display.print(newDate);

  display.setCursor(32, 26);
  display.print(data0);

  display.setCursor(32, 36);
  display.print(data1);

  display.setCursor(32, 46);
  display.print(data2);

  display.setCursor(32, 56);
  display.print(data3);

  display.display();
}
