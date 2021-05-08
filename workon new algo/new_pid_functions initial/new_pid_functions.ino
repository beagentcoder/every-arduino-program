//***********************************************************************************
int trigPin1 = 50;                                                                 //
int echoPin1 = 52;                                                                 //
int trigPin2 = 51;                                                                 // 
int echoPin2 = 53;                                                                 //
int trigPin3 = 49;                                                                 //
int echoPin3 = 47;                                                                 //
int trigPin4 = 48;                                                                 // *********these only change when there is any change in pcb******
int echoPin4 = 46;                                                                 //
                                                                                   //
int frontled = 43, leftled1 = 37, leftled2 = 33, backled = 29;                     //
                                                                                   //
int dir1 = 38, dir2 = 32, dir3 = 36, dir4 = 34;                                    //
int pwm1 = 9, pwm2 = 8, pwm3 = 10 , pwm4 = 11;                                     //
//***********************************************************************************


int backlast, backsensor, frontsensor, backerror, leftsensor1, leftsensor2, leftavg, error = 0,errorback=0;
double pulse, inches, cm, distance, duration;

int leftlast1 = 0, leftlast2 = 0, leftlast = 0, rotateerror, lastback, totalerrorb = 0, totalerror = 0,totalerrorleft=0;

int setpointb = 40, setpointl = 30,lasterrorb=0,lasterrorleft=0;

int flag = 100;
 
int sp1 = 50, sp2 = 70 ;   //*********speed of motors
//**************************************************************************
float  kp = 10, kd = 6, ki = 0.5;                                         //
float kp1 = 5, kd1 = 2.5, ki1 = 0.4;                                      // 
float p1 = 0, d1 = 0, i1 = 0, pid1 = 0;                                   //               *pid values*
float p = 0, i = 0, d = 0, pid = 0;                                       //         **kp for rotation error**
float p0 = 0, d0 = 0, i0 = 0, pid0 = 0;                                   //       ***kp1 for back correction***
float kp0 = 0, kd0 = 0, ki0 = 0;                                          //    ****kp0 for setpoint correction****
//***************************************************************************
int  steervalue1 = 0;
int steervalue2=0;

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

  
  Ultrasonic();

  leftavg = (leftsensor1 + leftsensor2) / 2;
  error = setpointl - leftavg;
  rotateerror = leftsensor1 - leftsensor2;
  totalerror += abs(rotateerror);
  backerror = setpointb - backsensor;
  totalerrorb += abs(backerror);
  totalerrorleft+=abs(error);
  
  calculate();

  if (flag == 100)                                 //************************flag 100 for checking correction functions
  {
    backcorrect();
    leftcorrect();

  }

  if (flag == 0)                                 //*******************flag0 for rightward move
  {
    steervalue1 = 140;

    if(leftavg>145)
    {
        moveleft();
        leftcorrect();
        backcorrect();
    }
  else if((leftavg<145)&&(leftavg>120))
     steerfront1();
//     setleft();
     

  else if (leftavg < 120)

      flag = 1;
  }

  if (flag == 1)                  //***************************flag 1 for diagoanal left
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
  if (flag == 2)                          //***********************flag 2 for forward move
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
  if (flag == 3)                            //**************************flag 3 for right diagonal movement
  { 
    if (backsensor < 232)
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
      moveleft();

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
      moveleft();

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
  lasterrorleft= error;
  lasterrorb=errorback;
}



void SonarSensor(int trigPin, int echoPin)    //******************calculates distance from sensors
{


  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

}



void leftdia()                                //************************************** moves in diagonal left
{

  analogWrite(pwm1, sp1);
  digitalWrite(dir1, HIGH);
  analogWrite(pwm3, sp1);
  digitalWrite(dir3, HIGH);

}

void rightdia()                             //************************************** moves in diagonal right
{
  analogWrite(pwm2, sp2);
  digitalWrite(dir2, HIGH);
  analogWrite(pwm4, sp1);
  digitalWrite(dir4, HIGH);

}



void leftcorrect()                         ///****************************rotation correction in left direction
{


  if (rotateerror < -2)
  {
    analogWrite(pwm2, (pid * 1.4));
    digitalWrite(dir2, HIGH);
    analogWrite(pwm4, pid);
    digitalWrite(dir4, LOW);

  }
  if (rotateerror > 2)
  {
    analogWrite(pwm2, (pid * 1.4));
    digitalWrite(dir2, LOW);
    analogWrite(pwm4, pid);
    digitalWrite(dir4, HIGH);

  }

  if ((rotateerror > -2) && (rotateerror < 2))
  {
    analogWrite(pwm2, 0);
    digitalWrite(dir2, LOW);
    analogWrite(pwm4, 0);
    digitalWrite(dir4, HIGH);

  }
}


