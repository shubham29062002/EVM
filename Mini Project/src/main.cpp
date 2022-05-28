#include <Arduino.h>
#include <LiquidCrystal.h>

int Secs = 0;

//                RS, En, D4, D5, D6, D7  R/W-Gnd
LiquidCrystal Lcd(12, 11, 5, 4, 3, 2);

void setup()
{
  // put your setup code here, to run once:

  // Initialize LED pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  Lcd.begin(16, 2);

  Lcd.print("hello world!");

  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:

  Lcd.setCursor(0, 1);
  Lcd.print(Secs);
  digitalWrite(LED_BUILTIN, (Secs % 2));
  Serial.println(Secs);
  delay(1000);
  Secs += 1;
}
