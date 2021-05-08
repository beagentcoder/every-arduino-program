int trigPin1 = 50;
int echoPin1 = 52;
int trigPin2 = 51;
int echoPin2 = 53;
int trigPin3 = 49;
int echoPin3 = 47;
int trigPin4 = 48;
int echoPin4 = 46;
int echoPin5 =  19;
int trigPin5 = 18 ;
int echoPin6 = 20 ;
int trigPin6 = 21 ;  //to be declared

int dir1 = 38, dir2 = 32, dir3 = 36, dir4 = 34;
int pwm1 = 9, pwm2 = 8, pwm3 = 10, pwm4 = 11;

unsigned long dt = 0, present = 0, past = 0;
int lastback, frontsensor, backsensor, errorback, avgrs, ls1, ls2, rs1, rs2, avgls, err = 0;
double pulse, inches, cm, distance, duration;
double lasterrorls = 0, totalerrorls = 0, errorrs = 0, totalerrorrs = 0, lasterrorrs = 0, lastrs = 0;
double lastl1 = 0, lastl2 = 0, lastls = 0, errorls;
int sp1 = 110, sp2 = 140;
float kp = 7.5, kd = 4.2, ki = 0.01;
float p = 0, i = 0, d = 0, pidl = 0, pidr = 0;
float kp0 = 7.8, kd0 = 1.3, ki0 = 0.03;
float p0 = 0, i0 = 0, d0 = 0, pid0 = 0, p1 = 0, i1 = 0, d1 = 0, pid1 = 0;
int error = 0, totalerror = 0, lasterror = 0, error1 = 0, totalerror1 = 0, lasterror1 = 0;
int errorleftsetpt,errorrightsetpt,   totalerrorleftsetpt,  totalerrorrightsetpt;       //// /////////////////////////edited by sam
int sp = 100;
int flag = 0;
int steerr = 1;
int steerl = 1;
int approxtime = 0;
int setpoint1 = 110;
int setpoint2 = 35;


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
  pinMode(trigPin5, OUTPUT);
  pinMode(echoPin5, INPUT);
  pinMode(trigPin6, OUTPUT);
  pinMode(echoPin6, INPUT);

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
  SonarSensor(trigPin5, echoPin5);
  rs1 = distance;
  SonarSensor(trigPin6, echoPin6);
  rs2 = distance;
  SonarSensor(trigPin1, echoPin1);
  ls1 = distance;
  SonarSensor(trigPin2, echoPin2);
  ls2 = distance;
  SonarSensor(trigPin3, echoPin3);
  backsensor = distance;
  SonarSensor(trigPin4, echoPin4);
  frontsensor = distance;

  avgrs = (rs1 + rs2) / 2;
  avgls = (ls1 + ls2) / 2;
  errorrs = rs2 - rs1;
  errorls = ls2 - ls1;

  totalerrorrs += abs(errorrs);
  totalerrorls += abs(errorls);

  errorleftsetpt = setpoint1 - avgls;
  totalerrorleftsetpt += abs(errorleftsetpt);
  
  errorrightsetpt = setpoint2 - avgrs;
  totalerrorrightsetpt += abs(errorrightsetpt);
  

  error1 = setpoint2 - backsensor;
  totalerror1 += abs(error1);

  cal();   // calculating pid
  Serial.println(flag);

  if ( flag == 100)
  {
    correctr();
    //    correctl();
    //    setpt1();
    //    forward();
  }

  if (flag == 0) {
    correctr();
    if ((errorls > -5) && (errorls < 5))
      flag = 1;
  }

  if (flag == 1)  {
    if (((backsensor < 110) && (avgls < 110)) ||(avgrs>30))
    {
      /*correctr();
        diaright();
        if (((backsensor < 25) && (avgls < 40)) /*|| ((backsensor > 90) && (backsensor < 100))) {
        steerr = 0;
        if (steerr == 0) {
          steerright();
        }
        }
        if (((backsensor > 25) && (avgls > 40) && (backsensor < 90)) || (backsensor > 100))) {
        steerr = 1;
        }
        }

        else if ((backsensor >= 70 ) && (avgls >= 110))  {
      analogWrite(pwm2, 120);                              // Just a high pulse for crossing the first pole
        digitalWrite(dir2, HIGH);
        analogWrite(pwm4, 120);
        digitalWrite(dir4, HIGH);*/
      correctr();
      diaright();
    }
    else if ((backsensor > 110)||(avgrs<=35))
    {
      //      set_zero();
      analogWrite(pwm2, 60);                              // Just a high pulse for crossing the first pole
      digitalWrite(dir2, HIGH);
      analogWrite(pwm4, 60);
      digitalWrite(dir4, LOW);
      correctr();
//      setpt() ;
      if ((errorls > -5) && (errorls < 5)) {
       flag = 2;
        set_zero();
      }
    }
  }

 /* if (flag == 2)  {
    if (backsensor < 121)   {
      forward();
      //      setpt();
    }
    else if (backsensor > 121) {
      set_zero();
      correctr();
      //      correctl();
      if ((errorls > -5) && (errorls < 5)) {
        setpt();
        //        set_zero();
        flag = 3;
      }
    }
  }

  if (flag == 3 ) {
    if ((backsensor < 250) || (avgls > 91))  {
      dialeft();
      correctl();
      if ((avgls < 130) && (avgls > 90)) {
        setpoint2 = backsensor;
        error1 = setpoint2 - backsensor;
        totalerror1 += abs(error1);
        steerl = 0;
        if (steerl == 0)
          steerleft();

      }

      else if (avgls < 90) {
        correctl();
        correctr();
        setpt1();
        steerl = 1;
        correctl();
        analogWrite(pwm1, 50);                              // Just a high pulse for crossing the first pole
        digitalWrite(dir1, HIGH);
        analogWrite(pwm3, 50);
        digitalWrite(dir3, LOW);
        setpoint1 = 70;
        setpt();
        //        set_zero();
        correctr();
        correctl();
        //        if ((errorls > -5) && (errorls < 5))
        flag = 4;
        //        flag = 100;
      }
    }
     else if ((backsensor > 150) || (avgls <= 100))  {
        //      set_zero();
        correctr();
        correctl();
        flag = 4;

        setpt();
      }
  }


  if (flag == 4)
  {
    if (backsensor < 320)   {
      forward();
    }
    else if (backsensor > 320) {
//      setpt();
      correctl();
      correctr();
      flag = 5;
    }
  }

  if (flag == 5)
  {
    if ( (avgls < 140)) { //increased range due to the unavailability of the y axis sensor
      //            correctr();
      //      diaright();
      if (((avgls < 140) && (avgls > 40))) {
        //        steerl = 0;
        //        if (steerl == 0) {
        //          steerleft();
        //        }
        right();
      }
      if ((avgls > 140)) {
        steerl = 1;
        setpoint1 = 140;
        //        setpt();
        flag = 6;

      }
    }
     else if ((avgls >= 110))  {
        set_zero();
        correctr();
        flag = 100;
      }

  }

  if (flag == 6)
  {
    approxtime = millis();
    if (millis < 900)   {   //no feedback that's why using millis function here after the third pole
      forward();
    }
    else if (millis > 900) {
      //      setpoint1 = 110;
      correctl();
      correctr();
//      setpt();
      flag = 100;
      //      check();
    }
  }

  if (flag == 7)
  {
    if ((avgls > 40))  {    // to be used for aligning with the river area
      dialeft();
      correctl();
      if ((avgls < 50)) {
        steerl = 0;
        if (steerl == 0) {
          steerleft();
          if ((avgls > 50)) {
            steerl = 1;
          }
        }
      }
    }
    else if (avgls <= 40)  {
      analogWrite(pwm1, 0);
      analogWrite(pwm3, 0);
      // flag = 100;
    }
  }

  if (flag == 8)
  {
    // setpoint pid will be used for this case
    forward();
    // bot will be moving through the river area
    if (frontsensor < 500) {
      correctl();
      check();
    }
  }

  if (flag == 9)
  {
    if (frontsensor < 160)   {
      forward();
    }
    else if (frontsensor > 160) {
      correctl();
      check();
    }
  }

  if (flag == 10)
  {
    if ((avgls < 120))  { //increased range due to the unavailability of the y axis sensor
      correctr();
      diaright();
      if ((avgls < 50)) {
        steerr = 0;
        if (steerr == 0) {
          steerright();
          if ((avgls > 50)) {
            steerr = 1;
          }
        }
      }
    }

    else if ((avgls >= 110))  {
      analogWrite(pwm2, 120);                              // Just a high pulse for crossing the third pole
      digitalWrite(dir2, HIGH);
      analogWrite(pwm4, 120);
      digitalWrite(dir4, HIGH);
      if (avgls > 120) {
        correctr();
        check();
      }
    }
  }

  if (flag == 11)
  {
    if (avgls < 300)   {
      right();
    }
    else if (avgls > 300) {
      correctl();
      check();
    }
  }

  if (flag == 12)
  {
    correctl();   // stop condition for now
  }
*/

  lasterrorls = errorls;
  lasterrorrs = errorrs;

  lastrs = avgrs;
  lastls = avgls;
  lasterror = error;
  lasterror1 = error1;
  Print();
  //  Serial.println(error);

}


