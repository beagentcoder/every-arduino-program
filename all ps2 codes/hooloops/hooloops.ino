// FOR PRACTICAL......FINALIZED.
// PROVIDING SUPPLY TO LED BCZ. IT GIVES AT THAT INSTANT SUPPLY TO CIRCUIT BY WHICH POWER LAGGING WITH JACK IS RESOLVED.

#include <PS2X_lib.h>
PS2X ps2x;
int clk = 23;
int cmnd = 27;
int attn = 25;
int data = 31;
int error;
int mapped1, mapped2, mapped3, mapped4, mapped5, mapped6, mapped7, mapped8;

int dir1 = 38 ;
int pwm1 =  9;
int dir2 = 32 ;
int dir3 = 8 ;
int dir4 =  36;
int pwm2 =  10;
int pwm3 =  34;
int pwm4 =  11;

int led1 =  29;
int led2 = 33;

//#define outputA 2
//#define outputB 3

/*int a = 0;
int dcv1 = 30;
int dcv2 = 32;
int counter = 0;
int astate;
int alaststate;
int dir5 = 10;
//int dir2 = 24;
int pwm5 = 9;
//int pwm2 = 10;

*/
int rot_speed = 60;


void setup() {
  Serial.begin(9600);
  error = ps2x.config_gamepad(clk, cmnd, attn, data, false, false);

  pinMode(dir1, OUTPUT);
  pinMode(pwm1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(dir4, OUTPUT);
  pinMode(pwm4, OUTPUT);
/*  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  pinMode(dcv1, OUTPUT);
  pinMode(dcv2, OUTPUT);

  pinMode(dir5, OUTPUT);
  pinMode(pwm5, OUTPUT);
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  attachInterrupt(digitalPinToInterrupt(outputA), value, CHANGE);

  alaststate = digitalRead(outputA);
*/

}


void loop() {

 
  if (error != 0)         //skip loop if no controller found
  {
    digitalWrite(led1, HIGH);
    return;
  }
  else
    digitalWrite(led2, HIGH);

  ps2x.read_gamepad();
  ps2x.reconfig_gamepad();

  if (ps2x.Button(PSB_L2))
    Serial.println("L2 pressed");
  if (ps2x.Button(PSB_R2))
    Serial.println("R2 pressed");
  if (ps2x.Button(PSB_L3))
    Serial.println("L3 pressed");
  if (ps2x.Button(PSB_R3))
    Serial.println("R3 pressed");
  if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1))
  {
    mapped1 = map(ps2x.Analog(PSS_LY), 120, 0 , 0, 80);
    mapped2 = map(ps2x.Analog(PSS_LY), 140, 255 , 0, 80);
    mapped3 = map(ps2x.Analog(PSS_RX), 120, 0 , 0, 80);
    mapped4 = map(ps2x.Analog(PSS_RX), 140, 255 , 0, 80);

    /*mapped5 = map(ps2x.Analog(PSS_RY), 120, 0 , 0, 140);
      mapped6 = map(ps2x.Analog(PSS_RY), 140, 255 , 0, 140);
      mapped7 = map(ps2x.Analog(PSS_RX), 120, 0 , 0, 140);
      mapped8 = map(ps2x.Analog(PSS_RX), 140, 255 , 0, 140);*/

    if (ps2x.Analog(PSS_LY) <= 120)
    {
      Serial.print("PWM is");
      Serial.println(mapped1);
      backward();
      analogWrite(pwm1, mapped1);
      analogWrite(pwm2, mapped1);
      analogWrite(pwm3, mapped1);
      analogWrite(pwm4, mapped1);
    }
    if (ps2x.Analog(PSS_LY) >= 140)
    {
      Serial.print("PWM is");
      Serial.println(mapped2);
      forward();
      analogWrite(pwm1, mapped2);
      analogWrite(pwm2, mapped2);
      analogWrite(pwm3, mapped2);
      analogWrite(pwm4, mapped2);
    }
    if (ps2x.Analog(PSS_RX) <= 120)
    {
      Serial.print("PWM is");
      Serial.println(mapped3);
      right();
      analogWrite(pwm1, mapped3);
      analogWrite(pwm2, mapped3);
      analogWrite(pwm3, mapped3);
      analogWrite(pwm4, mapped3);

    }
    if (ps2x.Analog(PSS_RX) >= 140)
    {
      Serial.print("PWM is");
      Serial.println(mapped4);
      left();
      analogWrite(pwm1, mapped4);
      analogWrite(pwm2, mapped4);
      analogWrite(pwm3, mapped4);
      analogWrite(pwm4, mapped4);
    }
    if (ps2x.Button(PSB_PAD_LEFT))
    {
      Serial.print("PWM is");
      Serial.println(rot_speed);
      ccw_circle();
      analogWrite(pwm1, rot_speed);
      analogWrite(pwm2, rot_speed);
      analogWrite(pwm3, rot_speed);
      analogWrite(pwm4, rot_speed);
    }
    if (ps2x.Button(PSB_PAD_RIGHT))
    {
      Serial.print("PWM is");
      Serial.println(rot_speed);
      cw_circle();
      analogWrite(pwm1, rot_speed);
      analogWrite(pwm2, rot_speed);
      analogWrite(pwm3, rot_speed);
      analogWrite(pwm4, rot_speed);
    }

    if (ps2x.Button(PSB_PAD_DOWN))
    {
      /*Serial.println("Button Presed now operation will perform");



        digitalWrite(dcv2, HIGH);  //DCV DOWN
        digitalWrite(dcv1, LOW);
        Serial.println("initial");

        delay(1000);

        digitalWrite(dcv2, LOW);  //DCV UP
        digitalWrite(dcv1, HIGH);
        Serial.println("throw");

        delay(150);

        digitalWrite(7, HIGH);
        Serial.println("PIN 7 of arduino A is high");
        delay(300);
        digitalWrite(7, LOW);*/
    }


    /*else if(ps2x.Analog(PSS_RX) >= 140 && ps2x.Analog(PSS_RY) <= 120)
      {
      right_diagonal_up_corner();
      }
      else if(ps2x.Analog(PSS_RY) >= 140 && ps2x.Analog(PSS_RX) <= 120)
      {
      right_diagonal_down_corner();
      }
      else if(ps2x.Analog(PSS_RX) <= 120 && ps2x.Analog(PSS_RY) <= 120)
      {
      left_diagonal_up_corner();
      }
      else if(ps2x.Analog(PSS_RX) >= 140 && ps2x.Analog(PSS_RY) >= 140)
      {
      left_diagonal_down_corner();
      }
      else if(ps2x.Button(PSB_RED))
      {
        cw_circle();
        }
        else if(ps2x.Button(PSB_BLUE))
      {
        ccw_circle();
        }*/
    else if (ps2x.NewButtonState(PSB_BLUE))
    {
      digitalWrite(7, HIGH);
      Serial.println("PIN 7 of arduino A is high");
      delay(100);
      digitalWrite(7, LOW);
    }
    else
    {
      Serial.println("PWM is 0");
      analogWrite(pwm1, 0);
      analogWrite(pwm2, 0);
      analogWrite(pwm3, 0);
      analogWrite(pwm4, 0);
    }
  }
  delay(10);
}
/*
void ccw1_circle()
{
  digitalWrite(dir5, LOW);
  //digitalWrite(dir2, LOW);
  analogWrite(pwm5, 60);
  //analogWrite(pwm2, 255);
}
void stop2()
{
  analogWrite(pwm5, 0);
}
void value()
{
  astate = digitalRead(outputA);
  if (astate != alaststate)
  {
    if (digitalRead(outputB) != astate)
    {
      counter++;
    }
    else {
      counter--;
    }
    //Serial.println("position");
    //Serial.print(counter);
  }
  //Serial.println("position");
  //Serial.print(counter);

  alaststate = astate;

}*/