void moveright()                            //************************************** moves  right
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

void moveleft()                               //************************************** moves  left
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

void moveforward()                                //************************************** moves  forward
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

void rightcorrect()                                ///****************************rotation correction in right direction 
{
  if (rotateerror < -1) {
    analogWrite(pwm1, pid );
    digitalWrite(dir1, HIGH);
    analogWrite(pwm3, pid);
    digitalWrite(dir3, LOW);
  }
  if (rotateerror > 1) {
    analogWrite(pwm1, pid );
    digitalWrite(dir1, LOW);
    analogWrite(pwm3, pid);
    digitalWrite(dir3, HIGH);
  }

  if ((rotateerror > -1) && (rotateerror < 1)) {
    analogWrite(pwm1, 0);
    digitalWrite(dir1, LOW);
    analogWrite(pwm3, 0);
    digitalWrite(dir3, LOW);
  }
}

void rest()                                       ///***********************stops everything
{
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, 0);
}

void calculate()   //*************************calculates pid for every error
{
  if (rotateerror > 1)      //*******************************************for rotation correction
  {
    p = rotateerror * kp;
    d = (rotateerror - leftlast) * kd;
    i = ((totalerror) * ki);
    pid = (p + i + d);
    if (pid > 120)
      pid = 120;
  }
  if (rotateerror < -1) {
    p = (-rotateerror) * kp;
    d = abs(rotateerror - leftlast) * kd;
    i = ((totalerror) * ki);
    pid = (p + i + d);
    if (pid > 120)
      pid = 120;
  }
  if ( (rotateerror > - 1) && (rotateerror < 1))
  {
    totalerror = 0;
    pid = 0;
  }
 //***********************************************************************for backsensor correction 
  if (backerror > 1)
  {
    p1 = backerror * kp1;
    d1 = (backerror - lastback) * kd1;
    i1 = ((totalerrorb) * ki1);
    pid1 = (p1 + i1 + d1 );
    if (pid > 120)
      pid = 120;
  }
  if (backerror < -1) {
    p1 = (-backerror) * kp1;
    d1 = abs(backerror - lastback) * kd1;
    i1 = ((totalerrorb) * ki1);
    pid1 = (p1 + i1 + d1);
    if (pid1 > 120)
      pid1 = 120;
  }
  if ( (backerror > - 1) && (backerror < 1))
  {
    totalerrorb = 0;
    pid1 = 0;

 
  }
//************************************************************** for setpoint correction

  if (error > 1)
  {
    p0 = error * kp1;
    d0 = (error - lasterrorleft) * kd1;
    i0 = ((totalerrorleft) * ki1);
    pid0 = (p0 + i0 + d0 );
    if (pid0 > 120)
      pid0 = 120;
  }
  if (error < -1) {
    p0 = (-error) * kp0;
    d0= abs(backerror - lasterrorleft) * kd0;
    i1 = ((totalerrorleft) * ki0);
    pid0 = (p0 + i0 + d0);
    if (pid0 > 120)
      pid0 = 120;
  }
  if ( (error > - 1) && (error < 1))
  {
    totalerrorleft = 0;
    pid0 = 0;

  }



  
}
void setleft()

{

  if (error < -3)
{

  
}
    

  if (error > 3)
   {
     
   }

  if ((error > -3) && (error < 3))
    rest();
  }

void steerfront1()
{


  analogWrite(pwm1, sp1);
  digitalWrite(dir1, HIGH);
  analogWrite(pwm2, ( sp2 * 0.8));
  digitalWrite(dir2, LOW);
  analogWrite(pwm3, sp1);
  digitalWrite(dir3, HIGH);
  analogWrite(pwm4, ( sp1 * 0.80));
  digitalWrite(dir4, LOW);

}

void backcorrect()
{
  if (backerror < -3)
  {
    analogWrite(pwm1, pid1);
    digitalWrite(dir1, LOW);
    analogWrite(pwm4, pid1);
    digitalWrite(dir4, LOW);

  }
  else if (backsensor > 3)
  {
    analogWrite(pwm4, pid1);
    digitalWrite(dir4, HIGH);
    analogWrite(pwm1, pid1);
    digitalWrite(dir1, HIGH);

  }
  else if ((backsensor > -3) && (backsensor < 3))
    rest();
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

  Serial.print("rotateerror");
  Serial.println(rotateerror);

  Serial.print("totalerror");
  Serial.println(totalerror);

  Serial.print("pid");
  Serial.println(pid);

  Serial.print("backerror");
  Serial.println(backerror);

  Serial.print("totalerrorb");
  Serial.println(totalerrorb);

  Serial.print("pid1");
  Serial.println(pid1);


}

void rightrest()
{
  analogWrite(pwm2, 0);
  analogWrite(pwm4, 0);
}
