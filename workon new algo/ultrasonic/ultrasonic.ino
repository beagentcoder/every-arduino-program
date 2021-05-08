int trigPin4 = 48;
int echoPin4 = 46;
int frontsensor;
double pulse, inches, cm, distance, duration;
void setup() {
  Serial.begin(9600);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);


}

void loop() {
  SonarSensor(trigPin4, echoPin4);
  frontsensor = distance;

  Serial.print("  frontsensor   "  );
  Serial.println(frontsensor);


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