void check()
{
  // correctr();
  if ((errorls > -3) && (errorls < 3))
    flag = flag + 1;
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

void Print() {
  Serial.print("   RightSensor1  ");
  Serial.print(rs1);
  Serial.print("  RightSensor2   ");
  Serial.print(rs2);
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
  Serial.print("   average right" );
  Serial.println(avgrs);
  Serial.print("   flag==    ");
  Serial.println(flag);
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

void setpt1()
{
  if (error1 < -5)
  {
    //    analogWrite(pwm1, pid1);
    analogWrite(pwm2, pid1);
    //    analogWrite(pwm3, pid1);
    analogWrite(pwm4, pid1);
    //    digitalWrite(dir1, HIGH);
    digitalWrite(dir2, HIGH);
    //    digitalWrite(dir3, LOW);
    digitalWrite(dir4, LOW);


  }
  if (error1  > 5)
  {
    //    analogWrite(pwm1, pid1);
    analogWrite(pwm2, pid1);
    //    analogWrite(pwm3, pid1);
    analogWrite(pwm4, pid1);
    //    digitalWrite(dir1, LOW);
    digitalWrite(dir2, LOW);
    //    digitalWrite(dir3, HIGH);
    digitalWrite(dir4, HIGH);
  }

  if ((error1 < 5) && (error1 > -5))
  {
    /* analogWrite(pwm1, pidl);
      analogWrite(pwm2, pidl);
      analogWrite(pwm3, pidl);
      analogWrite(pwm4, pidl);*/
    //    correctl();
    //    correctr();
    set_zero();
    pid1 = 0;
    totalerror1 = 0;
  }
}

void setptleft()

{
  if (errorleftsetpt < -3)
  {
    //    analogWrite(pwm1, pid0);
    //    analogWrite(pwm2, pid0);
    analogWrite(pwm3, pid0);
    analogWrite(pwm4, pid0);
    //    digitalWrite(dir1, LOW);
    //    digitalWrite(dir2, HIGH);
    digitalWrite(dir3, HIGH);
    digitalWrite(dir4, LOW);


  }
  if (errorleftsetpt > 3)
  {
    //    analogWrite(pwm1, pid0);
    //    analogWrite(pwm2, pid0);
    analogWrite(pwm3, pid0);
    analogWrite(pwm4, pid0);
    //    digitalWrite(dir1, HIGH);
    //    digitalWrite(dir2, LOW);
    digitalWrite(dir3, LOW);
    digitalWrite(dir4, HIGH);
  }

  if ((errorleftsetpt < 3) && (errorleftsetpt > -3))
  {
    /* analogWrite(pwm1, pidl);
      analogWrite(pwm2, pidl);
      analogWrite(pwm3, pidl);
      analogWrite(pwm4, pidl);*/
    correctl();
    correctr();
    //    set_zero();
    pid0 = 0;
    totalerrorleftsetpt = 0;
  }
}


void setptright()
{
  if (errorrightsetpt < -3)
  {
        analogWrite(pwm1, pid0);
       analogWrite(pwm2, pid0);
//    analogWrite(pwm3, pid0);
//    analogWrite(pwm4, pid0);
        digitalWrite(dir1, LOW);
        digitalWrite(dir2, HIGH);
//    digitalWrite(dir3, HIGH);
//    digitalWrite(dir4, LOW);


  }
  if (errorrightsetpt > 3)
  {
        analogWrite(pwm1, pid0);
        analogWrite(pwm2, pid0);
//    analogWrite(pwm3, pid0);
//    analogWrite(pwm4, pid0);
        digitalWrite(dir1, HIGH);
        digitalWrite(dir2, LOW);
//    digitalWrite(dir3, LOW);
//    digitalWrite(dir4, HIGH);
  }

  if ((errorrightsetpt < 3) && (errorrightsetpt > -3))
  {
    /* analogWrite(pwm1, pidl);
      analogWrite(pwm2, pidl);
      analogWrite(pwm3, pidl);
      analogWrite(pwm4, pidl);*/
    correctl();
    correctr();
        set_zero();
    pid0 = 0;
    totalerrorrightsetpt = 0;
  }
}


void correctl()
{
  if (errorls < -2 )//|| errorrs < -2 )
  {
    analogWrite(pwm4, pidl );
    digitalWrite(dir2, LOW);
    analogWrite(pwm2, pidl / 1.4);
    digitalWrite(dir4, LOW);
  }

  if (errorls > 2 )//|| errorrs > 2)
  {
    analogWrite(pwm4, pidl );
    digitalWrite(dir2, HIGH);
    analogWrite(pwm2, pidl / 1.4);
    digitalWrite(dir4, HIGH);
  }

  if ((errorls > -2) && (errorls < 2))// || (errorrs > -2) && (errorrs < 2))
  {
    if (steerl == 1) {
      analogWrite(pwm2, 20);
      digitalWrite(dir2, HIGH);
      analogWrite(pwm4, 20);
      digitalWrite(dir4, LOW);
    }

    totalerrorls  = 0;
    totalerrorrs = 0;
  }
}


void correctr()
{
  if (errorrs < -2 )//|| errorrs < -2 )
  {
    analogWrite(pwm3, pidl );
    digitalWrite(dir3, LOW);
    analogWrite(pwm1, pidl / 1.4);
    digitalWrite(dir1, LOW);
  }

  if (errorrs > 2 )//|| errorrs > 2)
  {
    analogWrite(pwm3, pidl );
    digitalWrite(dir3, HIGH);
    analogWrite(pwm1, pidl / 1.4);
    digitalWrite(dir1, HIGH);
  }

  if ((errorrs > -2) && (errorrs < 2))// || (errorrs > -2) && (errorrs < 2))
  {
//    if (steerl == 1) {
      analogWrite(pwm1, 0);
      digitalWrite(dir1, HIGH);
      analogWrite(pwm3, 0);
      digitalWrite(dir3, LOW);
//    }

    
  totalerrorls = 0;
  totalerrorrs = 0;

}

}
void cal()
{
  present = millis();
  //    Serial.println(p0);
  //  Serial.println(i0);
  //  Serial.println(d0);
  Serial.println(error1);
  //  Serial.println(pid0);
  dt = (present - past);
  if (errorls > 0)
  {
    p = (errorls) * kp;
    d = ((errorls - lasterrorls) * kd) / dt;
    i =  ((totalerrorls) * ki) * dt ;
    pidl = (p + i + d);

    if (pidl > 70) {
      pidl = 70;        //
      totalerrorls  = 0;
      totalerrorrs = 0;
    }
  }

  if (errorls < 0)
  {
    p = -(errorls) * kp;
    d = (abs((errorls - lasterrorls)) * kd) / dt;
    i =  ((totalerrorls) * ki) * dt ;
    pidl = (p + i + d);

    if (pidl > 70) {
      pidl = 70;        //
      totalerrorls  = 0;
      totalerrorrs = 0;
    }
  }

  if (errorrs > 0)
    {
      p = errorrs * kp;
      d = ((errorrs - lasterrorrs) * kd) / dt;
      i =  (totalerrorrs * ki) * dt;
      pidr = (p + i + d);
      if (pidr > 120) {
        pidr = 120;        //
        totalerrorls  = 0;
        totalerrorrs = 0;
      }
    }

    if (errorrs < 0)
    {
      p = (-errorrs) * kp;
      d = (abs(errorrs - lasterrorrs) * kd) / dt;
      i =  (totalerrorrs * ki) * dt;
      pidr = (p + i + d);
      if (pidl > 120) {
        pidl = 120;        //
        totalerrorls  = 0;
        totalerrorrs = 0;
      }
    }

  
  if (error > 0)
  {
    p0 = error * kp0;
    d0 = ((error - lasterror) * kd0) / dt;
    i0 =  ((totalerror) * ki0) * dt;
    pid0 = (p0 + i0 + d0);
    if (pid0 > 60) {
      pid0 = 60;
      totalerror = 0;
    }
  }

  if (error < 0)
  {
    p0 = (-error) * kp0;
    d0 = (abs(error - lasterror) * kd0) / dt;
    i0 =  ((totalerror) * ki0) * dt;
    pid0 = (p0 + i0 + d0);
    if (pid0 > 60) {
      pid0 = 60;
      totalerror = 0;
    }
  }

  if (error1 > 0)
  {
    p1 = error1 * kp0;
    d1 = ((error1 - lasterror1) * kd0) / dt;
    i1 =  ((totalerror1) * ki0) * dt;
    pid1 = (p1 + i1 + d1);
    if (pid1 > 40) {
      pid1 = 40;
      totalerror1 = 0;
    }
  }

  if (error1 < 0)
  {
    p1 = (-error1) * kp0;
    d1 = (abs(error1 - lasterror1) * kd0) / dt;
    i1 =  ((totalerror1) * ki0) * dt;
    pid1 = (p1 + i1 + d1);
    if (pid1 > 40) {
      pid1 = 40;
      totalerror1 = 0;
    }
  }
  past = present;

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

void steerleft()
{
  digitalWrite(dir2, HIGH);
  digitalWrite(dir4, LOW);
  analogWrite(pwm2, sp);
  analogWrite(pwm4, sp);
}

void set_zero()
{

  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, 0);
}
