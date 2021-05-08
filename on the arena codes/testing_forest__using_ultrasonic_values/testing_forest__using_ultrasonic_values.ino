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
float kp = 3.8, kd = 2.3 , ki = 0.3;
float p = 0, i = 0, d = 0, pid = 0;

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
  errorls = ls2 - ls1;
  cal();
  totalerrorls += abs(errorls);

  correctl();


  if (flag == 0)
  {
    if ((backsensor < 151) && (avgls < 155))
    {
      diaright();
      correctr();


    }
    else if ((backsensor >= 151 ) && (avgls >= 155))
    {
      analogWrite(pwm2, 0);
      digitalWrite(dir2, HIGH);
      analogWrite(pwm4, 0);
      digitalWrite(dir4, HIGH);
      flag = 1;
    }
  }



  if (flag == 1)
  {
    if ((backsensor < 330) && ((avgls > 35)))
    {
      int tm = millis();
      if (tm >= 700) && (tm <= 1000))
      {
        sp = 100;   /// depends on the battey voltage
        right();
      }
      else
      {
        sp =
          dialeft();
      }
        //      correctl();
      }
    else if ((backsensor >= 330) && ((avgls <= 35)))
    {
      
      analogWrite(pwm2, 0);
      digitalWrite(dir2, HIGH);
      analogWrite(pwm4, 0);
      digitalWrite(dir4, HIGH);
      //      flag = 2;
    }
  }


  if (flag == 2 )
  {
    if ((frontsensor > 117) && (avgls < 170))
    {
      diaright();
      //      correctl();
    }
    else if ((frontsensor <= 117 ) && (avgls < 170))
    {

      analogWrite(pwm1, 0);
      digitalWrite(dir1, HIGH);
      analogWrite(pwm3, 0);
      digitalWrite(dir3, HIGH);
      flag = 3;
    }
  }
  if (flag == 3)
  {
    if (frontsensor > 65)
    {
      dialeft();
      //      correctr();

    }
    else if (frontsensor <= 65)
    {
      flag = 4;
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




void correctl()
{


  if (errorls < -5 )
  {
    analogWrite(pwm2, pid );
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
}

/*void correctl_1()
  {
    if (errorls < -5)
  {
    analogWrite(pwm2, pid );
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

void correctr()
{


  if (errorls < -5)
  {
    analogWrite(pwm1, pid );
    digitalWrite(dir1, LOW);
    analogWrite(pwm3, pid);
    digitalWrite(dir3, LOW);

  }
  if (errorls > 5)
  {
    analogWrite(pwm1, pid );
    digitalWrite(dir1, HIGH);
    analogWrite(pwm3, pid);
    digitalWrite(dir3, HIGH);

  }

  if ((errorls > -5) && (errorls < 5))
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
    d = (errorls - lasterrorls) * kd;
    i =  totalerrorls * ki;
    pid = (p + d + i);

    if (pid > 120)
      pid = 120;
  }

  if (errorls < 0)
  {
    p = (-errorls) * kp;
    d = abs(errorls - lasterrorls) * kd;
    i =  totalerrorls * ki;
    pid = (p + d + i );

    if (pid > 120)
      pid = 120;
  }
}
