/*
  LiquidCrystal Library - Hello World

  Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
  library works with all LCD displays that are compatible with the
  Hitachi HD44780 driver. There are many of them out there, and you
  can usually tell them by the 16-pin interface.

  This sketch prints "Hello World!" to the LCD
  and shows the time.

  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

  Library originally added 18 Apr 2008
  by David A. Mellis
  library modified 5 Jul 2009
  by Limor Fried (http://www.ladyada.net)
  example added 9 Jul 2009
  by Tom Igoe
  modified 22 Nov 2010
  by Tom Igoe
  modified 7 Nov 2016
  by Arturo Guadalupi

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int rs = 37, en = 35, d4 = 33, d5 = 32, d6 = 31, d7 = 30, rw = 36;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int linesensorEN = 39;
int ls1 = A3;
int ls2 = A2;
int ls3 = A1;
int IRproxyEN = 6;
int IR1 = A4;
int IR2 = A5;
int IR3 = A6;
int IR4 = A7;
int IR5 = A8;

void setup() {
  pinMode(rw, OUTPUT);
  digitalWrite(rw, LOW);

  pinMode(linesensorEN, OUTPUT);
  digitalWrite(linesensorEN, LOW);

  pinMode(IRproxyEN, OUTPUT);
  digitalWrite(IRproxyEN, LOW);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  

}

void loop() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LS1  ");
  lcd.print("LS2  ");
  lcd.print("LS3  ");
  lcd.setCursor(0, 1);
  lcd.print(analogRead(ls1));
  lcd.print(" ");
  lcd.print(analogRead(ls2));
  lcd.print(" ");
  lcd.print(analogRead(ls3));
  lcd.print(" ");

  delay(1000); 
  
}
