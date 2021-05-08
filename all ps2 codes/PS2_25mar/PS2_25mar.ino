#include <PS2X_lib.h>  //for v1.6

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you conect the controller,
//or call config_gamepad(pins) again after connecting the controller.
int error = 0;
byte type = 0;
byte vibrate = 0;
int led1 = 33;
int led2 = 29;

int clk = 23;
int cmnd = 27;
int attn = 25;
int data = 31;
int mapped1, mapped2, mapped3, mapped4, mapped5, mapped6, mapped7, mapped8;

int dir1 = 40, dir2 = 38, dir3 = 36, dir4 = 34;                                    //
int pwm1 = 9, pwm2 = 10, pwm3 = 11 , pwm4 = 12;                                     //.3
void setup()
{
  Serial.begin(9600);

  error = ps2x.config_gamepad(clk, cmnd, attn, data, false, false); //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

 if (error == 0)
  {
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
   
  }

  if (error == 1)         //skip loop if no controller found
  {
    error = ps2x.config_gamepad(clk, cmnd, attn, data, false, false);
    digitalWrite(led1, HIGH);
    return;
  }
  else
    digitalWrite(led2, HIGH);




  pinMode(pwm1, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir2, OUTPUT);


  pinMode(pwm3, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(pwm4, OUTPUT);
  pinMode(dir4, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, 0);
}
void loop() {


 
  ps2x.read_gamepad();


   if (error = 0)
  {
    //DualShock Controller

    
    //ps2x.read_gamepad();
    //ps2x.reconfig_gamepad();

 
    if (1 == 1)
    {
      mapped1 = map(ps2x.Analog(PSS_LY), 120, 0 , 0, 100);
      mapped2 = map(ps2x.Analog(PSS_LY), 140, 255 , 0, 100);
      mapped3 = map(ps2x.Analog(PSS_RX), 120, 0 , 0, 100);
      mapped4 = map(ps2x.Analog(PSS_RX), 140, 255 , 0, 100);

      /*mapped5 = map(ps2x.Analog(PSS_RY), 120, 0 , 0, 140);
        mapped6 = map(ps2x.Analog(PSS_RY), 140, 255 , 0, 140);
        mapped7 = map(ps2x.Analog(PSS_RX), 120, 0 , 0, 140);
        mapped8 = map(ps2x.Analog(PSS_RX), 140, 255 , 0, 140);*/

      if (ps2x.Analog(PSS_LY) <= 120)
      {
        Serial.print("MOVING FORWARD");
        Serial.print("PWM is");
        Serial.println(mapped1);
        forward();
        analogWrite(pwm1, mapped1);
        analogWrite(pwm2, mapped1);
        analogWrite(pwm3, mapped1);
        analogWrite(pwm4, mapped1);

      }
      if (ps2x.Analog(PSS_LY) >= 140)
      {
        Serial.print("MOVING BACKWARD");
        Serial.print("PWM is");
        Serial.println(mapped2);
        backward();
        analogWrite(pwm1, mapped2);
        analogWrite(pwm2, mapped2);
        analogWrite(pwm3, mapped2);
        analogWrite(pwm4, mapped2);
      }
      if (ps2x.Analog(PSS_RX) <= 120)
      {
        Serial.print("MOVING LEFT");
        Serial.print("PWM is");
        Serial.println(mapped3);
        left();
        analogWrite(pwm1, mapped3);
        analogWrite(pwm2, mapped3);
        analogWrite(pwm3, mapped3);
        analogWrite(pwm4, mapped3);
      }
      if (ps2x.Analog(PSS_RX) >= 140)
      {
        Serial.print("MOVING RIGHT");
        Serial.print("PWM is");
        Serial.println(mapped4);
        right();
        analogWrite(pwm1, mapped4);
        analogWrite(pwm2, mapped4);
        analogWrite(pwm3, mapped4);
        analogWrite(pwm4, mapped4);
      }

      if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1))
      {
        if ((ps2x.Analog(PSS_LY) <= 120) && (ps2x.Analog(PSS_RX) <= 120))
        { 
          Serial.println("moving diagonal left front");
          diagonalleftfront();
          analogWrite(pwm1, mapped1);

          analogWrite(pwm3, mapped1);
        }

        if ((ps2x.Analog(PSS_LY) <= 120) && (ps2x.Analog(PSS_RX) >= 140))
        {
          diagonalrightfront();
          analogWrite(pwm2, mapped1);
            Serial.println("moving diagonal right front");
          analogWrite(pwm4, mapped1);

        }
        if ((ps2x.Analog(PSS_LY) >= 140) && (ps2x.Analog(PSS_RX) >= 140))
        {
          diagonalleftback();
          analogWrite(pwm1, mapped2);
          Serial.println("moving diagonal left back");
          analogWrite(pwm3, mapped2);

        }
        if ((ps2x.Analog(PSS_LY) >= 140) && (ps2x.Analog(PSS_RX) <= 120))
        {
          diagonalrightback();
          analogWrite(pwm2, mapped2);
          Serial.println("moving diagonal right back");
          analogWrite(pwm2, mapped2);

        }
      }

      if (ps2x.Button(PSB_L2))     //motor check
      {
        if (ps2x.Button(PSB_PAD_UP))
        { //will be TRUE as long as button is pressed
          Serial.print("running motor 1 ");
          digitalWrite(pwm1, 60);
        }
        if (ps2x.Button(PSB_PAD_RIGHT))
        {
          Serial.print("running motor 2 ");
          digitalWrite(pwm2, 60);
        }
        if (ps2x.Button(PSB_PAD_LEFT))
        {
          Serial.print("running motor 3 ");
          digitalWrite(pwm3, 60);
        }
        if (ps2x.Button(PSB_PAD_DOWN))
        {
          Serial.print("running motor 4 ");
          digitalWrite(pwm4, 60);
        }
      }

      if (ps2x.Button(PSB_R2))
      {
        if (ps2x.Button(PSB_GREEN))
        {
          clockwise();
          Serial.println("moving clockwise");
        }

        if (ps2x.ButtonPressed(PSB_RED))            //will be TRUE if button was JUST pressed
        {
          Serial.println("moving anticlockwise");
          anticlockwise();
        }


      }


      if ((ps2x.Analog(PSS_LY) > 120) && (ps2x.Analog(PSS_LY) < 140) && (ps2x.Analog(PSS_RX) > 120) && (ps2x.Analog(PSS_RX) < 140))
      {
        Serial.print("STOP!");
        analogWrite(pwm1, 0);
        analogWrite(pwm2, 0);
        analogWrite(pwm3, 0);
        analogWrite(pwm4, 0);
      }



    }
  }

  delay(50);
}
void forward()
{
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4, HIGH);
}
void backward()
{
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, LOW);
}
void left()
{
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4, LOW);
}
void right()
{
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, HIGH);
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
  digitalWrite(dir1, LOW );
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, LOW);
}

void anticlockwise()
{
  digitalWrite(dir1, HIGH );
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4, HIGH);
}
