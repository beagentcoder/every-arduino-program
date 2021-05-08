unsigned int countJP1 = 0;
byte rxLSA1 = 16;
byte txLSA1 = 17;
byte sEN_LSA1 = 28;
byte jPulse_LSA1 = 26;
char addressLSA1 = 0x01, commandLSA1, dataLSA1;
int posValLSA1;

void sendCommand_LSA1(char, char, char);

void setup() {
  Serial.begin(57600);
  Serial2.begin(57600);
  pinMode(jPulse_LSA1, INPUT_PULLUP);
  pinMode(sEN_LSA1, OUTPUT);
  digitalWrite(jPulse_LSA1, LOW);
  digitalWrite(sEN_LSA1, HIGH);

  attachInterrupt(digitalPinToInterrupt(jPulse_LSA1), count1, RISING);

  commandLSA1 = 'D';
  dataLSA1 = 0x02;
  sendCommand_LSA1(commandLSA1, dataLSA1, addressLSA1);
}

void sendCommand_LSA1(char command, char data, char address) {
  char checksum = address + command + data;
  Serial.write(address);
  Serial.write(command);
  Serial.write(data);
  Serial.write(checksum);
}

void loop() {
 
  
  
  get_value();
  Serial.println(countJP1);
  if (posValLSA1 > 37) {
    right();
  }
  if (posValLSA1 < 33) {
    left();
  }
  if ((posValLSA1 >= 33) && (posValLSA1 >= 33)) {
    forward();
  }




}
void count1() {
  countJP1++;
}
void get_value() {
  while (Serial.available() <= 0)
  {
    digitalWrite(sEN_LSA1, LOW);
    //Getting LSA data....
    while ((Serial2.available() <= 0));
    {
      posValLSA1 = Serial2.read();

      Serial.print("LSA :-> ");
      Serial.println(posValLSA1);
    }
    digitalWrite(sEN_LSA1, HIGH);
  } 
}
void right(){Serial.println("Right");}
void left(){Serial.println("Left");}
void forward(){Serial.println("Forward");}
