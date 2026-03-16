unsigned long startTime;
int ldrReading, pin1, pin2, intervalTime1, intervalTime2;

intervalTime1 = 1000;
intervalTime2 = 500;
pin1 = 10; // red
pin2 = 8; // blue

void setup() {
  Serial.begin(9600);
  startTime = millis();
}

void loop() {
  if (millis() - startTime >= intervalTime1) {
    digitalWrite(pin1, HIGH);
    startTime = millis();
  }
  if (millis() - startTime >= intervalTime2) {
    // ...
    digitalWrite(pin2, HIGH);
    startTime = millis();
  }



  Serial.println();
}