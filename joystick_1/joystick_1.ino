#include <Arduino.h>
#include <motor_control.h>


// Motor control pins : L298N H bridge
const int enAPin = 6; // Left motor PWM speed control
const int in1Pin = 7; // Left motor Direction 1
const int in2Pin = 10; // Left motor Direction 2


const int enBPin = 5; // Right motor PWM speed control
const int in3Pin = 4; // Right motor Direction 1
const int in4Pin = 2; // Right motor Direction 2


int longtitude_speed = 0;
int side_speed = 0;


MotorControl motor_control;


void setup() {
  Serial.begin(9600);
  motor_control.begin(enAPin, in1Pin, in2Pin, enBPin, in3Pin, in4Pin);
}


int get_speed_from_joystick(int value){
  int speed = 0;
  if(value >= 0 && value < 480) {
    speed = map(value, 480, 0, 0, 100);
  } else if (value >= 544 && value <= 1023) {
    speed = map(value, 544, 1023, 0, -100);
  } else if (value > 480 && value < 544) {
    speed = 0;
  }
  return speed;
}


void loop() {
  // get forward speed
  int x = analogRead(A0);
  int y = analogRead(A1);

  int longtitude_speed = get_speed_from_joystick(x);
  int side_speed = -get_speed_from_joystick(y);

  motor_control.set_platform_speed(longtitude_speed, side_speed);
  
  //Serial.println(String("X: ") + String(analogRead(A0)) + String(", Y: ") + String(analogRead(A1)) + String(" | L: ") + String(longtitude_speed) + String(", S: ") + String(side_speed));
}
