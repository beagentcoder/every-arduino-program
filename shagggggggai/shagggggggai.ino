#define outputA 21
#define outputB 20
int counter = 0;
int astate;
int alaststate;

int gripp = 11;               // GRIPPER
int gripd = 34 ;

int pullpwm = 10;
int pulldir = 28;         // SPRING MOTOR

int placep = 12;        //rotator
int placed = 32 ;

int dcv1 = 52;
int dcv2 = 53;       //piston

int testing = 0;
int grip = 1;
int c1 = 27, c2 = 25, c3 = 23;
int a1 = 0, a2 = 0, a3 = 0;

float kp = 0.65, kd = 0;

int p = 0 , d = 0 , pid = 0, pid1 = 0, setpoint = 0;
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
  Serial.println(counter);
  Serial.println("counter");

  error =  counter - setpoint;
  pid_encoder();
  last_error = error;
  // Serial.print("error is : ");
  //Serial.println(error);

  if (digitalRead(c1) == HIGH) {
    if (a1 == 0)
      Serial.println("c1 is high");
    a1 = 1;

    analogWrite(placep, 80);
    digitalWrite(placed, HIGH);
    delay(200);
    analogWrite(placep, 0);
    delay(100);
    analogWrite(gripp, 60);
    digitalWrite(gripd, LOW );
    delay(150);
    analogWrite(gripp, 0);
    delay(100);

    digitalWrite(dcv1, HIGH);   //piston open
    digitalWrite(dcv2, LOW);
    delay(600);

    //spring extends here
    analogWrite(pullpwm, 220);
    digitalWrite(pulldir, HIGH);
    delay(1600);
    digitalWrite(dcv1, LOW);   //piston lock
    digitalWrite(dcv2, HIGH);
    delay(600);
    analogWrite(pullpwm, 0);
    delay(150);



    analogWrite(pullpwm, 240); //pulling spring
    digitalWrite(pulldir, LOW);
    delay(400);
    analogWrite(pullpwm, 0);
    delay(400);


    /*Serial.println("Motor2 is moving backward");
      delay(200);
      analogWrite(gripp, 0);
      Serial.println("Motor2 STOP!");*/
    //flag = 1;
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
    digitalWrite(gripd, HIGH);
    delay(150);
    // digitalWrite(placed, LOW);
    // analogWrite(placep, 180);
    // delay(200);
    pid1 = abs(error * kp);
    if (pid1 > 40)
      pid1 = 40;


    if (counter > 65)
    {
      do
      { Serial.print("counter1 :");
        Serial.println(counter);
        // Serial.println("ccw gripper");
        digitalWrite(placed, LOW);
        analogWrite(placep, 150);
        Serial.println("Wapas1");
      } while (counter > 65);
    }

    if (counter >= 50 && counter <= 65)
    { do
      {
        if (c == 0)
        { Serial.print("counter2 : ");
          Serial.println(counter);
          digitalWrite(placed, LOW);       //LOW kp=1.4 setpoint=20
          analogWrite(placep, 50);
          Serial.println("Wapas2");
          Serial.print("PID is :  ");
          Serial.println(pid1);
        }


      } while (counter >= 50);
      c = 1;
    }
    if (counter < 50)
    {
      Serial.print("counter3 :");
      Serial.println(counter);
      analogWrite(placep, 0);
      delay(200);
      analogWrite(gripp, 250);
    }
    flag = 101;
  }
  if (flag == 101)
  {
    delay(100);
    analogWrite(gripp, 0);
    flag = 103;
  }

  if (digitalRead(c3) == HIGH)
  {
    if (a3 == 0)
      Serial.println("c3 is high");

    analogWrite(gripp, 0);
    delay(150);
    analogWrite(gripp, 50);
    digitalWrite(gripd, HIGH );
    delay(200);//................cmnt
    analogWrite(gripp, 0);
    delay(100);
    analogWrite(placep, 80);
    digitalWrite(placed, HIGH);
    delay(300);
    analogWrite(placep, 0);
    delay(200);
    digitalWrite(dcv1, HIGH);  //piston open
    digitalWrite(dcv2, LOW);
    delay(400);
    a3 = 1;
    delay(800);
    analogWrite(gripp, 80);
    digitalWrite(gripd, LOW);
    delay(120);
    analogWrite(gripp, 0);
    digitalWrite(placed, LOW);
    analogWrite(placep, 100);
    delay(500);
    analogWrite(placep, 0);

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
