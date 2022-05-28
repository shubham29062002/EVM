#include <Arduino.h>
#include <LiquidCrystal.h>

int numSwitches = 4;
int Switches[] = {6, 7, 8, 9};
int Counts[] = {0, 0, 0, 0};

//                RS, En, D4, D5, D6, D7  R/W-Gnd
LiquidCrystal Lcd(12, 11, 5, 4, 3, 2);

void PollingAndPrint(void);

void setup()
{
  // put your setup code here, to run once:

  for (int i = 0; i < numSwitches; i++)
  {
    pinMode(Switches[i], INPUT);
  }

  Lcd.begin(16, 2);

  Lcd.print("hello world!");

  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:

  PollingAndPrint();
  delay(500);
}

void PollingAndPrint(void)
{
  Lcd.setCursor(0, 1);
  for (int i = 0; i < numSwitches; i++)
  {
    int reading = digitalRead(Switches[i]);
    Serial.printf("Switch = %d, reading = %d\n", Switches[i], reading);
    if (reading)
    {
      Counts[i] += 1;
      Lcd.print(Counts[i]);
      delay(5000);
    }
    else
    {
      Lcd.print(Counts[i]);
    }
    Serial.println(Counts[i]);
    if (!(i == (numSwitches - 1)))
      Lcd.print(",");
  }
  Serial.println("--------------------------------------------");
  Serial.println("--------------------------------------------");
}
