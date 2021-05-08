int dir1 = 32;
int pwm1 = 9;




void setup() {
  pinMode(pwm1, OUTPUT);
  pinMode(dir1, OUTPUT);
  
  // put your setup code here, to run once:

}

void loop()

{
  analogWrite(pwm1, 220);
  digitalWrite(dir1, LOW);
 
}
