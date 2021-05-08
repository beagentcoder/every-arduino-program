#include<LiquidCrystal.h>

int RS = 37;
int RW = 36;
int EN = 35;
int DS4 = 33;
int DS5 = 32;
int DS6 = 31;
int DS7 = 30;

LiquidCrystal lcd(RS, EN, DS4, DS5, DS6, DS7);


int L_speed = 46;
int L_forward = 23;
int L_back = 22;
int R_speed = 45;
int R_forward = 24;
int R_back = 25;

void setup()
{
  pinMode(RW, OUTPUT);
  pinMode(RW, LOW);
  lcd.begin(16, 2);

  pinMode(L_speed, OUTPUT);
  pinMode(L_forward, OUTPUT);
  pinMode(L_back, OUTPUT);
  pinMode(R_speed, OUTPUT);
  pinMode(R_forward, OUTPUT);
  pinMode(R_back, OUTPUT);
  pinMode(L_speed, OUTPUT);
  pinMode(L_speed, 255);
  pinMode(R_speed, 255 );

}

void loop()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  forward();
  delay(2000);

  stopp();
  delay(2000);

  reverse();
  delay(2000);

  stopp();
  delay(2000);

}
void forward()
{
  lcd.setCursor(0, 0);
  lcd.print("chaleg");
  left_forward();
  right_forward();
}
void reverse()
{
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

