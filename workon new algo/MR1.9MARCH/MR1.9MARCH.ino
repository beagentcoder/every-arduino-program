int trigPin1 = 50;
int echoPin1 = 52;
int trigPin2 = 51;
int echoPin2 = 53;
int trigPin3 = 49;
int echoPin3 = 47;
int trigPin4 = 48;
int echoPin4 = 46;

int dir1 = 38, dir2 = 32, dir3 = 36, dir4 = 34;
int pwm1 = 9, pwm2 = 8, pwm3 = 10 , pwm4 = 11;

int leftled1 = 20, leftled2 = 21, backled = 22, frontled = 23; //configuration needed date : 9/march/2019

int lastback, backsensor, errorback, lesftsensor1, lesftsensor2, leftoffset, err = 0;
double pulse, inches, cm, distance, duration;
int frontsensor;

int lastl1 = 0, lastl2 = 0, lastls = 0, lefterror;
int sp1 = 50, sp2 = 70;
int kp = 10, kd = 0, ki = 0;
int p = 0, i = 0, d = 0, pid = 0;
int setpoint = 40;
int flag = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);

  pinMode(pwm1, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(pwm4, OUTPUT);
  pinMode(dir4, OUTPUT);

}


void loop()
{

  // ********************************************************************

  Distance();

  leftoffset = (lesftsensor1 + lesftsensor2) / 2;
  err = setpoint - leftoffset;

  lefterror = lesftsensor1 - lesftsensor2;
  cal();



  if (flag == 0)
  {
    leftcorrect();
    if (leftoffset > 160)
      left();
    else if (leftoffset < 160)
      flag = 1;

  }




  if (flag == 1)
  {
    if ((backsensor <= 80) && (leftoffset > 50))
    {
      leftdiagonal();
      leftcorrect();

    }
    else if ((backsensor > 80) && (leftoffset < 50))
    {
      analogWrite(pwm1, 0);
      digitalWrite(dir1, HIGH);
      analogWrite(pwm3, 0);
      digitalWrite(dir3, HIGH);
      flag = 2;
    }
  }



  if (flag == 2)
  {
    if (backsensor < 140)
    {
      rightcorrect();
      //     correctsetpoint();
      moveforward();

    }
    else if (backsensor > 140)
    {
      flag = 3;
    }
  }



  if (flag == 3)
  { if (backsensor < 200)
    { rightdiagonal();
      rightcorrect();
    }
    else if (backsensor > 200)
    {
      flag = 4;
    }
  }




  if (flag == 4)
  {
    if (leftoffset < 150)
    {
      moveright();
      rightcorrect();
    }
    else if (leftoffset >= 150)
      flag = 5;
  }



  if (flag == 5)
  {
    if ((backsensor < 280) && (leftoffset < 170 ))
    {
      rightdiagonal();
      rightcorrect();
    }
    else if ((backsensor >= 280) && (leftoffset > 170))
    {
      flag = 6;
    }
  }




  if (flag == 6)
  {
    if (backsensor < 320)
    {
      moveforward();

      leftcorrect();
    }
    else if (backsensor >= 320)
    {
      flag = 7;

      leftcorrect();
    }
  }





  if (flag == 7)
  {
    set_zero();
    leftcorrect();
  }


  Print():
    lastls = leftoffset;
  }




  void leftdiagonal()
{

  analogWrite(pwm1, sp1);
  digitalWrite(dir1, HIGH);
  analogWrite(pwm3, sp1);
  digitalWrite(dir3, HIGH);

}

void rightdiagonal()
{
  analogWrite(pwm2, sp2);
  digitalWrite(dir2, HIGH);
  analogWrite(pwm4, sp1);
  digitalWrite(dir4, HIGH);

}


void rightcorrect()
{
  if (lefterror < -2)
  {
    analogWrite(pwm1, pid );
    digitalWrite(dir1, HIGH);
    analogWrite(pwm3, pid);
    digitalWrite(dir3, LOW);
  }
  if (lefterror > 2)
  {
    analogWrite(pwm1, pid );
    digitalWrite(dir1, LOW);
    analogWrite(pwm3, pid);
    digitalWrite(dir3, HIGH);
  }
  if ((lefterror > -2) && (lefterror < 2))
  {
    analogWrite(pwm1, 0);
    digitalWrite(dir1, LOW);
    analogWrite(pwm3, 0);
    digitalWrite(dir3, LOW);
  }
}


