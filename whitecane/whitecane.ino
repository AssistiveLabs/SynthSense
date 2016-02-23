// Project: Whitecane
// Organization: Synthsense
// Authors: Adhitya Murali, Tomas Vega
// Purpose: Example NewPing library sketch that does a ping about 20 times per second.

#include <NewPing.h>

#define TRIGGER_PIN_1  11  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_1     10  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN_2  9
#define ECHO_PIN_2     8
#define MOTOR_1        15
#define MOTOR_2        14
#define MAX_DISTANCE 500 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar_1(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar_2(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned int threshold_us1;
unsigned int threshold_us2;
unsigned int curr_us1;
unsigned int curr_us2;
unsigned int curr_dist;
unsigned int total;

enum state_type {
  INIT,
  US_1,
  VIBRATE_1,
  US_2,
  VIBRATE_2,
};

state_type state;

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  state = US_1;  // By right this should be CALIBRATION
  threshold_us1 = 100; // This is in centimeters!
  threshold_us2 = 100; // This is in centimeters!  
  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_2, OUTPUT);
  total = 0;
}

void loop() {
  switch(state) {
    case INIT:
      state = US_1;
      break;

    case US_1:
      delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
      curr_us1 = sonar_1.ping(); // Send ping, get ping time in microseconds (uS).
      curr_dist = curr_us1  / US_ROUNDTRIP_CM;
      Serial.print("US_1: ");
      Serial.print(curr_dist); // Convert ping time to distance in cm and print result (0 = outside set distance range)
      Serial.println("cm");
      if ((curr_dist > threshold_us1) || (curr_dist <= 1.0)) {
        state = US_2;
      } else {
        Serial.println("OBSTACLE DETECTED 1");
        state = VIBRATE_1;
      }
      break;

    case VIBRATE_1:
      digitalWrite(MOTOR_1, HIGH);
      delay(500);
      digitalWrite(MOTOR_1, LOW);
      state = US_2;
      break;

    case US_2:
      delay(50);
      curr_us2 = sonar_2.ping(); // Send ping, get ping time in microseconds (uS).
      curr_dist = curr_us2  / US_ROUNDTRIP_CM;
      Serial.print("US_2: ");
      Serial.print(curr_dist); // Convert ping time to distance in cm and print result (0 = outside set distance range)
      Serial.println("cm");
      if ((curr_dist > threshold_us2) || (curr_dist <= 1.0)) {
        state = INIT;
      } else {
        Serial.println("OBSTACLE DETECTED 2");
        state = VIBRATE_2;
      }
      break;

    case VIBRATE_2:
      digitalWrite(MOTOR_2, HIGH);
      delay(500);
      digitalWrite(MOTOR_2, LOW);
      state = INIT;
      break;

    default:
      Serial.print("DEFAULT");
      break;
  }
}
