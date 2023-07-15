#include <Servo.h>

int hall_sensor_pin = 2;
int servo_output_pin = 3;

unsigned long prev_pulse_end;
float pulse_on_ratio;
float rpm;

// calibration values
int max_rpm = 7800;  // rpm of driveshaft that correlates to maximum speed on gauge (100 mph)

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
  
  if (pulse_duration == 0) {
    rpm = 0;
  }
  else {
    pulse_on_ratio = float(pulse_duration) / rotation_period;
    rpm = pulse_on_ratio / (pulse_duration / 1000000.0) * 60;
  }
  
  int angle = map(rpm, 0, max_rpm, 0, 179);
  Serial.print("RPM: ");
  Serial.print(rpm);
  Serial.print(", angle: ");
  Serial.println(angle);
  speedo_servo.write(angle);
}
