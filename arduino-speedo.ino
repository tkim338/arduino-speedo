#include <Servo.h>

int hall_sensor_pin = 2;
int servo_output_pin = 3;

// calibration values
int max_rpm = 7500;  // rpm of driveshaft at max measured speed

Servo speedo_servo;

void setup() {
  Serial.begin(9600);
  pinMode(hall_sensor_pin, INPUT);
  speedo_servo.attach(servo_output_pin);
}

void loop() {
  int timeout = 100000;  // uS (0.1 sec)

  unsigned long start_time = micros();
  int pulse_count = 0
  while (micros() < start_time + timeout) {
    bool pulse_counted = false;
    while (digitalRead(hall_sensor_pin) == HIGH) {
      if (pulse_counted == False) {
        pulse_counted = True;
        pulse_count += 1;
      }
    }
  }

  unsigned long rpm = pulse_count / timeout / 1000000;
  unsigned long rpm = rps * 60;
  pulse_count = 0;

  int angle = map(rpm, 0, max_rpm, 0, 179);
  Serial.print(rpm);
  Serial.print(", ");
  Serial.println(angle);
  speedo_servo.write(angle);
}
