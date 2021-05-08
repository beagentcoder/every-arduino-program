#define outputA 20
#define outputB 21
int counter = 0;
int astate;
int alaststate;

void setup()
{
  Serial.begin(9600);
  
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  attachInterrupt(digitalPinToInterrupt(outputA), value, CHANGE);
  
  alaststate = digitalRead(outputA);
}
void loop()
{
   Serial.println("position");
   Serial.print(counter);
   //value();
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
