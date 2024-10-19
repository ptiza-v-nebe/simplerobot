#include <NewPing.h>

#define TRIGGER_PIN  8  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     9  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define NUM_READINGS 5


unsigned int readings[NUM_READINGS]; // Array to store distance readings
unsigned int readIndex = 0;          // Current index in the array
unsigned int total = 0;              // Total of readings
unsigned int distance_average = 0;            // Average of readings


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


void setup_measure_distance(){
  for (int i = 0; i < NUM_READINGS; i++) {
    readings[i] = 0;
  }  
}


int measure_distance(){
    static const long interval = 50;
    static unsigned long previousMillis = 0;
    
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
    }

    return distance_average;
}
