#include <Arduino.h>
#include <motor_control.h>

#include "ultrasonic_measurement.h"
#include "timer.h"

// Motor control pins : L298N H bridge
const int enAPin = 6; // Left motor PWM speed control
const int in1Pin = 7; // Left motor Direction 1
const int in2Pin = 5; // Left motor Direction 2

const int enBPin = 10; // Right motor PWM speed control
const int in3Pin = 4; // Right motor Direction 1
const int in4Pin = 2; // Right motor Direction 2

int longtitude_speed = 0;
int side_speed = 0;

MotorControl motor_control;

void setup() {
  pinMode(13, OUTPUT); // setup LED on pin 13
  Serial.begin(115200);
  motor_control.begin(enAPin, in1Pin, in2Pin, enBPin, in3Pin, in4Pin);
  setup_measure_distance();
}


void loop() {
    int distance = measure_distance();
      
    if (distance > 0 && distance < 40) { 
        
      if(spin_timer()){
        longtitude_speed = 0;
        side_speed = 30;
      } else {
        longtitude_speed = 0;
        side_speed = 0;  
      }
      
    } else if (distance >= 40) {
      longtitude_speed = 50;
      side_speed = 0;
      reset_timer();
    }

    motor_control.set_platform_speed(longtitude_speed, side_speed);
}
