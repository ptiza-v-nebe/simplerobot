#include <Arduino.h>
#include <motor_control.h>

// Motor control pins : L298N H bridge
const int enAPin = 6; // Left motor PWM speed control
const int in1Pin = 7; // Left motor Direction 1
const int in2Pin = 10; // Left motor Direction 2

const int enBPin = 5; // Right motor PWM speed control
const int in3Pin = 4; // Right motor Direction 1
const int in4Pin = 2; // Right motor Direction 2


// PD parameters
float Kp = 2.0;  // Proportional gain
float Kd = 0.0;  // Derivative gain

// PD control variables
float error = 0;
float previous_error = 0;
float derivative = 0;
float pd_output = 0;

// Target value (we want the sensor to detect black tape, which is 0)
int target_value = 0;  
float sensor_value = 0.0;


const int IR_SENSOR_LEFT_PIN = 12;
const int IR_SENSOR_RIGHT_PIN = 3;


int max_side_speed = 15;
int longtitude_speed = max_side_speed;
float side_speed = 0;





MotorControl motor_control;

enum TapeState {NO_TAPE_RIGHT, TAPE_LEFT, TAPE_LEFT_RIGHT, TAPE_RIGHT, NO_TAPE_LEFT};

TapeState tape_state = TapeState::NO_TAPE_RIGHT;
TapeState previous_tape_state = TapeState::NO_TAPE_RIGHT;
float raw_sensor_value = -1.0;

void setup() {
  //TCCR0B = TCCR0B & B11111000 | B00000010 ;
  Serial.begin(9600);
  
  pinMode(IR_SENSOR_LEFT_PIN, INPUT_PULLUP);
  pinMode(IR_SENSOR_RIGHT_PIN, INPUT_PULLUP);

  motor_control.begin(enAPin, in1Pin, in2Pin, enBPin, in3Pin, in4Pin);
}


float mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}



void loop() {
  int raw_left_sensor_value = digitalRead(IR_SENSOR_LEFT_PIN);
  int raw_right_sensor_value = digitalRead(IR_SENSOR_RIGHT_PIN);


  // platform should start from right
  switch (tape_state) {
    case TapeState::NO_TAPE_RIGHT: { // -1.0
      //forward
      if(previous_tape_state == TapeState::NO_TAPE_RIGHT && raw_left_sensor_value == 1 && raw_right_sensor_value == 0){
        raw_sensor_value = -0.5;
        tape_state = TapeState::TAPE_LEFT;
      }

      //backward
      if(previous_tape_state == TapeState::TAPE_LEFT && raw_left_sensor_value == 1 && raw_right_sensor_value == 0){
        raw_sensor_value = -0.5;
        previous_tape_state = tape_state;
        tape_state = TapeState::TAPE_LEFT;
      }
      
      break;
    } 
    case TapeState::TAPE_LEFT: { // -0.5
      //forward
      if(previous_tape_state == TapeState::NO_TAPE_RIGHT && raw_left_sensor_value == 1 && raw_right_sensor_value == 1){
        raw_sensor_value = 0.0;
        previous_tape_state = tape_state;
        tape_state = TapeState::TAPE_LEFT_RIGHT;
      }
      
      //backward
      if(previous_tape_state == TapeState::TAPE_LEFT_RIGHT && raw_left_sensor_value == 0 && raw_right_sensor_value == 0){
        raw_sensor_value = -1.0;
        previous_tape_state = tape_state;
        tape_state = TapeState::NO_TAPE_RIGHT;
      }      
      break;
    } 
    case TapeState::TAPE_LEFT_RIGHT: { // 0.0
      //forward
      if(previous_tape_state == TapeState::TAPE_LEFT && raw_left_sensor_value == 0 && raw_right_sensor_value == 1){
        raw_sensor_value = 0.5;
        previous_tape_state = tape_state;
        tape_state = TapeState::TAPE_RIGHT;
        
      }
      
      //backward
      if(previous_tape_state == TapeState::TAPE_RIGHT && raw_left_sensor_value == 1 && raw_right_sensor_value == 0){
        raw_sensor_value = -0.5;
        previous_tape_state = tape_state;
        tape_state = TapeState::TAPE_LEFT;
      }
      break;
    } 
    case TapeState::TAPE_RIGHT: { // 0.5
      //forward
      if(previous_tape_state == TapeState::TAPE_LEFT_RIGHT && raw_left_sensor_value == 0 && raw_right_sensor_value == 0){
        raw_sensor_value = 1.0;        
        previous_tape_state = tape_state;
        tape_state = TapeState::NO_TAPE_LEFT;

      }
      //backward
      if(previous_tape_state == TapeState::NO_TAPE_LEFT && raw_left_sensor_value == 1 && raw_right_sensor_value == 1){
        previous_tape_state = tape_state;
        tape_state = TapeState::TAPE_LEFT_RIGHT;
        raw_sensor_value = 0.0;
      }
      break;
    } 
    case TapeState::NO_TAPE_LEFT: { // 1.0
      //forward
      if(previous_tape_state == TapeState::TAPE_RIGHT && raw_left_sensor_value == 0 && raw_right_sensor_value == 1){
        raw_sensor_value = 0.5;
        previous_tape_state = tape_state;
        tape_state = TapeState::TAPE_RIGHT;
      }  
      break;
    }    
  };

  //sensor_value = 0.1 * sensor_value + 0.9 * raw_sensor_value;
  sensor_value = raw_sensor_value;
  
  // Calculate the error (we want the sensor to be over black tape, so target is 0)
  error = target_value - sensor_value;

  // Calculate the Derivative term
  derivative = error - previous_error;

  // Calculate the PD output (rotational correction)
  pd_output = (Kp * error + Kd * derivative)/2.0;

  // Clamp the PD output to avoid excessive correction
  side_speed = mapfloat(pd_output, -1, 1, -max_side_speed, max_side_speed);

  // Update previous error for the next loop
  previous_error = error;

  // Set the speed with forward speed and rotational speed (based on PD correction)
  //motor_control.set_platform_speed(longtitude_speed, side_speed);

  // Debugging info
//
  Serial.print("l: ");
  Serial.print(raw_left_sensor_value);
  Serial.print(" r: ");
  Serial.print(raw_right_sensor_value);


  Serial.print(" Raw Sensor: ");
  Serial.println(raw_sensor_value);
//  Serial.print(" Sensor: ");
//  Serial.print(sensor_value);
//  Serial.print(" Error: ");
//  Serial.print(error);
//  Serial.print(" PD: ");
//  Serial.println(pd_output);
//  Serial.print(" SS: ");
//  Serial.println(side_speed); 

  // Small delay for stability (tune if necessary)
  //delay(10);
}
