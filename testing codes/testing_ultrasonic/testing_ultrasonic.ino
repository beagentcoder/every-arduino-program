//***********************************************************************************
int trigPin1 = 50;                                                                 //
int echoPin1 = 52;                                                                 //
int trigPin2 = 51;                                                                 //
int echoPin2 = 53;                                                                 //
int trigPin3 = 49;                                                                 //
int echoPin3 = 47;                                                                 //
int trigPin4 = 48;                                                                 // *********these only change when there is any change in pcb******
int echoPin4 = 46;
int trigPin5 =  21;
int echoPin5 =  20;
int trigPin6 =  18;
int echoPin6 =  19;
//
//
int frontled = 43, leftled1 = 37, leftled2 = 33, backled = 29;                     //
//
int dir1 = 38, dir2 = 32, dir3 = 36, dir4 = 34;                                    //
int pwm1 = 9, pwm2 = 8, pwm3 = 10 , pwm4 = 11;                                     //
//***********************************************************************************


int  backsensor, frontsensor, leftsensor1, leftsensor2, leftavg, fronterror, backerror, error = 0, errorback = 0;
double pulse, inches, cm, distance, duration;
int rightsensor1, rightsensor2, rightavg;




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
  pinMode(leftled1, OUTPUT);
  pinMode(leftled2, OUTPUT);
  pinMode(frontled, OUTPUT);
  pinMode(backled, OUTPUT);

}

void loop() {

  Ultrasonic();

  Print();


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
    
  SonarSensor(trigPin5, echoPin5);
  rightsensor1 = distance;
  SonarSensor(trigPin6, echoPin6);
  rightsensor2 = distance;


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

  Serial.print("  rightsensor1   "  );
  Serial.println(rightsensor1);

  Serial.print("  rightsensor2   "  );
  Serial.println(rightsensor2);




  Serial.println("                  ");
  Serial.println("                  ");
  Serial.println("                  ");

}
