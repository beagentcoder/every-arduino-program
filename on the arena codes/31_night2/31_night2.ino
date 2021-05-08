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
int lasterrorls = 0, totalerrorls = 0;
int lastl1 = 0, lastl2 = 0, lastls = 0, errorls;
int sp1 = 100, sp2 = 140;
float kp = 4.8, kd = 1.3 , ki = 0.3;
float p = 0, i = 0, d = 0, pid = 0;
float kp0 = 4.8, kd0 = 1.3 , ki0 = 0.3;
float p0 = 0, i0 = 0, d0 = 0, pid0 = 0;
int error = 0, totalerror = 0, lasterror = 0;
int sp = 80;
int flag = 0;
int steerr = 0;
int setpoint1 = 40;
int setpoint2 = 35;
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
  errorls = ls2 - ls1;

  totalerrorls += abs(errorls);
  error = setpoint1 - avgls;
  totalerror += abs(error);
  cal();

  if ( flag == 100)
  {
    correctl();
  }

  if (flag == 0) {
    if ((errorls > -5) && (errorls < 5))
      flag = 1;
  }

  if (flag == 1)  {
    if ((backsensor < 110) && (avgls < 110))  {
      correctr();
      diaright();
      if ((backsensor < 40) && (avgls < 40))  {
        if (steerr == 0) {
          steerright();
          if ((backsensor < 80) && (avgls < 80))  {
            steerr = 1;
          }
        }
      }
    }
    else if ((backsensor >= 110 ) && (avgls >= 110))  {
      analogWrite(pwm2, 120);
      digitalWrite(dir2, HIGH);
      analogWrite(pwm4, 120);
      digitalWrite(dir4, HIGH);
      if (backsensor > 120)
        flag = 2;
    }
  }

  if (flag == 2)  {
    if (backsensor < 125)   {
      forward();
    }
    else if (backsensor > 125) {
      correctl();
      flag = 3;
    }
  }

  if (flag == 3 ) {
    if ((backsensor < 280) && (avgls > 40))  {
      dialeft();
      correctl();
    }
    else if ((backsensor > 280) && (avgls <= 40))  {
      analogWrite(pwm1, 0);
      digitalWrite(dir1, HIGH);
      analogWrite(pwm3, 0);
      digitalWrite(dir3, HIGH);
      flag = 100;
    }
  }

  if (flag == 3)
  {
    if (backsensor > 310)
    {
      forward();
      //      correctr();

    }
    else if (backsensor > 310)
    {
      analogWrite(pwm2, 0);
      digitalWrite(dir2, HIGH);
      analogWrite(pwm4, 0);
      digitalWrite(dir4, HIGH);
    }
  }

  if (flag == 4)
  {
    set_zero();

  }


  lasterrorls = errorls;




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
  lasterror = error;
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

  analogWrite(pwm1, sp);
  analogWrite(pwm3, sp);
  digitalWrite(dir1, LOW);
  digitalWrite(dir3, HIGH);

}

void diaright()
{
  digitalWrite(dir2, LOW);
  digitalWrite(dir4, HIGH);
  analogWrite(pwm2, sp);
  analogWrite(pwm4, sp);

}


void setpt()

{
  if (error > 3)
  {
    analogWrite(pwm1, sp + pid0);
    analogWrite(pwm2, sp + pid0);
    analogWrite(pwm3, sp - (pid0 / 2));
    analogWrite(pwm4, sp - (pid0 / 2));

  }
  if (error < -3)
  {
    analogWrite(pwm1, sp - (pid / 2));
    analogWrite(pwm2, sp - (pid0 / 2));
    analogWrite(pwm3, sp + pid0);
    analogWrite(pwm4, sp + pid0);

  }
  if ((error < 3) && (error > -3))
  {
    analogWrite(pwm1, sp);
    analogWrite(pwm2, sp);
    analogWrite(pwm3, sp);
    analogWrite(pwm4, sp);
    totalerror = 0;
  }



}
void correctl()
{


  if (errorls < -5 )
  {
    analogWrite(pwm2, pid );
    digitalWrite(dir2, LOW);
    analogWrite(pwm4, pid / 2);
    digitalWrite(dir4, LOW);

  }
  if (errorls > 5)
  {
    analogWrite(pwm2, pid );
    digitalWrite(dir2, HIGH);
    analogWrite(pwm4, pid / 2);
    digitalWrite(dir4, HIGH);

  }

  if ((errorls > -5) && (errorls < 5))
  {
    analogWrite(pwm2, 0);
    digitalWrite(dir2, LOW);
    analogWrite(pwm4, 0);
    digitalWrite(dir4, HIGH);
    totalerrorls  = 0;


  }
}

/*void correctl_1()
  {
    if (errorls < -5)
  {
    afloat kp = 4.8, kd = 1.3 , ki = 0.3;
  float p = 0, i = 0, d = 0, pid = 0;nalogWrite(pwm2, pid );
    digitalWrite(dir2, LOW);
    analogWrite(pwm4, pid);
    digitalWrite(dir4, LOW);

  }
  if (errorls > 5)
  {
    analogWrite(pwm2, pid );
    digitalWrite(dir2, HIGH);
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
  }*/


