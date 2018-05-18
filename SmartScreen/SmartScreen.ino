#include <Servo.h>

int motorA = 0;

Servo servoA;

void setup()
{
  Serial.begin(9600);
  servoA.attach(10);
}

void loop()
{
  motorA = servoMove(servoA, 30, motorA);
  Serial.println(motorA);
  delay(5000);
  if (motorA > 180)
    motorA = 0;
}

