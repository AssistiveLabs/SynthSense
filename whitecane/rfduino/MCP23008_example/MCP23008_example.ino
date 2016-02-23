#include <Wire.h>
#include "Adafruit_MCP23008.h"

// Basic toggle test for i/o expansion. flips pin #0 of a MCP23008 i2c
// pin expander up and down. Public domain

// Connect pin #1 of the expander to Analog 5 (i2c clock)
// Connect pin #2 of the expander to Analog 6 (i2c data)
// Connect pins #3, 4 and 5 of the expander to ground (address selection)
// Connect pin #6 and 18 of the expander to 5V (power and reset disable)
// Connect pin #9 of the expander to ground (common ground)

// Output #0 is on pin 10 so connect an LED or whatever from that to ground

Adafruit_MCP23008 mcp;
int delayTime = 300;
  
void setup() {  
  Serial.begin(9600);
  mcp.begin();      // use default address 0
  for (int i = 0; i < 8; i++) {
    mcp.pinMode(i, OUTPUT);  
  }
}


// flip the pin #0 up and down

void iterateLED() {
  for (int i = 0; i < 8; i++) {
    Serial.print("Low: ");
    Serial.println((i+8-1)%8);
    mcp.digitalWrite((i+8-1)%8, LOW);
    delay(delayTime);
    Serial.print("High: ");
    Serial.println(i);
    mcp.digitalWrite(i, HIGH);
    delay(delayTime);
  }
}

void loop() {
  iterateLED();
}
