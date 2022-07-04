#include <Arduino.h>

int pin1 = 2;
int pin2 = 16;

void setup()
{
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:

  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  delay(1000);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  delay(1000);

  Serial.println("Doing Something");
}
