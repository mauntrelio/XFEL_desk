/*
  Control table motor via serial
 */

#include <Arduino.h>

int pinUp = 2;
int pinDown = 3;
char receivedChar;
int currentStatus = 0;
unsigned long lastGo = 0;

void setup()  {
  Serial.begin(9600);
  pinMode(pinUp, OUTPUT);
  pinMode(pinDown, OUTPUT);
  // start with relay disconnected
  // signal must be high to be the same status
  digitalWrite(pinUp, HIGH);
  digitalWrite(pinDown, HIGH);
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
    delay(10);
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
// do never execute more one direction more than 30 seconds
void loop() {

  // get current millisecond of loop
  unsigned long currentMillis = millis();
  if (currentStatus!= 0 && (currentMillis - lastGo) > 30*1000) {
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
      default:
        stop();
      break;
    }
  }



}
