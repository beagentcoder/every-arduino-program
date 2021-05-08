/**********************************************************************************************************************************************************
 this program approaches the algorithim in such a way :--
 it has a integrate function for pid in which each sensors controls its own side for error coorection
 
 the  algo is simple enough we are trying to move diagonally perfectly so as to cross the forest area without crashing into pole
  
 the diagonal path is broken down into many small paths each path having itss own set of specific pids 

  to complete the diagonal motion  in one direction first we move right then little diagonal movement then some forward motion 

   the right and left motion is respective of thier direction of motion

   right now we sensor issues which will soon be resolved
   


*****************************************************************************************************************/






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


int  backsensor, frontsensor, leftsensor1, leftsensor2, leftavg, fronterror, backerror, error = 0, errorback = 0;
double pulse, inches, cm, distance, duration;

int leftlast1 = 0, leftlast2 = 0, leftlast = 0, rotateerror, totalerrorb = 0, totalerror = 0, totalerrorleft = 0, totalerrorf = 0;

int setpointb = 20, setpointl = 30, setpointf = 0, lasterrorb = 0, lasterrorleft = 0, lasterrorf = 0;

int flag = 100;

int sp1 = 120, sp2 = 168 ;   //*********speed of motors
//**************************************************************************
float  kp = 5, kd = 3.5, ki = 0.9;                                         //
float kp1 = 4, kd1 = 2.8, ki1 = 0.8;                                       //
float p1 = 0, d1 = 0, i1 = 0, pid1 =  0;                                   //               *pid values*
float p = 0, i = 0, d = 0, pid = 0;                                        //         **kp for rotation error**
float p0 = 0, d0 = 0, i0 = 0, pid0 = 0;                                    //       ***kp1 for back correction***
float kp0 = 5, kd0 = 3, ki0 = 0.8;                                         //    ****kp0 for setpoint correction****
float p2 = 0, d2 = 0, i2 = 0, pid2 = 0;                                    //    *****kp2 for front correction******
float kp2 = 5, kd2 = 3, ki2 = 0.8;                                         //
float kp3 = 8, kd3 = 5, ki3 = 0.4;                                         //
float p3 = 0, d3 = 0, i3 = 0, pid3 = 0;                                    //
//***************************************************************************
int steervalue1 = 0;
int steervalue2 = 0;

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
  totalerror += abs(rotateerror);                      //for rotational error pid

  backerror = setpointb - backsensor;
  totalerrorb += abs(backerror);
  totalerrorleft += abs(error);

  fronterror = setpointf - frontsensor;
  totalerrorf = abs(fronterror);
  calculate();

  if (flag == 100)                                 //************************flag 100 for checking correction functions
  {
    //moveleft();
    leftintegrate();
    //backcorrect();
    //setleft();
  }

  if (flag == 0)                                 //*******************flag0 for rightward move
  {
    if (leftavg > 145)
    {
      leftintegrate();
    }
    else if ((leftavg < 145) && (leftavg > 125))
    {
      //      steerfront1();
      leftcorrect();
    }
    else if ((leftavg < 125) && (leftavg > 0))
    {
      //      flag = 1;
      leftcorrect();
    }
  }

  if (flag == 1)                  //***************************flag 1 for diagoanal left
  {
    if ((backsensor <= 120) && (leftavg > 30))
    {
      leftdia();
      leftcorrect();
    }
    else if ((backsensor > 120) && (leftavg < 30))
    {
      analogWrite(pwm1, 10);
      digitalWrite(dir1, LOW);
      analogWrite(pwm3, 10);
      digitalWrite(dir3, LOW);
      leftcorrect();
      flag = 2;
    }
  }

  if (flag == 2)                          //***********************flag 2 for forward move
  {
    if (backsensor < 170)
    {
      leftcorrect();     /// INTEGRATION NEEDED
      moveforward();
      setleft();
    }
    else if (backsensor > 170)
    {
      flag = 3;
    }

  }
  if (flag == 3)                            //**************************flag 3 for right diagonal movement
  { setpointb = 200;
    if (backsensor < 200)
    { rightdia();
      rightcorrect();
    }
    else if (backsensor > 200)
    {
      flag = 4;
    }
  }
  if (flag == 4)
  { setpointl = 120;
    if (leftavg < 120)
    {
      moveright();
      rightcorrect();
    }
    else if (leftavg >= 120)
      // flag = 5;
      setleft();
    rest();
    backcorrect();


  }
  if (flag == 5)
  { setpointl = 150;
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
      // flag = 7;
      setleft();
      leftcorrect();
      rest();

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
  leftlast = rotateerror;
  lasterrorleft = error;
  lasterrorb = backerror;
  lasterrorf = fronterror;
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

}

void leftintegrate()                         ///****************************for left move with correction in left direction
{

  if (rotateerror < -4)
  { analogWrite(pwm1,  (sp1 + pid3));
    digitalWrite(dir1, HIGH);

    analogWrite(pwm2,  sp2 );
    digitalWrite(dir2, HIGH);
    analogWrite(pwm3, sp1);
    digitalWrite(dir3, HIGH);
    analogWrite(pwm4, sp1);
    digitalWrite(dir4, LOW);
  }
  if (rotateerror > 3)
  {
    analogWrite(pwm2, (pid3 + sp2));
    digitalWrite(dir2, LOW);

    analogWrite(pwm1, pid3);
    digitalWrite(dir1, HIGH);
    analogWrite(pwm3, sp1);
    digitalWrite(dir3, HIGH);
    analogWrite(pwm4, sp1);
    digitalWrite(dir4, LOW);
  }
  if ((rotateerror > -4) && ((rotateerror) > 3)) {
    analogWrite(pwm1, sp1);
    digitalWrite(dir1, HIGH);
    analogWrite(pwm2, sp2);
    digitalWrite(dir2, LOW);
    analogWrite(pwm3, sp1);
    digitalWrite(dir3, HIGH);
    analogWrite(pwm4, sp1);
    digitalWrite(dir4, LOW);
  }
}

