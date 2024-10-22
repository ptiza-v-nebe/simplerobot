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

enum MovementState { 
  INFRONT_OF_OBSTACLE,
  FORWARD_MOVING,
  REVERSE_MOVING
};

MovementState movement_state = MovementState::FORWARD_MOVING;


void loop() {
    int distance = measure_distance();

    if(movement_state == INFRONT_OF_OBSTACLE){
      if( spin_timer(400) ){
        longtitude_speed = 50;
        side_speed = 0;
        movement_state = FORWARD_MOVING;
      }

//      if (distance >= 40){
//        longtitude_speed = 50;
//        side_speed = 0;
//        movement_state = FORWARD_MOVING;
//      }
     
    } else if (movement_state == REVERSE_MOVING) {
      if( spin_timer(1000) ){
        movement_state = INFRONT_OF_OBSTACLE;
        longtitude_speed = 0;
        side_speed = 30;  
        reset_timer();
      }
      
    } else if (movement_state == FORWARD_MOVING) {
      if (distance > 0 && distance < 40){
        movement_state = REVERSE_MOVING;
        longtitude_speed = -30;
        side_speed = 0;  
        reset_timer();
      }
    } 
    

    motor_control.set_platform_speed(longtitude_speed, side_speed);
}
