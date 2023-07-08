#include <Servo.h>

int hall_sensor_pin = 2;
int servo_output_pin = 3;

// calibration values
unsigned long pulse_ratio = 0.1;  // proportion of rotation during which sensor is activated
int max_rpm = 7500;  // rpm of driveshaft that correlates to maximum speed on gauge

Servo speedo_servo;

void setup() {
  Serial.begin(9600);
  pinMode(hall_sensor_pin, INPUT);
  speedo_servo.attach(servo_output_pin);
}

void loop() {
  unsigned long timeout = 1000000;  // uS (1 sec); min 60 RPM measurement
  unsigned long pulse_duration = pulseIn(hall_sensor_pin, HIGH, timeout) / 1000000;  // seconds
  unsigned long rps = pulse_ratio / pulse_duration;
  unsigned long rpm = rps * 60;
  if (pulse_duration == 0) {  // This avoids division by 0 that results in rpm = inf
    rpm = 0;
  }
  int angle = map(rpm, 0, max_rpm, 0, 179);
  Serial.print(rpm);
  Serial.print(", ");
  Serial.println(angle);
  speedo_servo.write(angle);
}
