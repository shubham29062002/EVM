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

#define EVM_ID '1'

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
  WiFiClient client = server.available();

  if (client)
  {
    if (client.connected())
    {
      Serial.println("Client Connected");
      client.write(EVM_ID);
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
        char party;
        Serial.readBytes(&party, 1);
        if (party != '\n')
          client.write(party);
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}
