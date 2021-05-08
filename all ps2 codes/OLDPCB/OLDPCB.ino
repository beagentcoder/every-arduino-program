// FOR PRACTICAL......FINALIZED.
// PROVIDING SUPPLY TO LED BCZ. IT GIVES AT THAT INSTANT SUPPLY TO CIRCUIT BY WHICH POWER LAGGING WITH JACK IS RESOLVED.

#include <PS2X_lib.h>
PS2X ps2x;
int clk = 28;
int cmnd = 24;
int attn = 26;
int data = 22;
int error;
int mapped1, mapped2, mapped3, mapped4, mapped5, mapped6, mapped7, mapped8;

int dir1 = 34;
int dir2 = 40;
int dir3 = 38;
int dir4 = 36;
int pwm1 = 12;
int pwm2 = 9;
int pwm3 = 10;
int pwm4 =  11;

int led1 = 31;
int led2 = 41;

#define outputA 2
#define outputB 3

int a = 0;
int dcv1 = 30;
int dcv2 = 32;
int counter = 0;
int astate;
int alaststate;
int dir5 = 10;
//int dir2 = 24;
int pwm5 = 9;
//int pwm2 = 10;


int rot_speed = 60;

void setup() {
  Serial.begin(57600);
  error = ps2x.config_gamepad(clk, cmnd, attn, data, false, false);

  pinMode(dir1, OUTPUT);
  pinMode(pwm1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(dir4, OUTPUT);
  pinMode(pwm4, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  pinMode(dcv1, OUTPUT);
  pinMode(dcv2, OUTPUT);
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, 0);


  pinMode(dir5, OUTPUT);
  pinMode(pwm5, OUTPUT);
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  attachInterrupt(digitalPinToInterrupt(outputA), value, CHANGE);

  alaststate = digitalRead(outputA);

}


void loop() {
  if (error == 0)
  {
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
    digitalWrite(led2, HIGH);
    digitalWrite(led1, LOW);
    error = 4;
  }

  if (error == 1)         //skip loop if no controller found
  {
    error = ps2x.config_gamepad(clk, cmnd, attn, data, false, false);
    ps2x.reconfig_gamepad();
    digitalWrite(led1, HIGH);
    return;
  }



  /* if (ps2x.Button(PSB_L2))
     Serial.println("L2 pressed");
    if (ps2x.Button(PSB_R2))
     Serial.println("R2 pressed");
    if (ps2x.Button(PSB_L3))
     Serial.println("L3 pressed");
    if (ps2x.Button(PSB_R3))
     Serial.println("R3 pressed");*/
  if (1 == 1)
  {
    mapped1 = map(ps2x.Analog(PSS_LY), 120, 0 , 0, 80);
    mapped2 = map(ps2x.Analog(PSS_LY), 140, 255 , 0, 80);
    mapped3 = map(ps2x.Analog(PSS_RX), 120, 0 , 0, 80);
    mapped4 = map(ps2x.Analog(PSS_RX), 140, 255 , 0, 80);

    /*mapped5 = map(ps2x.Analog(PSS_RY), 120, 0 , 0, 140);
      mapped6 = map(ps2x.Analog(PSS_RY), 140, 255 , 0, 140);
      mapped7 = map(ps2x.Analog(PSS_RX), 120, 0 , 0, 140);
      mapped8 = map(ps2x.Analog(PSS_RX), 140, 255 , 0, 140);*/

    if ((ps2x.Analog(PSS_LY) <= 120) && (!(ps2x.Button(PSB_R1))))

    {
      Serial.print("PWM is");
      Serial.println(mapped1);
      forward();
      analogWrite(pwm1, mapped1);
      analogWrite(pwm2, mapped1);
      analogWrite(pwm3, mapped1);
      analogWrite(pwm4, mapped1);
    }
    if ((ps2x.Analog(PSS_LY) >= 140) && (!(ps2x.Button(PSB_R1))))
    {
      Serial.print("PWM is");
      Serial.println(mapped2);
      backward();
      analogWrite(pwm1, mapped2);
      analogWrite(pwm2, mapped2);
      analogWrite(pwm3, mapped2);
      analogWrite(pwm4, mapped2);
    }
    if ((ps2x.Analog(PSS_RX) <= 120) && (!(ps2x.Button(PSB_L1))))
    {
      Serial.print("PWM is");
      Serial.println(mapped3);
      left();
      analogWrite(pwm1, mapped3);
      analogWrite(pwm2, mapped3);
      analogWrite(pwm3, mapped3);
      analogWrite(pwm4, mapped3);

    }
    if ((ps2x.Analog(PSS_RX) >= 140) && (!(ps2x.Button(PSB_L1))))
    {
      Serial.print("PWM is");
      Serial.println(mapped4);
      right();
      analogWrite(pwm1, mapped4);
      analogWrite(pwm2, mapped4);
      analogWrite(pwm3, mapped4);
      analogWrite(pwm4, mapped4);
    }
    if (ps2x.Button(PSB_R1))
    {
      if ((ps2x.Analog(PSS_LY) <= 100) && (ps2x.Analog(PSS_LX) <= 100))
      {
        Serial.println("moving diagonal left front");
        diagonalleftfront();
        analogWrite(pwm1, mapped1);

        analogWrite(pwm3, mapped1);
      }
      else if ((ps2x.Analog(PSS_LY) >= 150) && (ps2x.Analog(PSS_LX) <= 100))
      {
        diagonalleftback();
        analogWrite(pwm1, mapped2);
        Serial.println("moving diagonal left back");
        analogWrite(pwm3, mapped2);

      }
      if ((ps2x.Analog(PSS_LY) <= 100) && (ps2x.Analog(PSS_LX) >= 150))
      {
        diagonalrightfront();
        analogWrite(pwm2, mapped1);
        Serial.println("moving diagonal right front");
        analogWrite(pwm4, mapped1);

      }

      if ((ps2x.Analog(PSS_LY) >= 140) && (ps2x.Analog(PSS_LX) >= 150))
      {
        diagonalrightback();
        analogWrite(pwm2, mapped2);
        Serial.println("moving diagonal right back");
        analogWrite(pwm2, mapped2);

      }
    }

    if (ps2x.Button(PSB_L1))
    {
      if ((ps2x.Analog(PSS_RX) >= 140) && (ps2x.Button(PSB_L1)))
      {

        Serial.println(mapped4);
        clockwise();
        analogWrite(pwm1, mapped4);
        analogWrite(pwm2, mapped4);
        analogWrite(pwm3, mapped4);
        analogWrite(pwm4, mapped4);
      }
      if ((ps2x.Analog(PSS_RX) <= 120) && (ps2x.Button(PSB_L1)))
      {

        Serial.println(mapped3);
        anticlockwise();
        analogWrite(pwm1, mapped3);
        analogWrite(pwm2, mapped3);
        analogWrite(pwm3, mapped3);
        analogWrite(pwm4, mapped3);

      }

    }
    if (ps2x.Button(PSB_L2))     //motor check
    {
      if (ps2x.Button(PSB_PAD_UP))
      { //will be TRUE as long as button is pressed
        Serial.println("running motor 1 ");
        digitalWrite(pwm1, 60);
      }
      if (ps2x.Button(PSB_PAD_RIGHT))
      {
        Serial.println("running motor 2 ");
        digitalWrite(pwm2, 60);
      }
      if (ps2x.Button(PSB_PAD_LEFT))
      {
        Serial.println("running motor 4 ");
        digitalWrite(pwm4, 60);
      }
      if (ps2x.Button(PSB_PAD_DOWN))
      {
        Serial.println("running motor 3 ");
        digitalWrite(pwm3, 60);
      }
    }


  }
  else
  {
    Serial.println("PWM is 0");
    analogWrite(pwm1, 0);
    analogWrite(pwm2, 0);
    analogWrite(pwm3, 0);
    analogWrite(pwm4, 0);
  }

  delay(10);
}
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

}

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

  digitalWrite(dir1, LOW);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, HIGH);
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
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4, HIGH);
  /*analogWrite(pwm1, speed1);
    analogWrite(pwm2, speed1);
    analogWrite(pwm3, speed1);
    analogWrite(pwm4, speed1);
  */
}


void right()
{
  Serial.println("Moving right");

  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, HIGH);

  /*analogWrite(pwm4, speed1);
    analogWrite(pwm3, speed1);
    analogWrite(pwm2, speed1);
    analogWrite(pwm1, speed1);
  */
}
void backward()
{
  Serial.println("Moving backward");
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, LOW);
  /*analogWrite(pwm1, speed1);
    analogWrite(pwm2, speed1);
    analogWrite(pwm3, speed1);
    analogWrite(pwm4, speed1);
  */
}



void diagonalrightfront()
{

  digitalWrite(dir2, LOW);
  digitalWrite(dir4, HIGH);
}

void diagonalleftfront()
{

  digitalWrite(dir1, LOW);
  digitalWrite(dir3, HIGH);

}



void diagonalrightback()
{

  digitalWrite(dir2, HIGH);
  digitalWrite(dir4, LOW);
}

void diagonalleftback()
{

  digitalWrite(dir1, HIGH);
  digitalWrite(dir3, LOW);

}
void clockwise()
{
  Serial.println("moving clockwise");
  digitalWrite(dir1, LOW );
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, LOW);
}

void anticlockwise()
{
  Serial.println("moving anticlockwise");
  digitalWrite(dir1, HIGH );
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4, HIGH);
}
