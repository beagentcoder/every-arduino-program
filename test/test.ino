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

int l_speed = 46;
int l_forward = 23;
int l_back = 22;
int r_speed = 45;
int r_forward = 24;
int r_back = 25;
int sp = 200;

int ls_left = 0;
int ls_centre = 0;
int ls_right  = 0;
void setup() {

  pinMode(rw, OUTPUT);
  digitalWrite(rw, LOW);

  pinMode(linesensorEN, OUTPUT);
  digitalWrite(linesensorEN, LOW);

  pinMode(IRproxyEN, OUTPUT);
  digitalWrite(IRproxyEN, LOW);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // put your setup code here, to run once:
  pinMode(l_speed, OUTPUT);
  pinMode(l_forward, OUTPUT);
  pinMode(l_back, OUTPUT);
  pinMode(r_speed, OUTPUT);
  pinMode(r_forward, OUTPUT);
  pinMode(r_back, OUTPUT);
}

void loop() {
  ls_left = analogRead(ls1);
  ls_centre = analogRead(ls2);
  ls_right = analogRead(ls3);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LS1  ");
  lcd.print("LS2  ");
  lcd.print("LS3  ");
  lcd.setCursor(0, 1);
  lcd.print(ls_left);
  lcd.print(" ");
  lcd.print(  ls_centre);
  lcd.print(" ");
  lcd.print( ls_right);
  lcd.print(" ");
  delay(100);

  if (ls_centre<150)
  {
    if((ls_left>180)&&(ls_right<220))
       {
          forward(200,120);
        }
        else if((ls_right>220)&&(ls_left<180))
        {
          forward(120,200);
        }
        else if((ls_left<180)&&(ls_right<220))
          {
            forward(200,200);
          }
  }
  else 
  stopp();

}
void l_f()
{
  digitalWrite(l_back, LOW);
  digitalWrite(l_forward, HIGH);
}
void l_b()
{
  digitalWrite(l_back, HIGH);
  digitalWrite(l_forward, LOW);
}
void l_stop()
{
  digitalWrite(l_back, HIGH);
  digitalWrite(l_forward, HIGH);
}
void r_f()
{
  digitalWrite(r_back, LOW);
  digitalWrite(r_forward, HIGH);
}
void r_b()
{

  digitalWrite(r_back, HIGH);
  digitalWrite(r_forward, LOW);
}
void r_stop()
{
  digitalWrite(r_back, HIGH);
  digitalWrite(r_forward, HIGH);
}
void forward(int a, int b)
{
  analogWrite(l_speed, a);
  analogWrite(r_speed, b);
  l_f();
  r_f();

}
void back(int a, int b)
{
  analogWrite(l_speed, a);
  analogWrite(r_speed, b);
  r_b();
  l_b();
}
void l_turn(int a, int b)
{
  analogWrite(l_speed, a);
  analogWrite(r_speed, b);
  l_b();
  r_f();
}
void r_turn(int a, int b)
{
  analogWrite(l_speed, a);
  analogWrite(r_speed, b);
  r_b();
  l_f();
}
void stopp()
{
  r_stop();
  l_stop();

}
