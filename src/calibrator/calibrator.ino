#include <Servo.h>

int hall_sensor_pin = 2;
int servo_output_pin = 3;

unsigned long prev_pulse_end;

Servo speedo_servo;

void setup() {
  Serial.begin(9600);
  pinMode(hall_sensor_pin, INPUT);
  speedo_servo.attach(servo_output_pin);
}

void loop() {
  unsigned long timeout = 200000;  // uS (0.2 sec); min 12 RPM measurement
  unsigned long pulse_duration = pulseIn(hall_sensor_pin, HIGH, timeout);
  unsigned long rotation_period = micros() - prev_pulse_end; // uS
  prev_pulse_end = micros();

  unsigned long pulse_on_percent = pulse_duration / rotation_period;
  if (pulse_duration == 0) {
    pulse_on_percent = 0;
  }
  int angle = map(pulse_on_percent, 0, 1, 0, 179);
  Serial.print("pulse on percentage: ");
  Serial.println(angle);
  speedo_servo.write(angle);
}
