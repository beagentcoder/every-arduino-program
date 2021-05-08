int trigPin = 18;                                                                 //
int echoPin = 19;


int rightsensor,rightsensor2;
double pulse, inches, cm, distance, duration;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
  SonarSensor(trigPin,echoPin);
  rightsensor=distance;


  


 Serial.println("      rightsensor1      ");
 Serial.println(rightsensor);
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
