#define outputA 21
#define outputB 20
int counter = 0;
int astate;
int alaststate;

int pwm1 = 13;         // GRIPPER
int dir1 = 30 ;

int pwm2 = 10;
int dir2 = 28;        // SPRING MOTOR

int pwm3 = 11;                   //rotator
int dir3 = 34 ;

int dcv1 = 52;
int dcv2 = 53;     //piston

int testing = 0;
int grip = 1;
int c1 = 27, c2 = 25, c3 = 23;
int a1 = 0, a2 = 0, a3 = 0;

float kp = 5, kd = 3.2;

int p = 0 , d = 0 , pid = 0, setpoint = 90;
int error, last_error;

int flag = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(pwm1, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(dcv1, OUTPUT);
  pinMode(dcv2, OUTPUT);
  pinMode(c1, INPUT);
  pinMode(c2, INPUT);
  pinMode(c3, INPUT);
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  attachInterrupt(digitalPinToInterrupt(outputA), value, CHANGE);

  alaststate = digitalRead(outputA);



  analogWrite(pwm1, 0);
  digitalWrite(dir1, LOW);
  analogWrite(pwm2, 0);
  digitalWrite(dir2, LOW);
  analogWrite(pwm3, 0);
  digitalWrite(dir3, LOW);

  digitalWrite(dcv1, HIGH);
  digitalWrite(dcv2, LOW);


  attachInterrupt(digitalPinToInterrupt(c1), value, CHANGE);
  attachInterrupt(digitalPinToInterrupt(c2), value, CHANGE);
  attachInterrupt(digitalPinToInterrupt(c3), value, CHANGE);

  // put your setup code here, to run once:


}



void loop()
{
  Serial.println(counter);
  Serial.println("counter");

  error = setpoint - counter;
  last_error = error;
  pid_encoder();

  stopsetpt();


  /* if (digitalRead(c1) == HIGH)
    {
     if (a1 == 0)
       Serial.println("c1 is high");

     a1 = a1 + 1;
     pick();


    }
    if (flag == 1)
    {

     if (counter > (-100))
     {
       analogWrite(pwm3, 80);
       digitalWrite(dir3, HIGH);
       Serial.println("moving down");
     }
     else if (counter <= (-100))
     {
       analogWrite(pwm3, 0);
       Serial.println("        stoppppp         ");


     }
    }
    if (flag == 2) {




     if (counter < (-120))
     {
       analogWrite(pwm3, 150);
       digitalWrite(dir3, LOW);
       Serial.println("going up");
     }
     else
     {
       analogWrite(pwm3, 0);
       delay(50);
       analogWrite(pwm3, 50);
       delay(50);
       analogWrite(pwm3, 0);
       flag = 3;
     }


    }
    if (digitalRead(c2) == HIGH)
    {

     if (a2 == 0)
     { Serial.println("c2 is high");


       a2 = a2 + 1;
       place();

     }
    }
    if (digitalRead(c3) == HIGH)
    {
     if (a3 == 0)
       Serial.println("c3 is high");
     throww();
     a3 = a3 + 1;

    }
    //else Serial.println("Firse");*/

}
void pick()
{
  digitalWrite(dcv1, HIGH);  //piston open
  digitalWrite(dcv2, LOW);
  delay(500);
  //spring extends here
  analogWrite(pwm2, 220);
  digitalWrite(dir2, HIGH);
  delay(1200);
  digitalWrite(dcv1, LOW);   //piston lock
  digitalWrite(dcv2, HIGH);
  analogWrite(pwm2, 0);
  delay(500);



  analogWrite(pwm2, 240);    //opp dir
  digitalWrite(dir2, LOW);
  delay(400);
  analogWrite(pwm2, 0);
  delay(400);


  // code for gripper ........................
  analogWrite(pwm1, 60);
  digitalWrite(dir1, HIGH );
  delay(200);//................cmnt
  analogWrite(pwm1, 0);

  flag = 1;



}
void place()
{
  // code for placing.........................
  analogWrite(pwm1, 255);
  digitalWrite(dir1, LOW);
  Serial.println("closing");
  delay(200);

  flag = 2;



}




void throww()
{

  analogWrite(pwm1, 0);
  delay(150);
  analogWrite(pwm1, 60);
  digitalWrite(dir1, HIGH );
  delay(150);//................cmnt
  analogWrite(pwm1, 0);
  analogWrite(pwm3, 100);
  digitalWrite(dir3, HIGH);
  delay(150);
  analogWrite(pwm3, 0);

  digitalWrite(dcv1, HIGH);  //piston open
  digitalWrite(dcv2, LOW);
  //delay(400);............cmnt
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

void pid_encoder()
{
  if (error > 0)
  {
    p = error * kp;
    d = last_error * kd;
    pid = (p + d);

  }
  if (error < 0)
  {
    p = (-error) * kp;
    d = abs(error - last_error) * kd;
    pid = (p + d);
  }

}

void stopsetpt()
{
  if (error > 1)
  {
    analogWrite(pwm3, pid);
    digitalWrite(dir3, HIGH);
    setpoint = setpoint - 10;
  }
  if (error < -1)
  {


    analogWrite(pwm3, pid);
    digitalWrite(dir3, LOW);
  }
  if ((error >= -1) && (error <= 1))
  {
    analogWrite(pwm3, pid);

  }
}

void place2()
{
  
  }
