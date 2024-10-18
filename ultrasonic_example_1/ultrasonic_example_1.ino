#include <Arduino.h>
#include <dc_motor_control.h>
#include "ultrasonic_measurement.h"

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
    
    if (distance_average > 0 && distance_average < 30) {
      longtitude_speed = 0;
      side_speed = 0;
      digitalWrite(13, HIGH); // turn led on
    } else if (distance_average > 30) {
      longtitude_speed = 50;
      side_speed = 0;
      digitalWrite(13, LOW); // turn led off
    }

    set_platform_speed(longtitude_speed, side_speed);
}
