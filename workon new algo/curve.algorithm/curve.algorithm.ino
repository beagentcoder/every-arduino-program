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


int lastback, backsensor, errorback, ls1, ls2, avgls, err = 0;
double pulse, inches, cm, distance, duration;
int frontsensor;

int lastl1 = 0, lastl2 = 0, lastls = 0, errorls;
int sp1 = 50, sp2 = 70;
int kp = 10, kd = 0, ki = 0;
int p = 0, i = 0, d = 0, pid = 0;
int setpoint = 40;
int flag = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(trigPin1 , OUTPUT);
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

  SonarSensor(trigPin1, echoPin1);
  ls1 = distance;
  SonarSensor(trigPin2, echoPin2);
  ls2 = distance;
  SonarSensor(trigPin3, echoPin3);
  backsensor = distance;
  SonarSensor(trigPin4, echoPin4);
  frontsensor = distance;


  avgls = (ls1 + ls2) / 2;
  err = setpoint - avgls;

  errorls = ls1 - ls2;
  cal();


  if (flag == 0)
  {
    correctl();
    if (avgls > 160)
      left();
    else if (avgls < 160)
      flag = 1;

  }
  if (flag == 1)
  {
    if ((backsensor <= 80) && (avgls > 50))
    {
      dialeft();
      correctl();

    }
    else if ((backsensor > 80) && (avgls < 50))
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
      correctr();
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
    { diaright();
      correctr();
    }
    else if (backsensor > 200)
    {
      flag = 4;
    }
  }




  if (flag == 4)
  {
    if (avgls < 150)
    {
      moveright();
      correctr();
    }
    else if (avgls >= 150)
      flag = 5;


  }
  if (flag == 5)

  {
    if ((backsensor < 280) && (avgls < 170 ))
    {
      diaright();

      correctr();
    }

    else if ((backsensor >= 280) && (avgls > 170))

    {
      flag = 6;


    }

  }

  if (flag == 6)
  {
    if (backsensor < 300)
    {
      moveforward();

      correctl();
    }
    else if (backsensor >= 300)
    {
      flag = 7;


    }

  }


  if (flag == 7)

  {
    if ((backsensor < 330) && (avgls > 150))
    {
      dialeft();
      correctl();

    }
    else if ((backsensor > 330) && (avgls > 150))
    {
      flag = 8;
    }

  }
  if (flag == 8)
  {
    if (avgls > 120)
    {
      moveleft();

      correctl();
    }
    else if (avgls <= 120)
    {
      flag = 9;
    }
  }
  if (flag == 9)
  {
    if ((backsensor < 360) && (avgls > 50))
    {
      dialeft();
      correctl();

    }
    else if ((backsensor > 360) && (avgls <= 50))
    {
      flag = 10;
    }
  }

  if (flag == 10)
  {
    if (backsensor < 380)
    {
      moveforward();
      correctl();
    }
    else if (backsensor > 380)
    {
      flag = 11;
    }
  }
  if (flag == 11)
  {
    if (avgls < 130)
    {
      moveright();

      correctr();
    }
    else if (avgls >= 130)
    {
      flag = 12;
    }
  }

  if (flag == 12)
  {
    if (frontsensor > 100)
    {
      moveforward();
      corrctside();
    }
    else if (frontsensor <= 100)
    {
      flag = 13;
    }
  }
  if (flag == 13)
  {
    if (avgls > 100)
    {
      moveleft();

    }
    else if (avgls <= 100)
    {
      flag = 14;

    }
  }
  if (flag == 14)
  {
    set_zero();
  }

  Serial.print("   LeftSensor1  ");
  Serial.print(ls1);
  Serial.print("  LeftSensor2   ");
  Serial.print(ls2);
  Serial.print("  backsensor   "  );
  Serial.println(backsensor);
  Serial.print("  frontsensor   "  );
  Serial.println(frontsensor);


  Serial.println("                  ");
  Serial.println("                  ");
  Serial.println("                  ");

  Serial.print("   average left" );
  Serial.println(avgls);
  Serial.print("       flag==    ");
  Serial.println(flag);
  lastls = avgls;

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



void dialeft()
{

  analogWrite(pwm1, sp1);
  digitalWrite(dir1, HIGH);
  analogWrite(pwm3, sp1);
  digitalWrite(dir3, HIGH);

}

void diaright()
{
  analogWrite(pwm2, sp2);
  digitalWrite(dir2, HIGH);
  analogWrite(pwm4, sp1);
  digitalWrite(dir4, HIGH);

}



void correctl()
{


  if (errorls < -5)
  {
    analogWrite(pwm2, (pid * 1.4));
    digitalWrite(dir2, HIGH);
    analogWrite(pwm4, pid);
    digitalWrite(dir4, LOW);

  }
  if (errorls > 5)
  {
    analogWrite(pwm2, (pid * 1.4));
    digitalWrite(dir2, LOW);
    analogWrite(pwm4, pid);
    digitalWrite(dir4, HIGH);

  }

  if ((errorls > -5) && (errorls < 5))
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


void correctr()
{


  if (errorls < -2)
  {
    analogWrite(pwm1, pid );
    digitalWrite(dir1, HIGH);
    analogWrite(pwm3, pid);
    digitalWrite(dir3, LOW);

  }
  if (errorls > 2)
  {
    analogWrite(pwm1, pid );
    digitalWrite(dir1, LOW);
    analogWrite(pwm3, pid);
    digitalWrite(dir3, HIGH);

  }

  if ((errorls > -2) && (errorls < 2))
  {
    analogWrite(pwm1, 0);
    digitalWrite(dir1, LOW);
    analogWrite(pwm3, 0);
    digitalWrite(dir3, LOW);

  }
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

  if (errorls > 0)
  {
    p = errorls * kp;
    d = (errorls - lastls) * kd;

    pid = (p + d );

    if (pid > 120)
      pid = 120;
  }

  if (errorls < 0)
  {
    p = (-errorls) * kp;
    d = abs(errorls - lastls) * kd;
    pid = (p + d );

    if (pid > 120)
      pid = 120;
  }
}
