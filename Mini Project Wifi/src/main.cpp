#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define BUFFER_SIZE 8

#ifndef APSSID
#define APSSID "shubham3"
#define APPSK "29062002"
#endif

const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

void handleRoot();

void setup()
{
  // put your setup code here, to run once:

  Serial.begin(9600);
  while (!Serial)
  {
    continue;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  bool connected = false;
  while (!connected)
  {

    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Error Connecting to Hotspot");
    }
    else
    {
      Serial.println("Connected to AP");
      connected = true;
    }
    delay(500);
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:

  int read = Serial.available();

  if (read > 0)
  {
    char serialData[BUFFER_SIZE];
    Serial.readBytes(serialData, BUFFER_SIZE);

    Serial.println(serialData[0]);
  }

  server.handleClient();
}

void handleRoot()
{
  server.send(200, "text/html", "<h1>You are Connected</h1>");
}
