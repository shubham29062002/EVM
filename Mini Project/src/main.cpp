#include <Arduino.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

int numSwitches = 4;
int Switches[] = {8, 9, 10, 11};
int Counts[] = {0, 0, 0, 0};

int RedLED = 12;
int GreenLED = 13;

char PNames[] = {'A', 'B', 'C', 'D'};

//                RS, En, D4, D5, D6, D7  R/W-Gnd
LiquidCrystal Lcd(7, 6, 5, 4, 3, 2);

void PollingAndPrint(void);
void SetLCDCursorByIndex(int Index);
void LedFlash(int Pin);

void setup()
{
  // put your setup code here, to run once:

  for (int i = 0; i < numSwitches; i++)
  {
    pinMode(Switches[i], INPUT);
  }

  pinMode(RedLED, OUTPUT);
  pinMode(GreenLED, OUTPUT);

  digitalWrite(RedLED, LOW);
  digitalWrite(GreenLED, LOW);

  Lcd.begin(16, 2);
  Lcd.print("Initializing");

  Serial.begin(9600);
  EEPROM.begin();
  for (int i = 0; i < numSwitches; i++)
  {
    Counts[i] = EEPROM.read(i);
  }

  delay(2000);

  Lcd.setCursor(0, 0);
  Lcd.clear();
}

void loop()
{
  // put your main code here, to run repeatedly:

  PollingAndPrint();
  delay(200);
}

void PollingAndPrint(void)
{
  Lcd.setCursor(0, 0);

  for (int i = 0; i < numSwitches; i++)
  {
    int reading = digitalRead(Switches[i]);
    if (reading)
    {
      digitalWrite(GreenLED, HIGH);
      digitalWrite(RedLED, LOW);
      for (int c = 0; c < 10; c++)
      {
        for (int j = 0; j < numSwitches; j++)
        {
          if (i == j)
            continue;

          int reading = digitalRead(Switches[j]);
          if (reading)
          {
            digitalWrite(GreenLED, HIGH);
            LedFlash(RedLED);
            return;
          }
        }
        delay(100);
      }

      delay(2000);
      Counts[i] += 1;
      Serial.printf("%c\n", PNames[i]);
      SetLCDCursorByIndex(i * 8);
      Lcd.printf("%c-%d", PNames[i], Counts[i]);
    }
    else
    {
      SetLCDCursorByIndex(i * 8);
      Lcd.printf("%c-%d", PNames[i], Counts[i]);
    }

    EEPROM.write(i, Counts[i]);
  }

  digitalWrite(GreenLED, LOW);
  digitalWrite(RedLED, HIGH);
}

void SetLCDCursorByIndex(int Index)
{
  if (Index < 16)
    Lcd.setCursor(Index, 0);
  else if (Index < 32)
    Lcd.setCursor(Index - 16, 1);
  else
  {
    Serial.println("Error!!!\n");
    Serial.println("Wrong Index for Setting Cursor");
  }
}

void LedFlash(int Pin)
{
  bool On = false;
  for (int i = 0; i < 10; i++)
  {
    if (On)
      digitalWrite(Pin, LOW);
    else
      digitalWrite(Pin, HIGH);

    On = !On;

    delay(100);
  }
}
