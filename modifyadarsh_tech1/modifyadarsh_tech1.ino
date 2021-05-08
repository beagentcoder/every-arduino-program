const int Kp = 3.5;   // Kp value that you have to change
const int Kd = 2;   // Kd value that you have to change
const int setPoint = 35;    // Middle point of sensor array
const int baseSpeed = 50;    // Base speed for your motors
const int maxSpeed = 80;   // Maximum speed for your motors
int laste = 0;
const byte rx = 17;    // Defining pin 0 as Rx
const byte tx = 16;    // Defining pin 1 as Tx
const byte serialEn = 28;    // Connect UART output enable of LSA08 to pin 2
const byte junctionPulse = 26;   // Connect JPULSE of LSA08 to pin 4
const byte dir1 = 8;   // Connect DIR1 of motor driver to pin 13
const byte dir2 = 10;   // Connect DIR2 of motor driver to pin 12
const byte pwm1 = 9;   // Connect PWM1 of motor driver to pin 11
const byte pwm2 = 11;   // Connect PWM2 of motor driver to pin 10

int fspeed = 60;

int flag = 0;

int error = 0; 
int motorSpeed = 0;

// Adjust the motor speed based on calculated value
// You might need to interchange the + and - sign if your robot move in opposite direction
int rightMotorSpeed = baseSpeed - motorSpeed;
int leftMotorSpeed = baseSpeed + motorSpeed;

int jcount = 0;

void setup() {
  pinMode(serialEn, OUTPUT);  // Setting serialEn as digital output pin
  pinMode(junctionPulse, INPUT);  // Setting junctionPulse as digital input pin

  // Setting pin 10 - 13 as digital output pin
  for (byte i = 10; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
  // Setting initial condition of serialEn pin to HIGH
  digitalWrite(serialEn, HIGH);

  // Setting the initial condition of motors
  // make sure both PWM pins are LOW
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);

  // State of DIR pins are depending on your physical connection
  // if your robot behaves strangely, try changing thses two values
  analogWrite(dir1, 0);
  analogWrite(dir2, 0);

  // Begin serial communication with baudrate 9600
  Serial2.begin(57600);
  Serial.begin(57600);
  attachInterrupt(digitalPinToInterrupt(junctionPulse), count1, RISING);
}

int lastError = 0;    // Declare a variable to store previous error

void loop() {

  digitalWrite(serialEn, LOW);  // Set serialEN to LOW to request UART data
  while (Serial2.available() <= 0);  // Wait for data to be available
  int positionVal = Serial2.read();    // Read incoming data and store in variable positionVal
  digitalWrite(serialEn, HIGH);   // Stop requesting for UART data
  Serial.print(error);
  Serial.print("    ");
  Serial.println(flag);

  error = positionVal - setPoint;   // Calculate the deviation from position to the set point
  motorSpeed = Kp * error + Kd * (error - lastError);   // Applying formula of PID
  lastError = error;    // Store current error as previous error for next iteration use

  // Adjust the motor speed based on calculated value
  // You might need to interchange the + and - sign if your robot move in opposite direction

  if (positionVal != 255) {
    laste = error;
  }
  // Writing the motor speed value as output to hardware motor

  if (flag == 0) {

    rightMotorSpeed = baseSpeed - motorSpeed;
    leftMotorSpeed = baseSpeed + motorSpeed;

    if (rightMotorSpeed > maxSpeed) rightMotorSpeed = maxSpeed;
    if (leftMotorSpeed > maxSpeed) leftMotorSpeed = maxSpeed;

    if (rightMotorSpeed < 0) rightMotorSpeed = 0;
    if (leftMotorSpeed < 0) leftMotorSpeed = 0;
    followline();
   /* if (digitalRead(junctionPulse) == HIGH) {
      delay(100);
      if (error > 0) {
        flag = 1;
      }
      if (error < 0) {
        flag = 2;
      }
      if (error == 0) {
        flag = 1;
      }
    }*/
}
if (flag == 1) {
    rightMotorSpeed = fspeed;
    leftMotorSpeed = fspeed;
    right();

    if ((positionVal <= 40)) {
      flag = 0;
    }
    /*if (digitalRead(junctionPulse) == HIGH) {
      if (error > 0) {
        flag = 1;
      }
      if (error < 0) {
        flag = 2;
      }
      if (error == 0) {
        flag = 1;
      }
      }*/
  }
  if (flag == 2) {
    rightMotorSpeed = fspeed;
    leftMotorSpeed = fspeed;
    left();

    if (positionVal >= 40){
      flag = 0;
    }
    /*if (digitalRead(junctionPulse) == HIGH) {
      if (error > 0) {
        flag = 1;
      }
      if (error < 0) {
        flag = 2;
      }
      if (error == 0) {
        flag = 1;
      }
      }*/
  }
}

void followline() {
  analogWrite(dir1, 0);
  analogWrite(dir2, 0);
  analogWrite(pwm1, rightMotorSpeed);
  analogWrite(pwm2, leftMotorSpeed);
}
void left() {
  analogWrite(dir1, 0);
  analogWrite(dir2, 255);
  analogWrite(pwm1, rightMotorSpeed);
  analogWrite(pwm2, leftMotorSpeed);
}
void right() {
  analogWrite(dir1, 255);
  analogWrite(dir2, 0);
  analogWrite(pwm1, rightMotorSpeed);
  analogWrite(pwm2, leftMotorSpeed);
}
void count1() {
  jcount++;
     
      if (error > 0) {
        flag = 1;
      }
      if (error < 0) {
        flag = 2;
      }
      if (error == 0) {
        flag = 1;
      }
    
}
