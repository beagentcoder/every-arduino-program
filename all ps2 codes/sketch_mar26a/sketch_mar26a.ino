#include <PS2X_lib.h>  //for v1.6

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you conect the controller,
//or call config_gamepad(pins) again after connecting the controller.
int error = 0;
byte type = 0;
byte vibrate = 0;

int mapped1, mapped2, mapped3, mapped4, mapped5, mapped6, mapped7, mapped8;

/*int dir1 = 40, dir2 = 38, dir3 = 36, dir4 = 34;                                    
int pwm1 = 9, pwm2 = 10, pwm3 = 11 , pwm4 = 12;  
 
*/int dir1 = 38, dir2 = 32, dir3 = 36, dir4 = 34;                                    //
int pwm1 = 9, pwm2 = 8, pwm3 = 10 , pwm4 = 11;  

void setup()
{
  Serial.begin(9600);

  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************

  //error = ps2x.config_gamepad(28, 24, 26, 22, false, false); //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  error = ps2x.config_gamepad(23, 27, 25, 31, false, false); //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  if (error == 0)
  {
    Serial.println("Found Controller, configured successful");
  }

  if (error != 0)         //skip loop if no controller found
  {
    //error = ps2x.config_gamepad(28, 24, 26, 22, false, false);     //2018PCB
    error = ps2x.config_gamepad(23, 27, 25, 31, false, false);       //2019PCB
  }

  type = ps2x.readType();
  switch (type)
  {
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
  pinMode(pwm1, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(pwm4, OUTPUT);
  pinMode(dir4, OUTPUT);
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, 0);
}
void loop() {
  /* 
    if you don't enable the rumble, use ps2x.read_gamepad(); with no values
    you should call this at least once a second
  */

  //  ps2x.read_gamepad();
//  ps2x.reconfig_gamepad();

  if (error == 1) //skip loop if no controller found
    return;
  //  ps2x.read_gamepad();

  else
  { //DualShock Controller

    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed
    
    ps2x.read_gamepad();
//    ps2x.reconfig_gamepad();

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


    vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on
    //how hard you press the blue (X) button

/*    if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
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
*/
    //    if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE

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
      if ((ps2x.Analog(PSS_LY) > 120) && (ps2x.Analog(PSS_LY) < 140) && (ps2x.Analog(PSS_RX) > 120) && (ps2x.Analog(PSS_RX) < 140))
      {
        Serial.println("STOP!");
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
void diagonal_R()
{
  //digitalWrite(dir1, LOW);
  digitalWrite(dir2, HIGH);
  //digitalWrite(dir3, LOW);
  digitalWrite(dir4, LOW);
  
}
void diagonal_L()
{
  digitalWrite(dir1, HIGH);
  //digitalWrite(dir2, HIGH);
  digitalWrite(dir3, LOW);
 //digitalWrite(dir4, HIGH);
}
void clockwise()
{
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, LOW);
}
void anticlockwise()
{
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4, HIGH);
}
