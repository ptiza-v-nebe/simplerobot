#include <Arduino.h>
#include "motor_control.h"


//#define DECODE_NEC
#include <IRremote.hpp> // include the library
#define IR_RECEIVE_PIN 11

unsigned long lastReceiveTime = 0;
unsigned long debounceDelay = 120; // Adjust this value as needed
uint32_t lastCommand = 0;
bool buttonPressed = false;

void setup() {
  Serial.begin(115200);
  setup_motor_pins();
  IrReceiver.begin(IR_RECEIVE_PIN);
}


void set_platform_speed(int longtitude_speed, int side_speed){
  set_motor_speed(MOTOR_LEFT, longtitude_speed + side_speed);
  set_motor_speed(MOTOR_RIGHT, longtitude_speed - side_speed);
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

int longtitude_speed = 0;
int side_speed = 0;
int general_speed = 25;



void loop() {

  if (IrReceiver.decode()) {
      if (IrReceiver.decodedIRData.command == 0x18) {  //forward
        longtitude_speed = general_speed;
        side_speed = 0;
      } else if (IrReceiver.decodedIRData.command == 0x52) { //backward
        longtitude_speed = -general_speed;
        side_speed = 0;
      } else if (IrReceiver.decodedIRData.command == 0x8) { //left
        longtitude_speed = 0;
        side_speed = -general_speed;
      } else if (IrReceiver.decodedIRData.command == 0x5A) { //right
        longtitude_speed = 0;
        side_speed = general_speed;
      } 

      
      if (IrReceiver.decodedIRData.command == 0x45) { //right
        general_speed = 25;
      } else if (IrReceiver.decodedIRData.command == 0x46) { //right
        general_speed = 50;
      } else if (IrReceiver.decodedIRData.command == 0x47) { //right
        general_speed = 100;
      }
 
      // Check if the received data is a repeat signal
      if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
        // Update the last received time if it's a repeat of the last command
        if (IrReceiver.decodedIRData.command == lastCommand) {
          lastReceiveTime = millis();
        }
      } else {
        // New button press detected
        lastCommand = IrReceiver.decodedIRData.command;
        lastReceiveTime = millis();
        buttonPressed = true;
        //Serial.println("Button Pressed: " + String(lastCommand));
      }
      IrReceiver.resume(); // Receive the next value
  }

  // Check if enough time has passed since the last signal
  if (buttonPressed && (millis() - lastReceiveTime > debounceDelay)) {
    buttonPressed = false;
    longtitude_speed = 0;
    side_speed = 0;  
    //Serial.println("Button Released: " + String(lastCommand));
  }


  

  
  set_platform_speed(longtitude_speed, side_speed);  

  
//  // get forward speed
//  int x = analogRead(A0);
//  int y = analogRead(A1);
//
//  int longtitude_speed = get_speed_from_joystick(x);
//  int side_speed = get_speed_from_joystick(y);
//
//  set_platform_speed(longtitude_speed, side_speed);
  
  //Serial.println(String("X: ") + String(analogRead(A0)) + String(", Y: ") + String(analogRead(A1)) + String(" | L: ") + String(longtitude_speed) + String(", S: ") + String(side_speed));
  //delay(10);
}
