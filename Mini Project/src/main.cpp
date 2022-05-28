#include <Arduino.h>

void setup()
{
  // put your setup code here, to run once:

  // Initialize LED pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:

  // Set the LED HIGH
  digitalWrite(LED_BUILTIN, HIGH);

  // Wait for a second
  delay(1000);

  // Set the LED LOW
  digitalWrite(LED_BUILTIN, LOW);

  // Wait for a second
  delay(1000);
}
