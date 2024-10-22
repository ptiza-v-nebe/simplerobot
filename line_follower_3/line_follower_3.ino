#include <Arduino.h>
#include <motor_control.h>

// Motor control pins : L298N H bridge
const int enAPin = 6; // Left motor PWM speed control
const int in1Pin = 7; // Left motor Direction 1
const int in2Pin = 10; // Left motor Direction 2

const int enBPin = 5; // Right motor PWM speed control
const int in3Pin = 4; // Right motor Direction 1
const int in4Pin = 2; // Right motor Direction 2


const int IR_SENSOR_LEFT_PIN = 12;
const int IR_SENSOR_RIGHT_PIN = 3;


MotorControl motor_control;

void setup() {
  Serial.begin(9600);
  
  pinMode(IR_SENSOR_LEFT_PIN, INPUT_PULLUP);
  pinMode(IR_SENSOR_RIGHT_PIN, INPUT_PULLUP);

  motor_control.begin(enAPin, in1Pin, in2Pin, enBPin, in3Pin, in4Pin);
}

#define MOTOR_SPEED 20

void loop() {
  int leftIRSensorValue = digitalRead(IR_SENSOR_LEFT_PIN);
  int rightIRSensorValue = digitalRead(IR_SENSOR_RIGHT_PIN);

  //If none of the sensors detects black line, then go straight
  if (rightIRSensorValue == LOW && leftIRSensorValue == LOW)
  {
    motor_control.set_motor_speed(MOTOR_LEFT, MOTOR_SPEED);
    motor_control.set_motor_speed(MOTOR_RIGHT, MOTOR_SPEED);
  }
  //If right sensor detects black line, then turn right
  else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW )
  {
    motor_control.set_motor_speed(MOTOR_LEFT, MOTOR_SPEED);
    motor_control.set_motor_speed(MOTOR_RIGHT, -MOTOR_SPEED);
  }
  //If left sensor detects black line, then turn left  
  else if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH )
  {
    motor_control.set_motor_speed(MOTOR_LEFT, -MOTOR_SPEED);
    motor_control.set_motor_speed(MOTOR_RIGHT, MOTOR_SPEED);
  } 
  //If both the sensors detect black line, then stop 
  else 
  {
    motor_control.set_motor_speed(MOTOR_LEFT, 0);
    motor_control.set_motor_speed(MOTOR_RIGHT, 0);
  }
}
