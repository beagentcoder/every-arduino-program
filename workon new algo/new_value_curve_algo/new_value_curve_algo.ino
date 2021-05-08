int trigPin1 = 50;
int echoPin1 = 52;
int trigPin2 = 51;
int echoPin2 = 53;
int trigPin3 = 49;
int echoPin3 = 47;
int trigPin4 = 48;
int echoPin4 = 46;

int frontled = 43, leftled1 = 37, leftled2 = 33, backled = 29;

int dir1 = 38, dir2 = 32, dir3 = 36, dir4 = 34;
int pwm1 = 9, pwm2 = 8, pwm3 = 10 , pwm4 = 11;

int backlast, backsensor, frontsensor, backerror, leftsensor1, leftsensor2, leftavg, error = 0;
double pulse, inches, cm, distance, duration;

int leftlast1 = 0, leftlast2 = 0, leftlast = 0, lefterror, totalerror = 0;
int sp1 = 100, sp2 = 140;
float  kp = 5, kd = 2, ki = 0.45;
float p = 0, i = 0, d = 0, pid = 0;
int setpoint = 40;
int flag = 100;


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

  pinMode(leftled1, OUTPUT);
  pinMode(leftled2, OUTPUT);
  pinMode(frontled, OUTPUT);
  pinMode(backled, OUTPUT);

  pinMode(pwm3, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(pwm4, OUTPUT);
  pinMode(dir4, OUTPUT);

}


void loop() {
  // ********************************************************************

  // digitalWrite(leftled1, HIGH);
  Ultrasonic();

  leftavg = (leftsensor1 + leftsensor2) / 2;
  error = setpoint - leftavg;
  lefterror = leftsensor1 - leftsensor2;
  totalerror += lefterror;
  
  calculate();

  if (flag == 100)
  {
//   leftcorrect();
    rightcorrect();
  }

  if (flag == 0) {
    leftcorrect();
    //    backcorrect();
    if (leftavg > 120)
      left();
    else if (leftavg < 120)
      flag = 1;
  }
  if (flag == 1)
  {
    if ((backsensor <= 100) && (leftavg > 40))
    {
      leftdia();
      leftcorrect();

    }
    else if ((backsensor > 100) && (leftavg < 40))
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
    if (backsensor < 180)
    {
      rightcorrect();

      moveforward();

    }
    else if (backsensor > 180)
    {
      flag = 3;


    }

  }

  if (flag == 3)
  { if (backsensor < 232)
    { rightdia();
      rightcorrect();
    }
    else if (backsensor > 232)
    {
      flag = 4;
    }
  }




  if (flag == 4)
  {
    if (leftavg < 120)
    {
      moveright();
      rightcorrect();
    }
    else if (leftavg >= 120)
      flag = 5;


  }
  if (flag == 5)

  {
    if ((backsensor < 275) && (leftavg < 150 ))
    {
      rightdia();

      rightcorrect();
    }

    else if ((backsensor >= 275) && (leftavg > 150))

    {
      flag = 6;


    }

  }

  if (flag == 6)
  {
    if ((frontsensor > 233) && (leftavg > 154))
    {
      moveforward();

      leftcorrect();
    }
    else if ((frontsensor <= 232) && (leftavg < 154))
    {
      flag = 7;


    }

  }


  if (flag == 7)

  {
    if ((frontsensor >= 182) && (leftavg < 110 ))
    {
      leftdia();
      leftcorrect();

    }
    else if ((frontsensor < 182) && (leftavg > 110))
    {
      flag = 8;
    }

  }
  if (flag == 8)
  {

    if (leftavg > 80)
    {
      left();

      //leftcorrect();
    }
    else if (leftavg <= 80)
    {
      rest();
      flag = 9;
    }
  }
  if (flag == 9)
  {
    if ((frontsensor > 135) && (leftavg > 40))
    {
      leftdia();
      leftcorrect();

    }
    else if ((frontsensor < 135) && (leftavg <= 40))
    {
      flag = 10;
    }
  }

  if (flag == 10)
  {
    if (frontsensor > 80)
    {
      moveforward();
      leftcorrect();
    }
    else if (frontsensor < 80)
    {
      flag = 11;
    }
  }
  if (flag == 11)
  {
    if ((frontsensor > 25) && (leftavg < 90))
    {
      rightdia();
      rightcorrect();
    }
    else if ((frontsensor < 25) && (leftavg > 90))
    {
      flag = 12;
      rightcorrect();
      rightrest();
      delay(1000);

    }
  }

  if (flag == 12)
  {
    if (frontsensor != 0)
    {
      moveforward();
      //      corrctside();
    }
    if (frontsensor <= 100)
    {
      flag = 13;
    }
  }
  if (flag == 13)
  {
    if (leftavg > 100)
    {
      left();

    }
    else if (leftavg <= 100)
    {
      flag = 14;

    }
  }
  if (flag == 14)
  {
    rest();
  }

  Print();
  leftlast = leftavg;

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



void leftdia()
{

  analogWrite(pwm1, sp1);
  digitalWrite(dir1, HIGH);
  analogWrite(pwm3, sp1);
  digitalWrite(dir3, HIGH);

}

void rightdia()
{
  analogWrite(pwm2, sp2);
  digitalWrite(dir2, HIGH);
  analogWrite(pwm4, sp1);
  digitalWrite(dir4, HIGH);

}



void leftcorrect()
{


  if (lefterror < -1)
  {
    analogWrite(pwm2, (pid * 1.4));
    digitalWrite(dir2, HIGH);
    analogWrite(pwm4, pid);
    digitalWrite(dir4, LOW);

  }
  if (lefterror > 1)
  {
    analogWrite(pwm2, (pid * 1.4));
    digitalWrite(dir2, LOW);
    analogWrite(pwm4, pid);
    digitalWrite(dir4, HIGH);

  }

  if ((lefterror > -1) && (lefterror < 1))
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

  if (error < -3)

    left();

  if (error > 3)
    moveright();

  if ((error > -3) && (error < 3))
    rest();
}


void rightcorrect() {
  if (lefterror < -1) {
    analogWrite(pwm1, pid );
    digitalWrite(dir1, HIGH);
    analogWrite(pwm3, pid);
    digitalWrite(dir3, LOW);
  }
  if (lefterror > 1) {
    analogWrite(pwm1, pid );
    digitalWrite(dir1, LOW);
    analogWrite(pwm3, pid);
    digitalWrite(dir3, HIGH);
  }

  if ((lefterror > -1) && (lefterror < 1)) {
    analogWrite(pwm1, 0);
    digitalWrite(dir1, LOW);
    analogWrite(pwm3, 0);
    digitalWrite(dir3, LOW);
  }
}

void rest() {
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, 0);
}

