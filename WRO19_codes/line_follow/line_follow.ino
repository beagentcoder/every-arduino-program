/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you 
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

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
int RS=37;
int RW=36;
int EN=35;
int DS4=33;
int DS5=32;
int DS6=31;
int DS7=30;
int LineSensorEN = 39; 
int LS1 =A3;
int LS2 =A2;
int LS3 =A1;

LiquidCrystal lcd(RS, EN, DS4, DS5, DS6, DS7);

int L_Speed = 46;
int L_forward = 23;
int L_back = 22;
int R_Speed = 45;
int R_forward = 24;
int R_back = 25;

int LS_Left = 0;
int LS_Center = 0;
int LS_Right  = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  pinMode(RW,OUTPUT);
  pinMode(LineSensorEN,OUTPUT); 
 
  digitalWrite(RW,LOW);
  digitalWrite(LineSensorEN,LOW);

  lcd.begin(16, 2);
  pinMode(L_Speed, OUTPUT);
  pinMode(L_forward, OUTPUT);
  pinMode(L_back, OUTPUT);
  pinMode(R_Speed, OUTPUT);
  pinMode(R_forward, OUTPUT);
  pinMode(R_back, OUTPUT);
  pinMode(L_Speed, OUTPUT);
  pinMode(L_Speed, 255);
  pinMode(R_Speed, 255 );
}

void loop() {
  LS_Left = analogRead(LS1);
  LS_Center = analogRead(LS2);
  LS_Right = analogRead(LS3);
  

  lcd.setCursor(0, 0);
  lcd.print("LS1 = ");
  lcd.print("LS2 = ");
  lcd.print("LS3 = ");
  lcd.setCursor(0, 1);
  lcd.print(analogRead(LS_Left));
  lcd.print(" ");
  lcd.print(analogRead(LS_Center));
  lcd.print(" ");
  lcd.print(analogRead(LS_Right));
  lcd.print(" ");

  if((LS_Center<350)||(LS_Left<350)||(LS_Right<350))
  {
    if(LS_Left<350)
    {
      forward(50,150);
    }
    else if(LS_Right<350)
    {
      forward(150,50);
    }
    else if((LS_Left>350)&&(LS_Right))
    {
      forward(150,150);
    }
   }
   else
   {
    stopp();
   }
  delay(50);
}

void forward(int a, int b)
{
  analogWrite(L_Speed,a);
  analogWrite(L_Speed,b);
  left_forward();
  right_forward();
}
void reverse(int a, int b)
{
  analogWrite(L_Speed,a);
  analogWrite(L_Speed,b); 
  left_reverse();
  right_reverse();
}
void stopp()
{
  left_stop();
  right_stop();
}

void left_forward()
{
  digitalWrite(L_forward, HIGH);
  digitalWrite(L_back, LOW);
}
void left_reverse()
{
  digitalWrite(L_forward, LOW);
  digitalWrite(L_back, HIGH);
}
void left_stop()
{
  digitalWrite(L_forward, HIGH);
  digitalWrite(L_back, HIGH);
}
void right_forward()
{
  digitalWrite(R_forward, HIGH);
  digitalWrite(R_back, LOW);
}
void right_reverse()
{
  digitalWrite(L_forward, LOW);
  digitalWrite(L_back, HIGH);
}
void right_stop()
{
  digitalWrite(L_forward, HIGH);
  digitalWrite(L_back, HIGH);
}

