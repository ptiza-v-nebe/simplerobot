#include <Arduino.h>

#include <dc_motor_control.h>
#include <ir_remote_handler.h>

#include <NewPing.h>

#define IR_RECEIVE_PIN 11


#define TRIGGER_PIN  8  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     9  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define NUM_READINGS 5

unsigned long previousMillis = 0;
const long interval = 50;
unsigned int readings[NUM_READINGS]; // Array to store distance readings
unsigned int readIndex = 0;          // Current index in the array
unsigned int total = 0;              // Total of readings
unsigned int distance_average = 0;            // Average of readings

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int longtitude_speed = 0;
int side_speed = 0;
int general_speed = 25;

void myCommandHandler(uint8_t command) {
    //Serial.println("commandhandler");
    switch (command) {
        case 0x18: // Forward
            longtitude_speed = general_speed;
            side_speed = 0;
            break;
        case 0x52: // Backward
            longtitude_speed = -general_speed;
            side_speed = 0;
            break;
        case 0x8: // Left
            longtitude_speed = 0;
            side_speed = -general_speed;
            break;
        case 0x5A: // Right
            longtitude_speed = 0;
            side_speed = general_speed;
            break;
        case 0x45:
            general_speed = 25;
            break;
        case 0x46:
            general_speed = 50;
            break;
        case 0x47:
            general_speed = 100;
            break;
        default:
            break;
    }
}

void myPressedHandler() {
   // do something on press
}

void myReleaseHandler() {
    longtitude_speed = 0;
    side_speed = 0;
}

IRRemoteHandler irRemoteHandler;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  setup_motor_pins();
  //IrReceiver.begin(IR_RECEIVE_PIN);
  
  irRemoteHandler.begin(IR_RECEIVE_PIN);
  irRemoteHandler.set_command_handler(myCommandHandler);
  irRemoteHandler.set_pressed_handler(myPressedHandler);
  irRemoteHandler.set_released_handler(myReleaseHandler);


  for (int i = 0; i < NUM_READINGS; i++) {
    readings[i] = 0;
  }
}


void set_platform_speed(int longtitude_speed, int side_speed){
  set_motor_speed(MOTOR_LEFT, longtitude_speed + side_speed);
  set_motor_speed(MOTOR_RIGHT, longtitude_speed - side_speed);
}




// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------


void loop() {
    irRemoteHandler.run_once();
    
    unsigned long currentMillis = millis();

    //read distance
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
  
      // Subtract the last reading
      total = total - readings[readIndex];
      // Take a new reading
      unsigned int distance = sonar.ping_cm();
      readings[readIndex] = distance;
      // Add the reading to the total
      total = total + readings[readIndex];
      // Advance to the next position in the array
      readIndex++;
  
      // If we're at the end of the array, wrap around to the beginning
      if (readIndex >= NUM_READINGS) {
        readIndex = 0;
      }
  
      // Calculate the average
      distance_average = total / NUM_READINGS;
  
//      Serial.print("Average Distance: ");
//      Serial.print(distance_average);
//      Serial.println(" cm");
    }

    // do something useful with distance
    if(distance_average == 0) {
      //do nothing
    } else if (distance_average > 0 && distance_average < 30) {
      longtitude_speed = 0;
      side_speed = 0;
    } else if (distance_average > 30) {
      digitalWrite(13, !digitalRead(13)); // toggle led 13
    }

    //set platform speed
    set_platform_speed(longtitude_speed, side_speed);

}
