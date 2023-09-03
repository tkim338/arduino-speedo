#include <Servo.h>

int hall_sensor_pin = 2;
int servo_output_pin = 3;

unsigned long prev_pulse_end;
float rpm;
int prev_pulse_count;
int pulse_count;
unsigned long prev_time;
unsigned long pulse_duration;

unsigned long timeout = 200000;  // uS (0.5 sec)

// calibration values
float diff_ratio = 3.909;
float wheel_diameter = 24.7;  // inches
float speedo_max = 100.0;  // mph
float mile_to_inch = 6336.0;  // inch/mile
float hr_to_min = 60.0;  // min/hr
float max_rpm = (speedo_max * mile_to_inch / hr_to_min) / (wheel_diameter * PI) * diff_ratio;  // rpm of driveshaft that correlates to maximum speed on gauge (100 mph)

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
  rpm = 60.0 * pulse_count / (timeout / 1000000.0);  // [sec/min * unitless / (u_sec / (sec / u_sec))]
  prev_pulse_count = pulse_count;
  
  int angle = map(rpm, 0, max_rpm, 0, 179);
  Serial.print("RPM: ");
  Serial.print(rpm);
  Serial.print(", angle: ");
  Serial.println(angle);
  speedo_servo.write(angle);

  pulse_count = 0;
}
