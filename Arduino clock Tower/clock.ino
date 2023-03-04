// Code  by Lahiru Sandaruwan
// DS3231.h - http://www.rinkydinkelectronics.com/library.php?id=73


#include "SevSeg.h"
#include <DS3231.h>
DS3231  rtc(SDA, SCL);
Time  t;
SevSeg Display;
const int hrs_set = A0;
const int min_set = A1;
const int ledPin =  A3;
unsigned int number = 0;
const long interval = 500;
unsigned long startMillis;
unsigned long currentMillis;
unsigned long previousMillis = 0;
unsigned int Hour = 0;
unsigned int hrs_var = 0;
unsigned int min_var = 0;
int ledState = LOW;

void setup()
{
  rtc.begin();
  pinMode(ledPin, OUTPUT);
  pinMode(hrs_set, INPUT_PULLUP);
  pinMode(min_set, INPUT_PULLUP);
  byte numDigits = 4;
  byte digitPins[] = {12, 11, 10, 9};
  byte segmentPins[] = {13, 3, 8, 5, 4, 2, 7, 6};
  bool resistorsOnSegments = false;
  bool updateWithDelays = false;
  byte hardwareConfig = COMMON_CATHODE;
  bool leadingZeros = true;
  bool disableDecPoint = true;
  Display.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  Display.setBrightness(100);
}

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    if (ledState == LOW)
    {
      ledState = HIGH;
    }
    else
    {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);
  }
  t = rtc.getTime();
  Hour = t.hour;
  hrs_var = t.hour;
  min_var = t.min;
  if (t.hour > 12)
  {
    if (t.hour == 13) Hour = 1;
    if (t.hour == 14) Hour = 2;
    if (t.hour == 15) Hour = 3;
    if (t.hour == 16) Hour = 4;
    if (t.hour == 17) Hour = 5;
    if (t.hour == 18) Hour = 6;
    if (t.hour == 19) Hour = 7;
    if (t.hour == 20) Hour = 8;
    if (t.hour == 21) Hour = 9;
    if (t.hour == 22) Hour = 10;
    if (t.hour == 23) Hour = 11;
  }
  else
  {
    if (t.hour == 0) Hour = 12;
  }
  number = Hour * 100 + t.min;
  Display.setNumber(number);
  Display.refreshDisplay();
  if (digitalRead(hrs_set) == LOW)
  {
    hrs_var += 1;
    if (hrs_var > 23) hrs_var = 0;
    rtc.setTime(hrs_var, min_var, 0);
    for (int i = 0; i < 1000; i ++)
    {
      Display.setNumber(number);
      Display.refreshDisplay();
    }
  }
  if (digitalRead(min_set) == LOW)
  {
    min_var += 1;
    if (min_var >= 60) min_var = 0;
    rtc.setTime(hrs_var, min_var, 0);
    for (int i = 0; i < 1000; i ++)
    {
      Display.setNumber(number);
      Display.refreshDisplay();
    }
  }
}