void leftcorrect()
{


  if (lefterror < -5)
  {
    analogWrite(pwm2, (pid * 1.4));
    digitalWrite(dir2, HIGH);
    analogWrite(pwm4, pid);
    digitalWrite(dir4, LOW);

  }
  if (lefterror > 5)
  {
    analogWrite(pwm2, (pid * 1.4));
    digitalWrite(dir2, LOW);
    analogWrite(pwm4, pid);
    digitalWrite(dir4, HIGH);

  }

  if ((lefterror > -5) && (lefterror < 5))
  {
    analogWrite(pwm2, 0);
    digitalWrite(dir2, LOW);
    analogWrite(pwm4, 0);
    digitalWrite(dir4, HIGH);

  }
}


void moveright()
{
  analogWrite(pwm1, sp1);
  digitalWrite(dir1, LOW);
  analogWrite(pwm2, sp2);
  digitalWrite(dir2, HIGH);
  analogWrite(pwm3, sp1);
  digitalWrite(dir3, LOW);
  analogWrite(pwm4, sp1);
  digitalWrite(dir4, HIGH);


}

void left()
{
  analogWrite(pwm1, sp1);
  digitalWrite(dir1, HIGH);
  analogWrite(pwm2, sp2);
  digitalWrite(dir2, LOW);
  analogWrite(pwm3, sp1);
  digitalWrite(dir3, HIGH);
  analogWrite(pwm4, sp1);
  digitalWrite(dir4, LOW);


}

void moveforward()
{
  analogWrite(pwm1, sp1);
  digitalWrite(dir1, HIGH);
  analogWrite(pwm2, sp2);
  digitalWrite(dir2, HIGH);
  analogWrite(pwm3, sp1);
  digitalWrite(dir3, HIGH);
  analogWrite(pwm4, sp1);
  digitalWrite(dir4, HIGH);


}




void correctsetpoint()
{
  if (err < -3)
    left();
  if (err > 3)
    moveright();
  if ((err > -3) && (err < 3))
    set_zero();
}


void set_zero()
{
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, 0);
}

void cal()
{
  if (lefterror > 0)
  {
    p = lefterror * kp;
    d = (lefterror - lastls) * kd;
    pid = (p + d );
    if (pid > 120)
      pid = 120;
  }
  if (lefterror < 0)
  {
    p = (-lefterror) * kp;
    d = abs(lefterror - lastls) * kd;
    pid = (p + d );
    if (pid > 120)
      pid = 120;
  }
}

void Distance()
{
  SonarSensor(trigPin1, echoPin1);
  lesftsensor1 = distance;
  if ((lesftsensor1 > 0) && (lesftsensor1 < 320))
    digitalWrite(leftled1, LOW);
  else
    digitalWrite(leftled1, HIGH);
  SonarSensor(trigPin2, echoPin2);
  lesftsensor2 = distance;
  if ((lesftsensor2 > 0) && (lesftsensor2 < 320))
    digitalWrite(leftled2, LOW);
  else
    digitalWrite(leftled2, HIGH);
  SonarSensor(trigPin3, echoPin3);
  backsensor = distance;
  if ((backsensor > 0) && (backsensor < 320))
    digitalWrite(backled, LOW);
  else
    digitalWrite(backled, HIGH);
  SonarSensor(trigPin4, echoPin4);
  frontsensor = distance;
  if ((frontsensor > 0) && (frontsensor < 320))
    digitalWrite(frontled, LOW);
  else
    digitalWrite(frontled, HIGH);
}


void SonarSensor(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

}

void Print()
{
  Serial.print("   LeftSensor1  ");
  Serial.print(lesftsensor1);
  Serial.print("  LeftSensor2   ");
  Serial.print(lesftsensor2);
  Serial.print("  backsensor   "  );
  Serial.println(backsensor);
  Serial.print("  frontsensor   "  );
  Serial.println(frontsensor);
  Serial.println("                  ");
  Serial.println("                  ");
  Serial.println("                  ");
  Serial.print("   average left" );
  Serial.println(leftoffset);
  Serial.print("       flag==    ");
  Serial.println(flag);
}
