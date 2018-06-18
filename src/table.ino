/*
  Control table motor via serial
 */

#include <Arduino.h>

const int MAX_MOVING_TIME = 20;
const int minDist = 50;
const int maxDist = 100;

const int pinUp = 2;
const int pinDown = 3;
const int pinFan = 5;
const int trigPin = 9;
const int echoPin = 10;

char receivedChar;
int currentStatus = 0;
unsigned long lastGo = 0;
unsigned long lastPrint = 0;

// control a USB fan
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

  // start with fan off
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
// do never execute more one direction more than MAX_MOVING_TIME seconds
void loop() {

  int height;
  // get current millisecond of loop
  unsigned long currentMillis = millis();

  // if we are moving
  if (currentStatus!= 0) {
    // get the height of the table
    height = distance();
    // print distance from table every half a second
    if ((currentMillis - lastPrint) > 500) {
      Serial.println(height);
      lastPrint = millis();
    }

    // stop moving if we are doing it for more than MAX_MOVING_TIME seconds
    if ((currentMillis - lastGo) > MAX_MOVING_TIME*1000) {
      Serial.println("Stopping while going beacuse max time reached");
      stop();
    }
  }

  // stop raising if height > maxDist
  // if the height measures more than 3000 cm this is a wrong read (misplaced sensor?): ignore it
  if (currentStatus == pinUp && height >= maxDist && height < 3000) {
    Serial.print("Stopping while going up beacuse height =");
    Serial.println(height);
    stop();
  }

  // stop lowering if height < minDist
  if (currentStatus == pinDown && height <= minDist) {
    Serial.print("Stopping while going down beacuse height =");
    Serial.println(height);
    stop();
  }

  // receive command from serial
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
        // switch the fan off
        fan(0);
      break;
      case '1':
        // switch the fan to first level speed
        fan(160);
      break;
      case '2':
        // switch the fan to second level speed
        fan(215);
      break;
      case '3':
        // switch the fan to maximum speed
        fan(255);
      break;
      default:
        stop();
      break;
    }
  }


}
