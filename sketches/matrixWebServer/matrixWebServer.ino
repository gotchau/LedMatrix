// Скетч бегущей строки для ESP01
// для матрицы 12х6
// Gotchau 25/12/22

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Fonts/TomThumb.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "your-wifi"
#define STAPSK "password"
#endif

#define PIN 2
#define MAX_TEXT_LENGHT 30
#define CHAR_WIDTH 3

const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(12, 6, PIN,
                                              NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
                                              NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                                              NEO_GRB + NEO_KHZ800);

int x = matrix.width();
int y = matrix.height();
int pass = 0;

String text;
int16_t text_width;

uint32_t now = 0;
uint32_t lastMillis = 0;

void onContent()
{
  text = server.arg("text");
  String color = server.arg("color");

  String r = server.arg("r");
  String g = server.arg("g");
  String b = server.arg("b");

  if (color == "red")
    matrix.setTextColor(matrix.Color(255, 0, 0));
  else if (color == "green")
    matrix.setTextColor(matrix.Color(0, 255, 0));
  else if (color == "blue")
    matrix.setTextColor(matrix.Color(0, 0, 255));
  else if (color == "caef")
    matrix.setTextColor(matrix.Color(202, 239, 0));

  if (r != "" && g != "" && b != "")
  {
    matrix.setTextColor(matrix.Color(r.toInt(), g.toInt(), b.toInt()));
  }

  Serial.println(text);
  server.send(200, "text/plain", text);
}

void matrixInit()
{
  text.reserve(MAX_TEXT_LENGHT);

  matrix.begin();
  matrix.setBrightness(125);
  matrix.setTextWrap(false);
  matrix.setFont(&TomThumb);
  matrix.setTextColor(matrix.Color(202, 239, 0));

  // более правильный способ определения ширины текста
  // к сожалению, нерабочий
  // int16_t d1;
  // uint16_t d2;
  // matrix.getTextBounds(text, 0, 0, &d1, &d1, &text_width, &d2);
}

void serverInit()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  text = WiFi.localIP().toString();

  if (MDNS.begin("esp8266"))
  {
    Serial.println("MDNS responder started");
  }

  server.on("/content", onContent);

  server.begin();
  Serial.println("HTTP server started");
}

void setup(void)
{
  matrixInit();
  serverInit();
}

void display()
{
  now = millis();
  if (now - lastMillis > 100)
  {
    lastMillis = now;

    matrix.fillScreen(0);
    matrix.setCursor(x, y);
    matrix.print(text);

    text_width = text.length() * (CHAR_WIDTH + 1); // символ и пробел

    if (--x < -text_width)
    {
      x = matrix.width();
    }
    matrix.show();
  }
}

void loop(void)
{
  server.handleClient();
  MDNS.update();

  display();
}
