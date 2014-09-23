// Brighton College Micro-electronic Engineering

// Animatonic Prosthetic Hand

// //
//
// File to be transferred to the Arduino.
// Collects Bluetooth input and outputs to the servos
//
// //

#include <Servo.h>
#define angle(ch) map(ch, 'a', 'z', 1, 179) 

#define DEBUG 0
#define DEBUG_PRINT(msg) \
    if(DEBUG) {          \
      Serial.print("DEBUG: ");\
      Serial.println(msg);\
    } else // consumes semicolon, not a bug

// Servo Setup
Servo fingers_servo[5];

const char FINGERS_NAME[][7] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };

// Communication Constants
const byte START_BYTE  = 'S';
//const byte END_BYTE    = '\n';
const int BAUD         = 9600;

void setup() {
  // Attach Servos
  // Remember they can only be attached on PWM pins, marked with a "~"
  const int PINS[] = { 3, 5, 6, 10, 11 };
  int i;
  for(i=0; i<5; i++) {
    fingers_servo[i].attach(PINS[i]);
    fingers_servo[i].write(1);
  }
  // Setup Bluetooth serial line
  Serial.begin(BAUD); // Baud = 9600 bps
}

void loop() {
  byte finger_byte = 0;
  byte angle_byte = 0;
  
  // Feel free to change this back, thought it was neater...
  // Assumes the server knows what it's doing though
  
  while(Serial.read() != START_BYTE) ; // Wait for the start byte
  while(Serial.available() < 2) ; // Wait for the buffer to fill
  finger_byte = Serial.read();
  angle_byte = Serial.read();

  // Implement an END_BYTE
  //while(Serial.available() > 0 && Serial.read() != END_BYTE) ; // Consumes all input until END_BYTE
  // NOTE: the above is what you would do if you are to implement END_BYTE
  // However it may not be necessary, see issues for discussion

  // TODO: What if someone accidentally entered two bytes and pressed enter?
  
  int idx = finger_byte - '0'; // convert char to the int it represents
  // '0'=thumb, '1'=index, ... , '4'=pinky
  // idx stored as int instead of byte because it may be negative
  if(0 < idx || idx > 4) {
    // TODO: handle error: finger_byte out of range
    return;
  }

  if(angle_byte < 'a' || angle_byte > 'z') {
    // TODO: handle error: angle_byte out of range
    return;
  }
  
  fingers_servo[idx].write(angle(angle_byte));
  Serial.print(FINGERS_NAME[idx]);
  Serial.print(" moved ");
  Serial.println(angle_byte);
  
  // TODO: send success/failure message?
  // alternative approach:
  // send exit success here, failure in "TODO: handle error!"

}
