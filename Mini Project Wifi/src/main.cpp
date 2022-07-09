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

int port = 8888;

WiFiServer server(port);

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
      Serial.print(".");
    }
    else
    {
      Serial.println("Connected to AP");

      Serial.print("WiFi connected to ");
      Serial.println(ssid);
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());

      connected = true;
    }
    delay(500);
  }

  server.begin();
  Serial.print("Open Telnet and connect to IP:");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.println(port);
}

void loop()
{
  // put your main code here, to run repeatedly:

  // int read = Serial.available();

  // if (read > 0)
  // {
  //   char serialData[BUFFER_SIZE];
  //   Serial.readBytes(serialData, BUFFER_SIZE);

  //   Serial.println(serialData[0]);
  // }

  WiFiClient client = server.available();

  if (client)
  {
    if (client.connected())
    {
      Serial.println("Client Connected");
    }

    while (client.connected())
    {
      while (client.available() > 0)
      {
        // read data from the connected client
        Serial.write(client.read());
      }

      while (Serial.available() > 0)
      {
        // Send Data to connected client
        client.write(Serial.read());
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}
