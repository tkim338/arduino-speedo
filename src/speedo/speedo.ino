#include <Servo.h>

int hall_sensor_pin = 2;
int servo_output_pin = 3;

float rpm;
unsigned long pulse_duration;
unsigned long timeout = 200000;  // uS

// calibration values
unsigned long pulse_on_ratio = 0.1;
float diff_ratio = 3.909;
float wheel_diameter = 24.7;  // inches
float speedo_max = 100.0;  // mph
float mile_to_inch = 63360.0;  // inch/mile
float hr_to_min = 60.0;  // min/hr
float max_rpm = (speedo_max * mile_to_inch / hr_to_min) / (wheel_diameter * PI) * diff_ratio;  // rpm of driveshaft that correlates to maximum speed on gauge (100 mph)

Servo speedo_servo;

void setup() {
  Serial.begin(9600);
  pinMode(hall_sensor_pin, INPUT);
  speedo_servo.attach(servo_output_pin);
}

void loop() {
  pulse_duration = pulseIn(hall_sensor_pin, HIGH, timeout);
  if (pulse_duration > 0) {
    rpm = 60.0 / (pulse_duration / pulse_on_ratio);
  }
  else {
    rpm = 0;
  }
  
  int angle = map(rpm, 0, max_rpm, 0, 179);
  Serial.print("RPM: ");
  Serial.print(rpm);
  Serial.print(", angle: ");
  Serial.println(angle);

  speedo_servo.write(angle);
}
