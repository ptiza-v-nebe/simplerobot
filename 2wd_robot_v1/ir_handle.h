//#define DECODE_NEC
#include <IRremote.hpp> // include the library
#define IR_RECEIVE_PIN 11

class IRRemoteHandle {
private:
  unsigned long lastReceiveTime = 0;
  unsigned long debounceDelay = 120; // Adjust this value as needed
  uint32_t lastCommand = 0;
  bool buttonPressed = false;
public:
  IRRemoteHandle();
    
};


IRRemoteHandle::IRRemoteHandle(){

}


void IRRemoteHandle::init(int ir_receive_pin){
  IrReceiver.begin(ir_receive_pin);
}


void IRRemoteHandle::run(){
  if (IrReceiver.decode()) {

    
      

 
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
    button_released_cb();
    //Serial.println("Button Released: " + String(lastCommand));
  }
}




//      if (IrReceiver.decodedIRData.command == 0x18) {  //forward
//        longtitude_speed = general_speed;
//        side_speed = 0;
//      } else if (IrReceiver.decodedIRData.command == 0x52) { //backward
//        longtitude_speed = -general_speed;
//        side_speed = 0;
//      } else if (IrReceiver.decodedIRData.command == 0x8) { //left
//        longtitude_speed = 0;
//        side_speed = -general_speed;
//      } else if (IrReceiver.decodedIRData.command == 0x5A) { //right
//        longtitude_speed = 0;
//        side_speed = general_speed;
//      } 
//
//      
//      if (IrReceiver.decodedIRData.command == 0x45) { //right
//        general_speed = 25;
//      } else if (IrReceiver.decodedIRData.command == 0x46) { //right
//        general_speed = 60;
//      } else if (IrReceiver.decodedIRData.command == 0x47) { //right
//        general_speed = 100;
//      }
