#include <Arduino.h>
#include <dc_motor_control.h>
#include "ultrasonic_measurement.h"
#include "timer.h"

int longtitude_speed = 0;
int side_speed = 0;

void setup() {
  pinMode(13, OUTPUT); // setup LED on pin 13
  Serial.begin(115200);
  setup_motor_pins();
  setup_measure_distance();
}
  

void set_platform_speed(int longtitude_speed, int side_speed){
  set_motor_speed(MOTOR_LEFT, longtitude_speed + side_speed);
  set_motor_speed(MOTOR_RIGHT, longtitude_speed - side_speed);
}


void loop() {
    int distance = measure_distance();
    
    if (distance_average > 0 && distance_average < 40) {
      longtitude_speed = 0;
      side_speed = 0;
      if(spin_timer()){
        longtitude_speed = 0;
        side_speed = 30;
      }
    } else if (distance_average > 40) {
      longtitude_speed = 50;
      side_speed = 0;
      reset_timer();
    }

    set_platform_speed(longtitude_speed, side_speed);
}
