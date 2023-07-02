#include <Servo.h>

int hall_pin = 2;
float rpm;
float duration;

Servo speedo_servo;
int angle;

void setup() {
  Serial.begin(9600);
  // make the hall pin an input:
  pinMode(hall_pin, INPUT);
  speedo_servo.attach(9);
}

void loop() {
  duration = pulseIn(hall_pin, FALLING, 500000); //times the amount of microseconds the motor is not timing IR, Times out after 100000 uS. Raise the timeout for slower RPM readings. .5 second
  rpm = 60.0/duration*100000; //See above
  if (duration == 0) {
    rpm = 0;
  }
  angle = map(rpm, 0, 7435, 0, 179);
  Serial.print(rpm);
  Serial.print(", ");
  Serial.println(angle);
  speedo_servo.write(angle);
}
