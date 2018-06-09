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


Servo servoRot, servoUD, servoFB;
Ultrasonic ult;
int servoRotA = 70, servoUDA = 50, servoFBA = 90;

void setup()
{
  Serial.begin(9600);
  pinMode(13, INPUT);
  servoRot.attach(10);
  servoUD.attach(11);
  servoFB.attach(12);
  ult.init(8, 9);
}

void loop()
{
  if (Serial.available())
  {
    digitalWrite(13, HIGH);
    char cmd = Serial.read();
    while(Serial.available() > 0)
      Serial.read();
    if (cmd == 'r')
      servoRotA += 1;
    if (cmd == 'l')
      servoRotA -= 1;
    if (cmd == 'u')
      servoUDA += 1;
    if (cmd == 'd')
      servoUDA -= 1;
  }
  else
    digitalWrite(13, LOW);

  if (servoFBA < 110)
  {
    if (ult.distanceCM() < 30)
      servoFBA += 1;
  }
  if (servoFBA > 50)
  {
    if (ult.distanceCM() > 50)
      servoFBA -= 1;
  }
    
  servoRot.write(servoRotA);
  servoUD.write(servoUDA);
  servoFB.write(servoFBA);

}
