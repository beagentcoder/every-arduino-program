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


int lastback, backsensor, errorback, ls1, ls2, avgls;
double pulse, inches, cm, distance, duration;
int frontsensor;

int lastl1 = 0, lastl2 = 0, lastls = 0, errorls;

int kp = 5, kd = 0, ki = 0;
int p = 0, i = 0, d = 0, pid = 0;

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
  errorls = ls1 - ls2;
  cal();


  if (flag == 0)
  {
    if (backsensor <= 145)
    {
      dialeft();
 
     }
    else if (backsensor > 145 )
    {
      analogWrite(pwm1, 0);
      digitalWrite(dir1, HIGH);
      analogWrite(pwm3, 0);
      digitalWrite(dir3, HIGH);
      flag = 1;
    }
  }



  if (flag == 1)
  {
    if (backsensor < 300)
    {
      diaright();
      correctr();
    }
    else if (backsensor >= 300)
    {
      analogWrite(pwm2, 0);
      digitalWrite(dir2, HIGH);
      analogWrite(pwm4, 0);
      digitalWrite(dir4, HIGH);


      flag = 2;
    }
  }

  if (flag == 2 )
  {
    if (frontsensor > 170)
    {
      dialeft();
      correctl();
    }
    else if (frontsensor <= 170 )
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
    if (frontsensor > 80)
    {
      diaright();
      correctr();
    }
    else if (frontsensor <= 80)
      {
        flag = 4;
        analogWrite(pwm2, 0);
        digitalWrite(dir2, HIGH);
        analogWrite(pwm4, 0);
        digitalWrite(dir4, HIGH);

      }
  }

 if(flag == 4)
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

  analogWrite(pwm1, 100);
  digitalWrite(dir1, HIGH);
  analogWrite(pwm3, 100);
  digitalWrite(dir3, HIGH);

}

void diaright()
{
  analogWrite(pwm2, 140);
  digitalWrite(dir2, HIGH);
  analogWrite(pwm4, 100);
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


void correctr()
{


  if (errorls < -5)
  {
    analogWrite(pwm1, pid );
    digitalWrite(dir1, HIGH);
    analogWrite(pwm3, pid);
    digitalWrite(dir3, LOW);

  }
  if (errorls > 5)
  {
    analogWrite(pwm1, pid );
    digitalWrite(dir1, LOW);
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
