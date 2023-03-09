// *** 12Hour Format Clock CODED BY LAHIRU SANDARUWAN LIYANAGE ***



#include <Wire.h>
#include "RTClib.h"
const int g_pinData = 4;
const int g_pinCommLatch = 3;
const int g_pinClock = 2;
const int ledPin = 8;
const int GPin = 5;

RTC_DS1307 RTC;

int P1 = A1;  // Button SET MENU'
int P2 = A2;  // Button +
int P3 = A0;  // Button -

int hourupg;
int minupg;
int yearupg;
int monthupg;
int dayupg;
int menu = 0;


byte g_digits[10];

// Current number being displayed
int g_numberToDisplay = 0;

// Number of shift registers in use
const int g_registers = 4;

// Array of numbers to pass to shift registers
byte g_registerArray[g_registers];

void setup() {

  Wire.begin();
  RTC.begin();

  pinMode(P1, INPUT);
  pinMode(P2, INPUT);
  pinMode(P3, INPUT);
  pinMode(GPin, OUTPUT);
  int menu = 0;

  // Only set the time on compile if the RTC is not running...
  if (!RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  pinMode(g_pinCommLatch, OUTPUT);
  pinMode(g_pinClock, OUTPUT);
  pinMode(g_pinData, OUTPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(57600);

  int a = 1, b = 2, c = 4, d = 8, e = 16, f = 32, g = 64;

  g_digits[0] = a + b + c + d + e + f;
  g_digits[1] = b + c;
  g_digits[2] = a + b + g + e + d;
  g_digits[3] = a + b + g + c + d;
  g_digits[4] = f + g + b + c;
  g_digits[5] = a + f + g + c + d;
  g_digits[6] = a + f + g + c + d + e;
  g_digits[7] = a + b + c;
  g_digits[8] = a + b + c + d + e + f + g;
  g_digits[9] = a + b + c + d + g + f;

  int hour, minute, sec;

  DateTime now = RTC.now();

  hour = now.hour();
  minute = now.minute();
  sec = now.second();
  hourupg = hour;
  minupg = minute;
}

void sendSerialData(
  byte registerCount,  // How many shift registers?
  byte *pValueArray)   // Array of bytes with LSByte in array [0]
{
  // Signal to the 595s to listen for data
  digitalWrite(g_pinCommLatch, LOW);

  for (byte reg = registerCount; reg > 0; reg--) {
    byte value = pValueArray[reg - 1];

    for (byte bitMask = 128; bitMask > 0; bitMask >>= 1) {
      digitalWrite(g_pinClock, LOW);
      digitalWrite(g_pinData, value & bitMask ? HIGH : LOW);
      digitalWrite(g_pinClock, HIGH);
    }
  }
  // Signal to the 595s that I'm done sending
  digitalWrite(g_pinCommLatch, HIGH);
}

void loop() {
  int hour, minute, sec, disp = 0;
  digitalWrite(GPin, HIGH);
  DateTime now = RTC.now();

  hour = now.hour();
  minute = now.minute();
  sec = now.second();
  if (menu == 0) {
    hourupg = hour;
    minupg = minute;
    if (hourupg > 12) {
      if (hourupg == 13) hourupg = 1;
      if (hourupg == 14) hourupg = 2;
      if (hourupg == 15) hourupg = 3;
      if (hourupg == 16) hourupg = 4;
      if (hourupg == 17) hourupg = 5;
      if (hourupg == 18) hourupg = 6;
      if (hourupg == 19) hourupg = 7;
      if (hourupg == 20) hourupg = 8;
      if (hourupg == 21) hourupg = 9;
      if (hourupg == 22) hourupg = 10;
      if (hourupg == 23) hourupg = 11;
      if (hourupg == 00) hourupg = 12;
    }




  } else {
  }

  disp = (hourupg * 100) + minupg;
  g_numberToDisplay = disp;

  if (g_numberToDisplay < 10) {
    g_registerArray[0] = g_digits[0];
    g_registerArray[1] = g_digits[0];
    g_registerArray[2] = g_digits[0];
    g_registerArray[3] = g_digits[g_numberToDisplay];
  } else if (g_numberToDisplay < 100) {

    g_registerArray[1] = g_digits[0];
    g_registerArray[2] = g_digits[g_numberToDisplay / 10];
    g_registerArray[3] = g_digits[g_numberToDisplay % 10];
  } else if (g_numberToDisplay < 1000) {

    g_registerArray[1] = g_digits[g_numberToDisplay / 100];
    g_registerArray[2] = g_digits[(g_numberToDisplay % 100) / 10];
    g_registerArray[3] = g_digits[g_numberToDisplay % 10];
  } else {
    g_registerArray[0] = g_digits[g_numberToDisplay / 1000];
    g_registerArray[1] = g_digits[(g_numberToDisplay % 1000) / 100];
    g_registerArray[2] = g_digits[(g_numberToDisplay % 100) / 10];
    g_registerArray[3] = g_digits[g_numberToDisplay % 10];
  }

  sendSerialData(g_registers, g_registerArray);
  // Blink the LED to indicate seconds




  Serial.print(menu);


  if (menu == 1) {
    DisplaySetHour();
  }
  if (menu == 2) {
    DisplaySetMinute();
    Serial.print("min");
  }
  if (menu == 3) {
    StoreAgg();
    delay(500);
    menu = 0;
  }
  if (digitalRead(P1)) {
    menu = menu + 1;
  }
  if (menu == 0) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  } else {
    delay(500);
  }
}  // loop

void DisplayDateTime() {
}

void DisplaySetHour() {

  if (digitalRead(P2) == HIGH) {
    hourupg = hourupg + 1;
  }



  if (digitalRead(P3) == HIGH) {
    if (hourupg == 1) {
      hourupg = 12;
    } else {
      hourupg = hourupg - 1;
    }
  }
}

void DisplaySetMinute() {
  // Setting the minutes

  if (digitalRead(P2) == HIGH) {
    if (minupg == 59) {
      minupg = 0;
    } else {
      minupg = minupg + 1;
    }
  }
  if (digitalRead(P3) == HIGH) {
    if (minupg == 0) {
      minupg = 59;
    } else {
      minupg = minupg - 1;
    }
  }
}

void StoreAgg() {
  RTC.adjust(DateTime(yearupg, monthupg, dayupg, hourupg, minupg, 0));
  delay(200);
  menu = 0;
}
