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


void setup() {
   Serial.begin(9800);

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

void loop() { SonarSensor(trigPin1, echoPin1);
  ls1 = distance;
  SonarSensor(trigPin2, echoPin2);
  ls2 = distance;
  SonarSensor(trigPin3, echoPin3);
  backsensor = distance;
  SonarSensor(trigPin4, echoPin4);
  frontsensor = distance;

  dialeft();
  delay(1500);
  diaright();
  delay(1500);
  dialeft();
  delay(1500);
  diaright();
  delay(1000);
  Stop();




}




void dialeft()
{

  analogWrite(pwm1, 80);

  analogWrite(pwm3, 80);

 analogWrite(pwm2, 0);
  analogWrite(pwm4, 0);
  digitalWrite(dir1, LOW);
  digitalWrite(dir3, HIGH);

}

void diaright()
{
  
  digitalWrite(dir2, LOW);
  digitalWrite(dir4, HIGH);
  analogWrite(pwm2, 80);
  analogWrite(pwm4, 80);
   analogWrite(pwm1, 0);

  analogWrite(pwm3, 0);

  digitalWrite(dir2, LOW);
  digitalWrite(dir4, HIGH);

}




void Stop()
{



  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, 0);


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
