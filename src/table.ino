/*
  Control table motor via serial
 */

#include <Arduino.h>

const int pinUp = 2;
const int pinDown = 3;
const int pinFan = 5;
const int trigPin = 9;
const int echoPin = 10;

const int minDist = 55;
const int maxDist = 105;

char receivedChar;
int currentStatus = 0;
unsigned long lastGo = 0;

void fan(int speed) {
  analogWrite(pinFan, speed);
}

int distance() {
  // defines variables
  long duration;
  int distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH) + 10;
  // Calculating the distance
  distance= duration*0.034/2;

  return distance;

}

void setup()  {
  Serial.begin(9600);
  pinMode(pinUp, OUTPUT);
  pinMode(pinDown, OUTPUT);
  pinMode(pinFan, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  // start with relay disconnected
  // signal must be high to be the same status
  digitalWrite(pinUp, HIGH);
  digitalWrite(pinDown, HIGH);

  fan(0);
}

void stop() {
  digitalWrite(pinUp, HIGH);
  digitalWrite(pinDown, HIGH);
  currentStatus = 0;
}

void go(int pinDirection) {
  lastGo = millis();
  if (currentStatus != pinDirection) {
    stop();
    delay(100);
    digitalWrite(pinDirection, LOW);
    currentStatus = pinDirection;
  }
}

void goUp() {
  go(pinUp);
}

void goDown() {
  go(pinDown);
}

// read command from serial
// do never execute more one direction more than 15 seconds
void loop() {

  int dist = distance();

  // get current millisecond of loop
  unsigned long currentMillis = millis();
  if (currentStatus!= 0 && (currentMillis - lastGo) > 15*1000) {
    stop();
  }

  if (currentStatus == pinUp && dist >= maxDist) {
    Serial.print("Stopping while going up beacuse dist =");
    Serial.println(dist);
    stop();
  }

  if (currentStatus == pinDown && dist <= minDist) {
    Serial.print("Stopping while going down beacuse dist =");
    Serial.println(dist);
    stop();
  }


  if (Serial.available() > 0) {
    receivedChar = Serial.read();
    switch(receivedChar) {
      case 'U':
        goUp();
      break;
      case 'D':
        goDown();
      break;
      case '0':
        fan(0);
      break;
      case '1':
        fan(160);
      break;
      case '2':
        fan(215);
      break;
      case '3':
        fan(255);
      break;
      default:
        stop();
      break;
    }
  }


}
