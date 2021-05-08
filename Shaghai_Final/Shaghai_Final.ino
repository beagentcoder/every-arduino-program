#define outputA 21
#define outputB 20
int counter = 0;
int astate;
int alaststate;

int gripp = 13;               // GRIPPER
int gripd = 30 ;

int pullpwm = 10;
int pulldir = 28;         // SPRING MOTOR

int placep = 11;        //rotator
int placed = 34 ;

int dcv1 = 52;
int dcv2 = 53;       //piston

int testing = 0;
int grip = 1;
int c1 = 27, c2 = 25, c3 = 23;
int a1 = 0, a2 = 0, a3 = 0;

float kp = 2, kd = 0;

int p = 0 , d = 0 , pid = 0, pid1 = 0, setpoint = 90;
int error, last_error;

int flag = 0;
int c = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(gripp, OUTPUT);
  pinMode(gripd, OUTPUT);
  pinMode(pullpwm, OUTPUT);
  pinMode(pulldir, OUTPUT);
  pinMode(placep, OUTPUT);
  pinMode(placed, OUTPUT);
  pinMode(dcv1, OUTPUT);
  pinMode(dcv2, OUTPUT);
  pinMode(c1, INPUT);
  pinMode(c2, INPUT);
  pinMode(c3, INPUT);
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  attachInterrupt(digitalPinToInterrupt(outputA), value, CHANGE);

  alaststate = digitalRead(outputA);

  analogWrite(gripp, 0);
  digitalWrite(gripd, LOW);
  analogWrite(pullpwm, 0);
  digitalWrite(pulldir, LOW);
  analogWrite(placep, 0);
  digitalWrite(placed, LOW);

  // digitalWrite(dcv1, HIGH);
  //  digitalWrite(dcv2, LOW);


  attachInterrupt(digitalPinToInterrupt(c1), value, CHANGE);
  attachInterrupt(digitalPinToInterrupt(c2), value, CHANGE);
  attachInterrupt(digitalPinToInterrupt(c3), value, CHANGE);
}



void loop()
{
  //   Serial.println(counter);
  // Serial.println("counter");

  error = setpoint - counter;
  pid_encoder();
  last_error = error;
  // Serial.print("error is : ");
  //Serial.println(error);

  if (digitalRead(c1) == HIGH) {
    if (a1 == 0)
      Serial.println("c1 is high");
    a1 = 1;
     digitalWrite(dcv1, HIGH);   //piston open
      digitalWrite(dcv2, LOW);
      delay(600);
      //spring extends here
      analogWrite(pullpwm, 220);
      digitalWrite(pulldir, HIGH);
      delay(1600);
      digitalWrite(dcv1, LOW);   //piston lock
      digitalWrite(dcv2, HIGH);
      analogWrite(pullpwm, 0);
      delay(600);



      analogWrite(pullpwm, 240); //pulling spring
      digitalWrite(pulldir, LOW);
      delay(400);
      analogWrite(pullpwm, 0);
      delay(400);

    analogWrite(gripp, 50);
    digitalWrite(gripd, HIGH );
    Serial.println("Motor2 is moving backward");
    delay(200);
    analogWrite(gripp, 0);
    Serial.println("Motor2 STOP!");
    flag = 1;
  }


  if (flag == 1) {
    if (counter < (90)) {
      analogWrite(placep, 50);
      digitalWrite(placed, HIGH);
      if (a1 == 1) {
        Serial.println("moving down");
        a1 = 2;
      }
    }
    else if (counter >= (90)) {
      analogWrite(placep, 0);
      Serial.println("stoppppp   ");
      flag = 10;
    }
  }


  if (flag == 2) {

    if (counter > (90))
    {
      analogWrite(placep, 150);
      digitalWrite(placed, LOW);
      Serial.println("going up");
    }
    else
    {
      analogWrite(placep, 0);
      delay(50);
      analogWrite(placep, 50);
      delay(50);
      analogWrite(placep, 0);
      flag = 3;
    }
  }



  if (digitalRead(c2) == HIGH) {
    //    if (a2 == 0) {
    //      Serial.pr/intln("c2 is high");
    //      a2 = 1;/
    // code for grab.........................
    //      if (counter > 50) {/
    flag = 100;

    //     / else if (counter < 50) {
    //        c = 0;/
    /*if (c == 0)     {
      if (error >= 1)   {
        digitalWrite(gripd, HIGH);
        analogWrite(gripp, 120);
        delay(100);
        c = 1;
        analogWrite(placep, 0);
        analogWrite(gripp, 0);*/
    //         / }
    //        }/
  }

  if (flag == 100) {

    //    if (counter < 50) {/
    analogWrite(gripp, 255);
    digitalWrite(gripd, LOW);
    delay(1500);
    // digitalWrite(placed, LOW);
    // analogWrite(placep, 180);
    // delay(200);
    pid1 = error * kp;
    if (pid1 > 50)
      pid1 = 0;

    if (counter > 90)
    {
      Serial.println("ccw gripper");
      digitalWrite(placed, LOW);
      analogWrite(placep, 150);
      delay(1100);
    }

    if (counter >= 40 && counter <= 90)
    {
      if (c == 0)
      {
        digitalWrite(placed, HIGH);
        analogWrite(placep, pid1);
        delay(100);
      }
      c = 1;
      if (counter < 50)
      {
        analogWrite(placep, 0);

      }
      // digitalWrite(placed, LOW);
      // analogWrite(placep, 80);
    }


    //    }/
    //    delay(400);
    //    else/ if (counter > 50) {
    analogWrite(placep, 0);

    delay(100);
    analogWrite(gripp, 0);

    //analogWrite(gripp, 0);
    flag = 102;
    
    //    }/
  }
  if(flag == 101)
  {
    delay(100);
    analogWrite(gripp, 0);
    flag=102;
    }

  if (digitalRead(c3) == HIGH)
  {
    if (a3 == 0)
      Serial.println("c3 is high");

    analogWrite(gripp, 0);
    delay(150);
    analogWrite(gripp, 50);
    digitalWrite(gripd, HIGH );
    delay(150);//................cmnt
    analogWrite(gripp, 0);
    analogWrite(placep, 100);
    digitalWrite(placed, HIGH);
    delay(150);
    analogWrite(placep, 0);
    digitalWrite(dcv1, HIGH);  //piston open
    digitalWrite(dcv2, LOW);
    a3 = 1;
  }

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
    d = (error - last_error) * kd;
    pid = (p + d);
    if (pid > 255)
    {
      pid = 255;
    }
    // Serial.print("Error is +ve and pid is  : ");
    // Serial.println(pid);
  }
  if (error < 0)
  {
    p = (-error) * kp;
    d = abs(error - last_error) * kd;
    pid = (p + d);
    if (pid > 255)
    {
      pid = 255;
    }
    //  Serial.print("Error is -ve and pid is : ");
    //  Serial.println(pid);
  }

}

void stopsetpt()
{
  if (error > 1)
  {
    analogWrite(placep, pid);
    digitalWrite(placed, HIGH);
    setpoint = setpoint - 10;
  }
  if (error < -1)
  {


    analogWrite(placep, pid);
    digitalWrite(placed, LOW);
  }
  if ((error >= -1) && (error <= 1))
  {
    analogWrite(placep, pid);

  }
}
