#include <Servo.h>

int hall_sensor_pin = 2;
int servo_output_pin = 3;

unsigned long t0;
unsigned long pulse_duration;
unsigned long running_average_duration;
unsigned long total_duration;
float pulse_ratio;

unsigned long timeout = 1000000;  // uS

Servo speedo_servo;

void setup() {
  Serial.begin(9600);
  pinMode(hall_sensor_pin, INPUT);
  speedo_servo.attach(servo_output_pin);
  t0 = micros();
}

void loop() {
  pulse_duration = pulseIn(hall_sensor_pin, HIGH, timeout);
  if (pulse_duration > 0) {
    running_average_duration = running_average_duration * 0.9 + pulse_duration * 0.1;
  }
  total_duration = micros() - t0;

  pulse_ratio = running_average_duration / total_duration;
  
  int angle = map(pulse_ratio, 0, 1, 0, 179);
  Serial.print("pulse_ratio: ");
  Serial.print(pulse_ratio);
  Serial.print(", angle: ");
  Serial.println(angle);

  speedo_servo.write(angle);
}
