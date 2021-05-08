#define outputA 2
#define outputB 3
int counter = 0;
int astate;
int alaststate;


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
int c1 = 27, c2 = 25, c3 = 23;
int a1 = 0, a2 = 0, a3 = 0;
int y=210;


void setup()
{
  Serial.begin(9600);

  pinMode(pwm1, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(dcv1, OUTPUT);
  pinMode(dcv2, OUTPUT);
  pinMode(c1, INPUT);
  pinMode(c2, INPUT);
  pinMode(c3, INPUT);



  analogWrite(pwm1, 0);
  digitalWrite(dir1, LOW);
  analogWrite(pwm2, 0);
  digitalWrite(dir2, LOW);
  analogWrite(pwm3, 0);
  digitalWrite(dir3, LOW);

  digitalWrite(dcv1, LOW);
  digitalWrite(dcv2, LOW);
  
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  attachInterrupt(digitalPinToInterrupt(outputA), value, CHANGE);
  alaststate = digitalRead(outputA);


    attachInterrupt(digitalPinToInterrupt(c1), value, CHANGE);
    attachInterrupt(digitalPinToInterrupt(c2), value, CHANGE);
    attachInterrupt(digitalPinToInterrupt(c3), value, CHANGE);
}



void loop()
{
  if (digitalRead(c1) == HIGH)
  {
      if(a1==0)
      Serial.println("c1 is high");
    a1 = a1 + 1;
    pick();
    
      

  }

  if (digitalRead(c2) == HIGH)
  {

   if(a2==0)
      Serial.println("c2 is high");
    a2 = a2 + 1;
    place();
    
  }
  if (digitalRead(c3) == HIGH)
  {
    if(a3==0)
      Serial.println("c3 is high");
    a3 = a3 + 1;
    throww();
    
  }
  //else Serial.println("Firse");
}
void value()
{
  astate = digitalRead(outputA);
  if (astate != alaststate)
  {
    if (digitalRead(outputB) != astate)
    {
      counter++;
    }
    else {
      counter--;
    }
    //Serial.println("position");
    //Serial.print(counter);
  }
  //Serial.println("position");
  //Serial.print(counter);

  alaststate = astate;

}


void pick()
{
  digitalWrite(dcv1, HIGH);  //piston open
  digitalWrite(dcv2, LOW);
  delay(500);
  //spring extends here
  analogWrite(pwm1, 200);
  digitalWrite(dir1, HIGH);
  delay(800);
  analogWrite(pwm1, 0);

  digitalWrite(dcv1, LOW);   //piston lock
  digitalWrite(dcv2, HIGH);
  delay(500);



  analogWrite(pwm1, 240);    //opp dir
  digitalWrite(dir1, LOW);
  delay(400);
  analogWrite(pwm1, 0);
  delay(400);


  // code for gripper ..........................
  analogWrite(pwm2, 255);
  digitalWrite(dir2, LOW );
  //delay(130);................cmnt
}



void place()
{
  // code for placing.........................
  /*analogWrite(pwm3, 255);
    digitalWrite(dir3, HIGH);
    delay(1680);

    analogWrite(pwm3, 40);  //place to avoid fall
    digitalWrite(dir3, LOW);
    delay(100);
    analogWrite(pwm3, 50);
    digitalWrite(dir3, LOW);
    delay(30);
    analogWrite(pwm3, 0);
    delay(100);*/
  //    analogWrite(pwm2, 0);
  //delay(10000);

  if (counter < y)
  {
    analogWrite(pwm3, 50);
    digitalWrite(dir3, LOW);
  }
  else if (counter > y)
  {
    analogWrite(pwm3, 0);
    delay(110);
    analogWrite(pwm2, 120);  //grip open
    digitalWrite(dir2, HIGH);
    delay(110);
    analogWrite(pwm2, 0);
  }



  if ((counter>y)&&(counter<=y+20))
   {
    analogWrite(pwm3, 50);
    digitalWrite(dir3, LOW);
    
   }
   else if (counter>y+20)
   
  
}


void throww()
{
  digitalWrite(dcv1, HIGH);  //piston open
  digitalWrite(dcv2, LOW);
  //delay(400);............cmnt
}
