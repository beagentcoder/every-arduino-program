
void setup() {
  Serial.begin(9600); //Transmit at 9600 Baud Rate
}

boolean Mclick = false;

void loop() {
  int potValue = analogRead(A0); //variable to store potValue
  int FlexValue = analogRead(A1); //variable to store Flex Value
  Serial.print("Flex: ");
 Serial.println(FlexValue);
  delay(1000);
  Serial.print("POT: ");
  Serial.println(potValue);

  Serial.println(FlexValue);

   Serial.println("FlexValue");
  if (FlexValue >= 310 && FlexValue <= 550) //my flex sensor range
  {
    FlexValue = map (FlexValue, 550, 310, 0, 240); //based on bending convert to 0 to 100
     Serial.println(FlexValue);


    if (FlexValue >= 90)
    {    Serial.println(FlexValue);}
            Mclick = true;
      Serial.write(1);  //1 is sent to make mouse left click
      Serial.write(FlexValue); //Flex value is the distance to move mouse in X direction

           Serial.println(FlexValue);
  }
    
  
  else
  {
   Mclick = false;
    Serial.write(0);
  }


  if (potValue <= 200)
  {
    potValue = map(potValue, 0, 200, 250, 350);
    Serial.write(1);
    Serial.write(potValue);
    //    Serial.println(potValue);
  }

  /*if ( potValue<=200)
    {
    potValue = map(potValue,0,200,250,350);
    if(potValue>=260 && potValue<=350){
    Serial.write(1);
    Serial.write(potValue);} //Pot value is the distance to move mouse in Y direction
    }

   /* else
    {
    Mclick=false;
    Serial.write(0);}*/
  

  delay(500);

}
