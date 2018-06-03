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
 
  if (Serial.available())
  {
    int angle = Serial.read()*0.5+90-motorA;
    motorA = servoMove(servoA, angle, motorA);
  }
  if (motorA > 180)
    motorA = 0;
   delay(500);
}