void calculate() {
  if (lefterror > 1) {
    p = lefterror * kp;
    d = (lefterror - leftlast) * kd;
    i = ((totalerror) * ki);
    pid = (p + i );
    if (pid > 120)
      pid = 120;
  }
  if (lefterror < -1) {
    p = (-lefterror) * kp;
    d = abs(lefterror - leftlast) * kd;
    i = ((totalerror) * ki);
    pid = (p + i);
    if (pid > 120)
      pid = 120;
  }
  if ( (lefterror > - 1) && (lefterror < 1))
  {
    totalerror = 0;
    pid = 0;
  }
}
 void correct()
 {
  
 }

void backccorrect() {

}
void Ultrasonic()
{
  SonarSensor(trigPin1, echoPin1);
  leftsensor1 = distance;
  if (leftsensor1 == 0)
    digitalWrite(leftled1, HIGH);
  else
    digitalWrite(leftled1, LOW);
  SonarSensor(trigPin2, echoPin2);
  leftsensor2 = distance;
  if (leftsensor2 == 0)
    digitalWrite(leftled2, HIGH);
  else
    digitalWrite(leftled2, LOW);
  SonarSensor(trigPin3, echoPin3);
  backsensor = distance;
  if (backsensor == 0)
    digitalWrite(backled, HIGH);
  else
    digitalWrite(backled, LOW);
  SonarSensor(trigPin4, echoPin4);
  frontsensor = distance;
  if (frontsensor == 0)
    digitalWrite(frontled, HIGH);
  else
    digitalWrite(frontled, LOW);
}

void Print()
{
  Serial.print("   LeftSensor1  ");
  Serial.print(leftsensor1);
  Serial.print("  LeftSensor2   ");
  Serial.print(leftsensor2);
  Serial.print("  backsensor   "  );
  Serial.print(backsensor);
  Serial.print("  frontsensor   "  );
  Serial.println(frontsensor);


  Serial.println("                  ");
  Serial.println("                  ");
  Serial.println("                  ");

  Serial.print("   average left" );
  Serial.println(leftavg);
  Serial.print("       flag==    ");
  Serial.println(flag);

  Serial.print("integral");
  Serial.println(i);

  Serial.print("lefterror");
  Serial.println(lefterror);

  Serial.print("totalerror");
  Serial.println(totalerror);

  Serial.print("pid");
  Serial.println(pid);


}

void rightrest()
{
  analogWrite(pwm2, 0);
  analogWrite(pwm4, 0);
}
