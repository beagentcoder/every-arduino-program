int l=34;
void setup() {
  Serial.begin(9600);
  pinMode(l, OUTPUT);
}

void loop() {
  digitalWrite(l, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(l, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
