#include <Servo.h>

int hall_sensor_pin = 2;
int servo_output_pin = 3;

unsigned long prev_pulse_end = 0.0;

// calibration values
int max_rpm = 5850;  // rpm of driveshaft that correlates to maximum speed on gauge

Servo speedo_servo;

void setup() {
  Serial.begin(9600);
  pinMode(hall_sensor_pin, INPUT);
  speedo_servo.attach(servo_output_pin);
}

void loop() {
  unsigned long timeout = 200000;  // uS (1 sec); min 60 RPM measurement
  unsigned long pulse_duration = pulseIn(hall_sensor_pin, HIGH, timeout);
  unsigned long rotation_period = micros() - prev_pulse_end; // uS
  prev_pulse_end = micros();

  unsigned long rps = 1.0 / (rotation_period / 1000000.0);
  unsigned long rpm = rps * 60;
  if (pulse_duration == 0) {
    rpm = 0;
  }
  int angle = map(rpm, 0, max_rpm, 0, 179);
  Serial.print(rpm);
  Serial.print(", ");
  Serial.println(angle);
  speedo_servo.write(angle);
}
