#include <Arduino.h>
#include <LiquidCrystal.h>

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

  delay(2000);

  Lcd.setCursor(0, 0);
  Lcd.print("               ");
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
    Serial.printf("Switch = %d, reading = %d\n", Switches[i], reading);
    if (reading)
    {
      Counts[i] += 1;
      SetLCDCursorByIndex(i * 8);
      Lcd.printf("%c-%d", PNames[i], Counts[i]);
      Serial.printf("%c-%d", PNames[i], Counts[i]);
      digitalWrite(GreenLED, HIGH);
      digitalWrite(RedLED, LOW);
      delay(3000);
    }
    else
    {
      SetLCDCursorByIndex(i * 8);
      Lcd.printf("%c-%d", PNames[i], Counts[i]);
      Serial.printf("%c-%d", PNames[i], Counts[i]);
    }
    Serial.println(Counts[i]);
  }
  Serial.println("--------------------------------------------");
  Serial.println("--------------------------------------------");

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
