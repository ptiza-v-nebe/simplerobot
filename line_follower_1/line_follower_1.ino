#include <Arduino.h>
#include <motor_control.h>

// Motor control pins : L298N H bridge
const int enAPin = 6; // Left motor PWM speed control
const int in1Pin = 7; // Left motor Direction 1
const int in2Pin = 10; // Left motor Direction 2

const int enBPin = 5; // Right motor PWM speed control
const int in3Pin = 4; // Right motor Direction 1
const int in4Pin = 2; // Right motor Direction 2

const int ir_pin = 12;

int longtitude_speed = 0;
int side_speed = 0;

MotorControl motor_control;


void setup() {
  Serial.begin(9600);
  pinMode(ir_pin, INPUT_PULLUP);
  motor_control.begin(enAPin, in1Pin, in2Pin, enBPin, in3Pin, in4Pin);
}


void loop() {
    longtitude_speed = 15;

    if(digitalRead(ir_pin)){ // reflective - 1, nonreflective - 0 
      side_speed = 15;
    } else {
      side_speed = -15;
    }
    
    motor_control.set_platform_speed(longtitude_speed, side_speed);
}
