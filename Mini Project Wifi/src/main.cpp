#include <Arduino.h>
#include <ESP8266WiFi.h>

#define BUFFER_SIZE 8

void setup()
{
  // put your setup code here, to run once:

  Serial.begin(9600);
  while (!Serial)
  {
    continue;
  }
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
}
