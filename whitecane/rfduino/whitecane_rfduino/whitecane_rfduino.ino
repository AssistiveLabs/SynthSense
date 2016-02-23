// Project: Whitecane
// Organization: Synthsense
// Authors: Adhitya Murali, Tomas Vega, Craig Hiller

#include <Wire.h>
#include "Adafruit_MCP23008.h"
#include <RFduinoBLE.h>

#define TRIGGER_PIN_1  7  // I/O extender ID
#define ECHO_PIN_1     3  // RFduino
#define TRIGGER_PIN_2  6  // I/O extender ID
#define ECHO_PIN_2     2  // RFduino

// These should be using the MCP23008 port expander
#define CANE_MOTOR_1        0
#define CANE_MOTOR_2        1
#define NAV_MOTOR_1         2
#define NAV_MOTOR_2         3
#define NAV_MOTOR_3         4
#define NAV_MOTOR_4         5

#define MAX_DISTANCE 500 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

enum state_type {
  INIT,
  US_1,
  VIBRATE_1,
  US_2,
  VIBRATE_2,
};

Adafruit_MCP23008 mcp;
long threshold;
state_type state;
long curr_dist;
int hcsr_timeout;
int inter_ping_delay;
int vibration_delay;
int num_end_rotations;

void pin_set(char pattern) {
  for(char i = 0; i < 8; i++) {
    bool val = (pattern & (1 << i)) >> i;

    if (val == 0) {
      mcp.digitalWrite(i, LOW);
    } else {
      mcp.digitalWrite(i, HIGH);
    }

  }
  
}

long read_distance(int trigger_pin, int echo_pin) {
  mcp.digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  mcp.digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  mcp.digitalWrite(trigger_pin, LOW);
  long duration = pulseIn(echo_pin, HIGH, hcsr_timeout);
  return duration/58.2;
}

void navband_vibrate_left(void) {
  Serial.print("*****NAV-BAND LEFT");
  for (int i = 4; i <= 32; i=i << 1) {
    pin_set(i);
    delay(200);
  }
  pin_set(0);
}

void navband_vibrate_right(void) {
  Serial.println("*****NAV-BAND RIGHT");
  for (int i = 32; i >= 4; i=i >> 1) {
    pin_set(i);
    delay(200);
  }
  pin_set(0);
}

void navband_vibrate_left_arrive(void) {
  Serial.print("*****NAV-BAND FINAL LEFT");
  navband_vibrate_left();
  for (int i = 0; i < num_end_rotations; i++) {
    pin_set(0);
    delay(100);
    pin_set(32);
    delay(100);
    pin_set(0);
  }
}

void navband_vibrate_right_arrive(void) {
  Serial.print("*****NAV-BAND FINAL RIGHT");
  navband_vibrate_right();
  for (int i = 0; i < num_end_rotations; i++) {
    pin_set(0);
    delay(100);
    pin_set(4);
    delay(100);
    pin_set(0);
  }
}

void navband_vibrate(void) {
  mcp.digitalWrite(NAV_MOTOR_1, HIGH);
  mcp.digitalWrite(NAV_MOTOR_2, LOW);
  mcp.digitalWrite(NAV_MOTOR_3, LOW);
  mcp.digitalWrite(NAV_MOTOR_4, LOW);
  delay(200);

  mcp.digitalWrite(NAV_MOTOR_1, LOW);
  mcp.digitalWrite(NAV_MOTOR_2, HIGH);
  mcp.digitalWrite(NAV_MOTOR_3, LOW);
  mcp.digitalWrite(NAV_MOTOR_4, LOW);
  delay(200);
  
  mcp.digitalWrite(NAV_MOTOR_1, LOW);
  mcp.digitalWrite(NAV_MOTOR_2, LOW);
  mcp.digitalWrite(NAV_MOTOR_3, HIGH);
  mcp.digitalWrite(NAV_MOTOR_4, LOW);
  delay(200);
  
  mcp.digitalWrite(NAV_MOTOR_1, LOW);
  mcp.digitalWrite(NAV_MOTOR_2, LOW);
  mcp.digitalWrite(NAV_MOTOR_3, LOW);
  mcp.digitalWrite(NAV_MOTOR_4, HIGH);
  delay(200);
}

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  state = INIT;  // By right this should be CALIBRATION

  // Initialize ultrasonic sensors
  threshold = 100.0; // This is in centimeters!
  hcsr_timeout = 1000;
  inter_ping_delay = 100;
  vibration_delay = 500;
  num_end_rotations = 4;
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(ECHO_PIN_2, INPUT);

  // Initialize Adafruit MCP23008 I/O expander
  mcp.begin();
  for (int i = 0; i < 8; i++) {
    mcp.pinMode(i, OUTPUT);  
  }

  RFduinoBLE.advertisementData = "CouDow";
  RFduinoBLE.deviceName = "ChillerNavBand";
  RFduinoBLE.begin();
}

void loop() {
  switch(state) {

    case INIT:
      state = US_1;
      break;

    case US_1:
      delay(inter_ping_delay); // Wait for some time between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
      curr_dist = read_distance(TRIGGER_PIN_1, ECHO_PIN_1);
      Serial.print("US_1: ");
      Serial.print(curr_dist);
      Serial.println("cm");
      if ((curr_dist > threshold) || (curr_dist <= 1.0)) {
        state = US_2;
      } else {
        state = VIBRATE_1;
      }
      break;

    case VIBRATE_1:
      Serial.println("VIBRATE 1");
      mcp.digitalWrite(CANE_MOTOR_1, HIGH);
      delay(vibration_delay);
      mcp.digitalWrite(CANE_MOTOR_1, LOW);
      state = US_2;
      break;

    case US_2:
      delay(inter_ping_delay); // Wait for some time between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
      curr_dist = read_distance(TRIGGER_PIN_2, ECHO_PIN_2);
      Serial.print("US_2: ");
      Serial.print(curr_dist);
      Serial.println("cm");
      if ((curr_dist > threshold) || (curr_dist <= 1.0)) {
        state = INIT;
      } else {
        state = VIBRATE_2;
      }
      break;

    case VIBRATE_2:
      Serial.println("VIBRATE 2");
      mcp.digitalWrite(CANE_MOTOR_2, HIGH);
      delay(vibration_delay);
      mcp.digitalWrite(CANE_MOTOR_2, LOW);
      state = INIT;
      break;
      
    default:
      Serial.print("DEFAULT");
      break;
  }
}

void RFduinoBLE_onReceive(char *data, int len) {
  int cmd = data[0];
  if (cmd == 0){
    navband_vibrate_left();
  } else if (cmd == 1) {
    navband_vibrate_right();
  } else if (cmd == 2) {
    navband_vibrate_left_arrive();
  } else if (cmd == 3) {
    navband_vibrate_right_arrive();
  } else if (cmd == 4) {
    threshold = 50;
  } else if (cmd == 5) {
    threshold = 100;
  } else if (cmd == 6) {
    threshold = 150;
  } else if (cmd == 7) {
    threshold = 200;
  } else if (cmd == 8) {
    threshold = 250;
  }
}