void correctr()
{


  if (errorls < -5)
  {
    analogWrite(pwm1, pid );
    digitalWrite(dir1, LOW);
    analogWrite(pwm3, pid / 2);
    digitalWrite(dir3, LOW);

  }
  if (errorls > 5)
  {
    analogWrite(pwm1, pid );
    digitalWrite(dir1, HIGH);
    analogWrite(pwm3, pid / 2);
    digitalWrite(dir3, HIGH);

  }

  if ((errorls > -5) && (errorls < 5))
  {
    if (steerr == 1) {
      analogWrite(pwm1, 0);
      digitalWrite(dir1, LOW);
      analogWrite(pwm3, 0);
      digitalWrite(dir3, LOW);
    }
    totalerrorls = 0;

  }
}


void cal()
{

  if (errorls > 0)
  {
    p = errorls * kp;
    d = (errorls - lasterrorls) * kd;
    i =  totalerrorls * ki;
    pid = ((p + i) - d);

    if (pid > 120)
      pid = 120;
  }

  if (errorls < 0)
  {
    p = (-errorls) * kp;
    d = abs(errorls - lasterrorls) * kd;
    i =  totalerrorls * ki;
    pid = ((p + i) - d);

    if (pid > 120)
      pid = 120;
  }


  if (error > 0)
  {
    p0 = error * kp0;
    d0 = (error - lasterror) * kd0;
    i0 =  totalerror * ki0;
    pid0 = ((p0 + i0) - d0);

    if (pid0 > 30)
      pid0 = 30;
  }

  if (error < 0)
  {
    p0 = (-error) * kp0;
    d0 = abs(error - lasterror) * kd0;
    i0 =  totalerror * ki0;
    pid0 = ((p0 + i0) - d0);

    if (pid0 > 40)
      pid0 = 40;
  }


}

void forward()
{
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4, HIGH);
  analogWrite(pwm1, sp);
  analogWrite(pwm2, sp);
  analogWrite(pwm3, sp);
  analogWrite(pwm4, sp);
}
void backward()
{
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, LOW);
  analogWrite(pwm1, sp);
  analogWrite(pwm2, sp);
  analogWrite(pwm3, sp);
  analogWrite(pwm4, sp);
}
void left()
{
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4, LOW);
  analogWrite(pwm1, sp);
  analogWrite(pwm2, sp);
  analogWrite(pwm3, sp);
  analogWrite(pwm4, sp);
}
void right()
{
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, HIGH);
  analogWrite(pwm1, sp);
  analogWrite(pwm2, sp);
  analogWrite(pwm3, sp);
  analogWrite(pwm4, sp);
}
void steerright()
{
  digitalWrite(dir1, HIGH);
  digitalWrite(dir3, LOW);
  analogWrite(pwm1, sp);
  analogWrite(pwm3, sp);
}
void set_zero()
{

  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, 0);
}

void set_dia()
{
  /*digitalWrite(dir2, LOW);
  analogWrite(pwm2, 120);
  digitalWrite(dir4, HIGH);
  analogWrite(pwm4, 120);
*/
  if (errorls >= 3)
  {
    digitalWrite(dir1, HIGH);
    analogWrite(pwm1, pidn);
    digitalWrite(dir3, HIGH);
    analogWrite(pwm3, pidn);
  }
  if (errorls <= -3)
  {
    digitalWrite(dir1, LOW);
    analogWrite(pwm1, pidn);
    digitalWrite(dir3, LOW);
    analogWrite(pwm3, pidn);
  }
  if ((errorls > -3) && (errorls < 3))
  {
    digitalWrite(dir1, LOW);
    analogWrite(pwm1, 0);
    digitalWrite(dir3, LOW);
    analogWrite(pwm3, 0);
  }
}

void dia_pid()
{
  if (errorls > 0)
  {
    p = errorls * kpn;
    d = (errorls - lasterrorls) * kdn;
    i =  totalerrorls * kin;
    pidn = ((p + i) - d);

    if (pidn > 120)
      pidn = 120;
  }

  if (errorls < 0)
  {
    p = (-errorls) * kpn;
    d = abs(errorls - lasterrorls) * kdn;
    i =  totalerrorls * kin;
    pidn = ((p + i) - d);

    if (pidn > 120)
      pidn = 120;
  }

}

//////////////////////////////////////////////////////////////

void set_left()
{
  /*digitalWrite(dir2, LOW);
  analogWrite(pwm2, 120);
  digitalWrite(dir4, HIGH);
  analogWrite(pwm4, 120);
*/
  if (error >= 3)
  {
    digitalWrite(dir3, LOW);
    analogWrite(pwm3, pidm);
    digitalWrite(dir4, HIGH);
    analogWrite(pwm4, pidm);
  }
  if (error <= -3)
  {
    digitalWrite(dir3, HIGH);
    analogWrite(pwm3, pidm);
    digitalWrite(dir4, LOW);
    analogWrite(pwm4, pidm);
  }
  if ((error > -3) && (error < 3))
  {
    digitalWrite(dir3, HIGH);
    analogWrite(pwm3, 0);
    digitalWrite(dir4, LOW);
    analogWrite(pwm4, 0);
  }
}

void left_pid()
{
  if (error > 0)
  {
    p = errorls * kpm;
    d = (errorls - lasterrorls) * kdm;
    i =  totalerrorls * kim;
    pidm = ((p + i) - d);

    if (pidm > 120)
      pidm = 120;
  }

  if (error < 0)
  {
    p = (-errorls) * kpm;
    d = abs(errorls - lasterrorls) * kdm;
    i =  totalerrorls * kim;
    pidm = ((p + i) - d);

    if (pidm > 120)
      pidm = 120;
  }

}