void moveright()                         //************************************** moves  right
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
  if (rotateerror < -2) {
    analogWrite(pwm1, pid );
    digitalWrite(dir1, HIGH);
    analogWrite(pwm3, pid);
    digitalWrite(dir3, LOW);
  }
  if (rotateerror > 2) {
    analogWrite(pwm1, pid );
    digitalWrite(dir1, LOW);
    analogWrite(pwm3, pid);
    digitalWrite(dir3, HIGH);
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
  if (rotateerror > 2)      //*******************************************for rotation correction
  {
    p = rotateerror * kp;
    d = (rotateerror - leftlast) * kd;
    i = ((totalerror) * ki);
    pid = (p + i + d);
    if (pid > 120)
      pid = 120;
  }
  if (rotateerror < -2) {
    p = (-rotateerror) * kp;
    d = abs(rotateerror - leftlast) * kd;
    i = ((totalerror) * ki);
    pid = (p + i + d);
    if (pid > 120)
      pid = 120;
  }
  if ( (rotateerror > - 2) && (rotateerror < 2))
  {
    totalerror = 0;
    pid = 0;
  }
  //***********************************************************************for backsensor correction
  if (backerror > 1)
  {
    p1 = backerror * kp1;
    d1 = (backerror - lasterrorb) * kd1;
    i1 = ((totalerrorb) * ki1);
    pid1 = (p1 + i1 + d1 );
    if (pid > 120)
      pid = 120;
  }
  if (backerror < -1) {
    p1 = (-backerror) * kp1;
    d1 = abs(backerror - lasterrorb) * kd1;
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
    p0 = error * kp0;
    d0 = (error - lasterrorleft) * kd0;
    i0 = ((totalerrorleft) * ki0);
    pid0 = (p0 + i0 + d0 );
    if (pid0 > 170)
      pid0 = 170;
  }
  if (error < -1) {
    p0 = (-error) * kp0;
    d0 = abs(error - lasterrorleft) * kd0;
    i0 = ((totalerrorleft) * ki0);
    pid0 = (p0 + i0 + d0);
    if (pid0 > 170)
      pid0 = 170;
  }
  if ( (error > - 1) && (error < 1))
  {
    totalerrorleft = 0;
    pid0 = 0;

  }


  //***********************************************************************for frontsensor correction
  if (fronterror > 1)
  {
    p2 = fronterror * kp2;
    d2 = (fronterror - lasterrorf) * kd2;
    i2 = ((totalerrorf) * ki2);
    pid2 = (p2 + i2 + d2 );
    if (pid2 > 120)
      pid2 = 120;
  }
  if (fronterror < -1) {
    p2 = (-fronterror) * kp2;
    d2 = abs(fronterror - lasterrorf) * kd2;
    i2 = ((totalerrorf) * ki2);
    pid2 = (p2 + i2 + d2);
    if (pid2 > 120)
      pid2 = 120;
  }
  if ( (fronterror > - 1) && (fronterror < 1))
  {
    totalerrorf = 0;
    pid2 = 0;


  }

  if (rotateerror > 2)      //*******************************************for steering correction
  {
    p3 = rotateerror * kp3;
    d3 = (rotateerror - leftlast) * kd3;
    i3 = ((totalerror) * ki3);
    pid3 = (p3 + i3 + d3);
    if (pid3 > 60)
      pid3 = 60;
  }
  if (rotateerror < -2) {
    p3 = (-rotateerror) * kp3;
    d3 = abs(rotateerror - leftlast) * kd3;
    i3 = ((totalerror) * ki3);
    pid3 = (p3 + i3 + d3);
    if (pid3 > 60)
      pid = 60;
  }
  if ( (rotateerror > - 2) && (rotateerror < 2))
  {
    totalerror = 0;
    pid3 = 0;
  }

}

void setleft()

{
  if (error < -2)
  {
    /////////////////////////////////////left;
    digitalWrite(dir1, HIGH);
    analogWrite(pwm1, pid0);
    digitalWrite(dir2, LOW);
    analogWrite(pwm2, (pid0 * 1.1));
  }
  if (error > 2)
  {
    //////////////////////right;
    digitalWrite(dir1, LOW);
    analogWrite(pwm1, pid0);
    digitalWrite(dir2, HIGH);
    analogWrite(pwm2, (pid0 * 1.1));
  }
}



void steerfront1()
{
  analogWrite(pwm1, (sp1 * 1.3));
  digitalWrite(dir1, HIGH);
  analogWrite(pwm2,  sp2);
  digitalWrite(dir2, LOW);
  analogWrite(pwm3, (sp1 * 1.3));
  digitalWrite(dir3, HIGH);
  analogWrite(pwm4, sp1 );
  digitalWrite(dir4, LOW);

}

void backcorrect()
{
  if (backerror < -3)
  {
    analogWrite(pwm1, pid1);
    digitalWrite(dir1, LOW);
    analogWrite(pwm4, ( pid1 * 1.1));
    digitalWrite(dir4, LOW);

  }
  else if (backerror > 3)
  {
    analogWrite(pwm4, pid1);
    digitalWrite(dir4, HIGH);
    analogWrite(pwm1, (pid1 * 1.1));
    digitalWrite(dir1, HIGH);

  }
  else if ((backerror > -3) && (backerror < 3))
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
