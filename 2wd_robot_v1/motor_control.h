// Motor control pins : L298N H bridge
const int enAPin = 6; // Left motor PWM speed control
const int in1Pin = 7; // Left motor Direction 1
const int in2Pin = 5; // Left motor Direction 2

const int enBPin = 10; // Right motor PWM speed control
const int in3Pin = 4; // Right motor Direction 1
const int in4Pin = 2; // Right motor Direction 2


void setup_motor_pins(){
    pinMode(enAPin, OUTPUT);
    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);
    pinMode(in3Pin, OUTPUT);
    pinMode(in4Pin, OUTPUT);
    pinMode(enBPin, OUTPUT);
}


int clip_speed(int speed_percent){
  int clipped_speed = speed_percent;
  if(speed_percent >= 100) {
    clipped_speed = 100;
  } else if (speed_percent <= -100) {
    clipped_speed = -100;
  }
  return clipped_speed;
}


enum Direction { DIRECTION_FORWARD, DIRECTION_BACKWARD };
enum Motor { MOTOR_LEFT, MOTOR_RIGHT };


int get_direction_pin_1(Motor motor);
int get_direction_pin_1(Motor motor){
  if(motor == MOTOR_RIGHT) {
    return in1Pin;
  } else if (motor == MOTOR_LEFT){
    return in3Pin;
  }
}

int get_direction_pin_2(Motor motor);
int get_direction_pin_2(Motor motor){
  if(motor == MOTOR_RIGHT) {
    return in2Pin;
  } else if (motor == MOTOR_LEFT){
    return in4Pin;
  }
}

int get_enable_pin(Motor motor);
int get_enable_pin(Motor motor){
  if(motor == MOTOR_RIGHT) {
    return enAPin;
  } else if (motor == MOTOR_LEFT){
    return enBPin;
  }
}


void set_motor_direction(Motor motor, Direction direction);
void set_motor_direction(Motor motor, Direction direction){
  if(direction == DIRECTION_FORWARD){
    digitalWrite(get_direction_pin_1(motor), HIGH);
    digitalWrite(get_direction_pin_2(motor), LOW);
  } else if (direction == DIRECTION_BACKWARD){
    digitalWrite(get_direction_pin_1(motor), LOW);
    digitalWrite(get_direction_pin_2(motor), HIGH);
  }
}

// in percent 0-100
void set_motor_speed(Motor motor, int raw_speed);
void set_motor_speed(Motor motor, int raw_speed){
  int clipped_speed = clip_speed(raw_speed);
  if(clipped_speed > 0) {
    set_motor_direction(motor, DIRECTION_FORWARD);
  } else if (clipped_speed < 0 ) {
    set_motor_direction(motor, DIRECTION_BACKWARD);
  }

  int absolute_speed = abs(clipped_speed);

  int mapped_speed = 0;
  if(absolute_speed > 0){
    mapped_speed = map(absolute_speed, 0, 100, 90, 200);
  } else if (absolute_speed == 0) {
    mapped_speed = 0;
  }
  
  analogWrite(get_enable_pin(motor), mapped_speed);
}
