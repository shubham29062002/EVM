#include <Arduino.h>
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
    // String serialData;
    // serialData.reserve(BUFFER_SIZE);

    char serialData[BUFFER_SIZE];
    Serial.readBytes(serialData, BUFFER_SIZE);

    Serial.printf("%c\n", serialData[0]);

    // byte x = Serial.read();
    // Serial.print((char *)x);
  }
}