void ccw_circle()
{
  Serial.println("Moving Anti-ClockWise");
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4, LOW);
  /*analogWrite(pwm1, speed1);
    analogWrite(pwm4, speed1);
    analogWrite(pwm2, speed1);
    analogWrite(pwm3, speed1);*/

}

void cw_circle()
{
  Serial.println("Moving ClockWise");
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, HIGH);
  /*analogWrite(pwm1, speed1);
    analogWrite(pwm4, speed1);
    analogWrite(pwm2, speed1);
    analogWrite(pwm3, speed1);*/
}
void stop1()
{
  Serial.println("Stop");
  /*analogWrite(pwm1, 0);  //.....................................Intially setting pwm = 0
    analogWrite(pwm2, 0);
    analogWrite(pwm3, 0);
    analogWrite(pwm4, 0);*/
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, LOW);

}
void left()
{
  Serial.println("Moving Left");

  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, LOW);

  /*analogWrite(pwm4, speed1);
    analogWrite(pwm3, speed1);
    analogWrite(pwm2, speed1);
    analogWrite(pwm1, speed1);
  */
}
void forward()
{
  Serial.println("Moving Forward");
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, HIGH);
  /*analogWrite(pwm1, speed1);
    analogWrite(pwm2, speed1);
    analogWrite(pwm3, speed1);
    analogWrite(pwm4, speed1);
  */
}


void right()
{
  Serial.println("Moving Left");

  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4, HIGH);

  /*analogWrite(pwm4, speed1);
    analogWrite(pwm3, speed1);
    analogWrite(pwm2, speed1);
    analogWrite(pwm1, speed1);
  */
}
void backward()
{
  Serial.println("Moving Forward");
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4, LOW);
  /*analogWrite(pwm1, speed1);
    analogWrite(pwm2, speed1);
    analogWrite(pwm3, speed1);
    analogWrite(pwm4, speed1);
  */
}
