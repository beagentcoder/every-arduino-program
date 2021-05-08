int pwm1 = 13;         // motor conneccted with relay_ lower
int dir1 = 30 ;

int pwm2 = 12;
int dir2 = 34;        // motor connection of gripper

int pwm3 = 10;                   //rotator
int dir3 = 28 ;

int dcv1 = 52;
int dcv2 = 53;     //piston

int testing = 0;
int grip = 1;
int c1=27,c2=25,c3=23;


void setup()
{
  pinMode(pwm1, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(dcv1, OUTPUT);
  pinMode(dcv2, OUTPUT);


  analogWrite(pwm1, 0);
  //  digitalWrite(dir1, LOW);
  analogWrite(pwm2, 0);
  //  digitalWrite(dir2, LOW);
  analogWrite(pwm3, 0);
  //  digitalWrite(dir3, LOW);

  digitalWrite(dcv1, LOW);
  digitalWrite(dcv2, LOW);


  // put your setup code here, to run once:

}
void loop()

{
  if (testing == 0)
  {

    digitalWrite(dcv1, HIGH);
    digitalWrite(dcv2, LOW);
    delay(500);
    //spring extends here
    analogWrite(pwm1, 200);
    digitalWrite(dir1, HIGH);
    delay(800);
    analogWrite(pwm1, 0);

    digitalWrite(dcv1, LOW);
    digitalWrite(dcv2, HIGH);
    delay(500);



    analogWrite(pwm1, 240);
    digitalWrite(dir1, LOW);
    delay(400);
    analogWrite(pwm1, 0);
    delay(400);


    // code for gripper ..........................
    analogWrite(pwm2, 255);
    digitalWrite(dir2, LOW );
    delay(130);

    // code for placing.........................
    analogWrite(pwm3, 255);
    digitalWrite(dir3, HIGH);
    delay(1680);

    analogWrite(pwm3, 40);
    digitalWrite(dir3, LOW);
    delay(100);
    analogWrite(pwm3, 50);
    digitalWrite(dir3, LOW);
    delay(30);
    analogWrite(pwm3, 0);
    delay(100);
    //    analogWrite(pwm2, 0);
    delay(10000);
    analogWrite(pwm2, 120);
    digitalWrite(dir2, HIGH);
    delay(110);
    analogWrite(pwm2, 0);
    //    analogWrite(pwm3, 100);
    //    digitalWrite(dir3, LOW);
    //    delay(500);
    //    analogWrite(pwm3, 50);
    //    digitalWrite(dir3, HIGH );
    //    delay(90);
    //    analogWrite(pwm3, 0);



    digitalWrite(dcv1, HIGH);
    digitalWrite(dcv2, LOW);
    delay(400);
    testing = 1;

  }

  if (grip == 0)
  {
    // code for gripper ..........................
    analogWrite(pwm2, 255);
    digitalWrite(dir2, LOW );
    delay(90);

    // code for placing.........................
    analogWrite(pwm3, 242);
    digitalWrite(dir3, LOW );
    delay(1320);

    analogWrite(pwm3, 40);
    digitalWrite(dir3, HIGH);
    delay(150);
    analogWrite(pwm3, 50);
    digitalWrite(dir3, HIGH);
    delay(30);
    analogWrite(pwm3, 0);
    delay(100);
    analogWrite(pwm2, 0);
    delay(200);
    analogWrite(pwm2, 70);
    digitalWrite(dir2, HIGH);
    delay(80);
    analogWrite(pwm2, 0);
    analogWrite(pwm3, 100);
    digitalWrite(dir3, HIGH);
    delay(500);
    analogWrite(pwm3, 50);
    digitalWrite(dir3, LOW );
    delay(90);
    analogWrite(pwm3, 0);


    grip = 1;
  }
}


/*void grip () {

  analogWrite(pwm2, 50);
    digitalWrite(dir2, LOW );
    delay(500);

  }

  void extend() {
  analogWrite(pwm1, 200);
    digitalWrite(dir1, HIGH);
    delay(600);
    analogWrite(pwm1, 0);

  }
  void piston() {

  digitalWrite(dcv1, HIGH);
    digitalWrite(dcv2, LOW);
    delay(800);


  }
  void placing() {
   analogWrite(pwm3, 150);
    digitalWrite(dir3, LOW );
    delay(500);
    analogWrite(pwm3, 50);
    digitalWrite(dir3, LOW);
    delay(300);
    analogWrite(pwm3, 0);

  }*/
