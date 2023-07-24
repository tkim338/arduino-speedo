#include <Servo.h>

int hall_sensor_pin = 2;
int servo_output_pin = 3;

unsigned long prev_pulse_end;
float rpm;
int prev_pulse_count;
int pulse_count;
unsigned long prev_time;
unsigned long pulse_duration;

unsigned long timeout = 500000;  // uS (0.5 sec)

// calibration values
float max_rpm = 7800;  // rpm of driveshaft that correlates to maximum speed on gauge (100 mph)

Servo speedo_servo;

void setup() {
  Serial.begin(9600);
  pinMode(hall_sensor_pin, INPUT);
  speedo_servo.attach(servo_output_pin);
}

void loop() {
  while (micros() < prev_time + timeout) {
    pulse_duration = pulseIn(hall_sensor_pin, HIGH, timeout);
    if (pulse_duration > 0) {
      pulse_count += 1;
    }
  }
  prev_time = micros();

  pulse_count = (prev_pulse_count + pulse_count) / 2; // truncated to int; round down
  rpm = pulse_count / (timeout / 1000000.0);
  prev_pulse_count = pulse_count;
  
  int angle = map(rpm, 0, max_rpm, 0, 179);
  Serial.print("RPM: ");
  Serial.print(rpm);
  Serial.print(", angle: ");
  Serial.println(angle);
  speedo_servo.write(angle);
}
