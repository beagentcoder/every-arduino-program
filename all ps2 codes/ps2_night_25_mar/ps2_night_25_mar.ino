#include <PS2X_lib.h>  //for v1.6

PS2X ps2x; // create PS2 Controller Class
int clk = 23;
int cmnd = 27;
int attn = 25;
int data = 31;
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
int vibrate = 255;
int led1 = 33;
int led2 = 29;

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
int type = 0;

int rot_speed = 60;

void setup() {
  Serial.begin(57600);

  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************

  error = ps2x.config_gamepad(clk, cmnd, attn, data, false, false);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  if (error == 0) {
    Serial.println("Connected to PS2 Master");
    Serial.print("   What are you waiting for");
    Serial.println("Let's Go Mongolia");

  }

  else if (error == 1)
    Serial.println("Have Patience !! I am trying Master");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. Please check connections");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  //Serial.print(ps2x.Analog(1), HEX);

  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
  }

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
  //attachInterrupt(digitalPinToInterrupt(outputA), value, CHANGE);

  //alaststate = digitalRead(outputA);

}

void loop() {

  if (error == 1) //skip loop if no controller found
  {
    ps2x.read_gamepad();
    return;
  }
  else
  {
    dont_touch();
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
  }


  delay(50);

}

void dont_touch()
{
  ps2x.read_gamepad(true, vibrate);          //read controller and set large motor to spin at 'vibrate' speed

  if (ps2x.Button(PSB_START))                  //will be TRUE as long as button is pressed
    Serial.println("Start is being held");
  if (ps2x.Button(PSB_SELECT))
    Serial.println("Select is being held");


  if (ps2x.Button(PSB_PAD_UP)) {        //will be TRUE as long as button is pressed
    Serial.print("Up held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
  }
  if (ps2x.Button(PSB_PAD_RIGHT)) {
    Serial.print("Right held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
  }
  if (ps2x.Button(PSB_PAD_LEFT)) {
    Serial.print("LEFT held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
  }
  if (ps2x.Button(PSB_PAD_DOWN)) {
    Serial.print("DOWN held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
  }


  //vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on
  //how hard you press the blue (X) button

  if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
  {



    if (ps2x.Button(PSB_L3))
      Serial.println("L3 pressed");
    if (ps2x.Button(PSB_R3))
      Serial.println("R3 pressed");
    if (ps2x.Button(PSB_L2))
      Serial.println("L2 pressed");
    if (ps2x.Button(PSB_R2))
      Serial.println("R2 pressed");
    if (ps2x.Button(PSB_GREEN))
      Serial.println("Triangle pressed");

  }


  if (ps2x.ButtonPressed(PSB_RED))            //will be TRUE if button was JUST pressed
    Serial.println("Circle just pressed");

  if (ps2x.ButtonReleased(PSB_PINK))            //will be TRUE if button was JUST released
    Serial.println("Square just released");

  if (ps2x.NewButtonState(PSB_BLUE))           //will be TRUE if button was JUST pressed OR released
    Serial.println("X just changed");


  if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE
  {
    Serial.print("Stick Values:");
    Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_LX), DEC);
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_RY), DEC);
    Serial.print(",");
    Serial.println(ps2x.Analog(PSS_RX), DEC);
  }
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
