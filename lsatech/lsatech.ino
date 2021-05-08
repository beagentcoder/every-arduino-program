const int Kp = 1;   // Kp value that you have to change
const int Kd = 1;   // Kd value that you have to change
const int setPoint = 35;    // Middle point of sensor array
const int baseSpeed = 100;    // Base speed for your motors
const int maxSpeed = 200;   // Maximum speed for your motors
unsigned int junctionCount = 0;  
const byte rx = 17;    // Defining pin 0 as Rx
const byte tx = 16;    // Defining pin 1 as Tx
const byte serialEn = 28;    // Connect UART output enable of LSA08 to pin 2
const byte junctionPulse = 26;   // Connect JPULSE of LSA08 to pin 4
const byte dir1 = 13;   // Connect DIR1 of motor driver to pin 13
const byte dir2 = 12;   // Connect DIR2 of motor driver to pin 12
const byte pwm1 = 11;   // Connect PWM1 of motor driver to pin 11
unsigned int countJP1 = 0;
const byte pwm2 = 10;   // Connect PWM2 of motor driver to pin 10

char addressLSA1 = 0x01;
char commandLSA1, dataLSA1;

// Update commands for settings of LSA through codes
void sendCommand_LSA1(char, char, char);

void setup() 
{
  pinMode(serialEn,OUTPUT);   // Setting serialEn as digital output pin
  pinMode(junctionPulse,INPUT);   // Setting junctionPulse as digital input pin

  // Setting pin 10 - 13 as digital output pin
  for(byte i=10;i<=13;i++) {
    pinMode(i,OUTPUT);
  }

    // Setting initial condition of serialEn pin to HIGH
    digitalWrite(serialEn,HIGH);
    pinMode(junctionPulse,INPUT);
      attachInterrupt(digitalPinToInterrupt(junctionPulse), count1, RISING);
  commandLSA1 = 'D';
  dataLSA1 = 0x02;
  sendCommand_LSA1(commandLSA1, dataLSA1, addressLSA1);
  // Setting the initial condition of motors
  // make sure both PWM pins are LOW
  digitalWrite(pwm1,LOW);
  digitalWrite(pwm2,LOW);
   digitalWrite(junctionPulse, LOW);

  // State of DIR pins are depending on your physical connection
  // if your robot behaves strangely, try changing thses two values
  digitalWrite(dir1,LOW);
  digitalWrite(dir2,LOW);

  // Begin serial communication with baudrate 9600
  Serial.begin(57600);
    Serial2.begin(57600);
    clearJunction();

}

int lastError = 0;    // Declare a variable to store previous error

void loop() {
  Serial.print("\n aaaa1111");
    if(digitalRead(junctionPulse)) {
   
      moveForward();
 
    
    // Retrieve the junction count from LSA08
    // You can do whatever you want with the junction count
    junctionCount = getJunction();
       Serial.print("aaaa\n" ); 
    Serial.println( junctionCount); 
  }
   Serial.print("ppppp/n" ); 
  digitalWrite(serialEn,LOW);   // Set serialEN to LOW to request UART data
  while(Serial2.available() <= 0);   // Wait for data to be available
   int positionVal = Serial2.read(); 
    Serial.println(positionVal);// Read incoming data and store in variable positionVal
  digitalWrite(serialEn,HIGH);    // Stop requesting for UART data

  // If no line is detected, stay at the position
  if(positionVal == 255) {
    analogWrite(pwm1,0);
    analogWrite(pwm2,0);
  }

  // Else if line detected, calculate the motor speed and apply
  else {
    int error = positionVal - setPoint;   // Calculate the deviation from position to the set point
    int motorSpeed = Kp * error + Kd * (error - lastError);   // Applying formula of PID
    lastError = error;    // Store current error as previous error for next iteration use

    // Adjust the motor speed based on calculated value
    // You might need to interchange the + and - sign if your robot move in opposite direction
    int rightMotorSpeed = baseSpeed - motorSpeed;
    int leftMotorSpeed = baseSpeed + motorSpeed;

    // If the speed of motor exceed max speed, set the speed to max speed
    if(rightMotorSpeed > maxSpeed) rightMotorSpeed = maxSpeed;
    if(leftMotorSpeed > maxSpeed) leftMotorSpeed = maxSpeed;

    // If the speed of motor is negative, set it to 0
    if(rightMotorSpeed < 0) rightMotorSpeed = 0;
    if(leftMotorSpeed < 0) leftMotorSpeed = 0;

    // Writing the motor speed value as output to hardware motor
    analogWrite(pwm1,rightMotorSpeed);
    analogWrite(pwm2,leftMotorSpeed);
  }
}
  void clearJunction() {
  char address = 0x01;
  char command = 'X';
  char data = 0x00;
  char checksum = address + command + data;

  Serial2.write(address);
  Serial2.write(command);
  Serial2.write(data);
  Serial2.write(checksum);
}

// Function to retrieve junction count from LSA08
int getJunction() {
  char address = 0x01;
  char command = 'X';
  char data = 0x01;
  char checksum = address + command + data;

  Serial2.write(address);
  Serial2.write(command);
  Serial2.write(data);
  Serial2.write(checksum);

  while(Serial2.available() <= 0);
  return (int(Serial2.read()));
}

void moveForward()
{
      analogWrite(pwm1,200);
    analogWrite(pwm2,200);
}
void count1() {
  countJP1++;
}
void sendCommand_LSA1(char command, char data, char address) {
  char checksum = address + command + data;

  Serial1.write(address);
  Serial1.write(command);
  Serial1.write(data);
  Serial1.write(checksum);
}
