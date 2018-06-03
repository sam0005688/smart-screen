#define COLLISIONDISTANCE 15
#include <Servo.h>

class Ultrasonic {
  private:
    int pinTrig, pinEcho;
    int duration;
    float distance;
    const float distanceMax = 100;

  public:
    void init(int trig, int echo) {
      pinTrig = trig;
      pinEcho = echo;
      pinMode (pinTrig, OUTPUT);
      pinMode (pinEcho, INPUT);
    }

    float distanceCM() {
      digitalWrite(pinTrig, LOW);
      delayMicroseconds(2);
      digitalWrite(pinTrig, HIGH);
      delayMicroseconds(10);
      digitalWrite(pinTrig, LOW);
      duration = pulseIn(pinEcho, HIGH, 5820);
      if (duration == 0) return distanceMax;
      else {
        distance = duration / 58.2;
        if (distance > distanceMax) return distanceMax;
        else return distance;
      }
    }
};


Servo servoA;
int motorA = 0;

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

int servoMove(Servo servo, int angle, int angleNow)
{
  Ultrasonic ult;
  ult.init(12, 11);
  int i;
  for(i = 0;i< angle;i++)
  {
    delay(10);
    servo.write(angleNow + i);
    Serial.println(ult.distanceCM());
    if (ult.distanceCM() < COLLISIONDISTANCE)
      break;
  }
  return angleNow + i;
}