/* La Fabrique

   Project hardware abstraction layer 
   by David Beaudette
   
   Constants and functions that depend on the circuit built around the Arduino.
   
*/

#ifndef __NANO_RFID_HAL_H__
#define __NANO_RFID_HAL_H__

#include "RF24.h"  // For radio parameter definitions

// GPIO pin numbers
const int redLedPin   = 3;      // Red LED
const int grnLedPin   = 4;      // Green LED
const int radioCsnPin = 5;      // NRF24L01+ CSN signal
const int radioCePin  = 6;      // NRF24L01+ CE signal
const int relayPin    = 7;      // Relay
const int rfResetPin  = 9;      // RC522 reset signal
const int rfSdaPin    = 10;     // RC522 SDA signal

// Serial communication rate
const int serialRate  = 115200;

// Other program constants
const int quickFlash = 500;    // duration in ms for quickly flashing a LED
const int slowFlash  = 1000;   // duration in ms for slowly flashing a LED

// State variable is one the following state:  
//  0 - Enabled: As commanded by server: relay activated until told otherwise
//  1 - Disabled: As commanded by server: relay deactivated until told otherwise
//  2 - Activated: Valid RFID triggered: relay activated until valid RFID triggers
//  3 - Idle: Relay deactivated, wait for card or server command
//  4 - TriggeredOnce: In dual RFID activation mode, valid RFID triggered: waiting for a second one
enum sys_state_t {ENABLED       = 0, 
                  DISABLED      = 1, 
                  ACTIVATED     = 2,
                  IDLE          = 3,
                  TRIGGEREDONCE = 4};

// Radio configuration
const uint64_t pipes[2]        = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};
const uint8_t radioChannel     = 0x4c;
const uint8_t payloadSize      = 10;
const int retryDelay           = 15;
const int retryCount           = 15;
const rf24_datarate_e dataRate = RF24_1MBPS;

void SetPins();
void FlashLed(const int pinNum, const int duration_ms, int num_times);
void Stall();

inline void SetPins() {
  // Reset digital outputs
  pinMode(relayPin,  OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(grnLedPin, OUTPUT);
  
  digitalWrite(relayPin,  LOW);
  digitalWrite(redLedPin, LOW);
  digitalWrite(grnLedPin, LOW);
}

inline void FlashLed(const int pinNum, const int duration_ms, int num_times) {
  for(int i = 0; i < num_times; i++) {
    digitalWrite(pinNum, HIGH);
    delay(duration_ms);
    digitalWrite(pinNum, LOW);
    if(i < (num_times-1)) {
      delay(duration_ms);
    }
  }
}

inline void Stall() {
  while(1) {
    FlashLed(redLedPin, quickFlash, 2);
    delay(2000);
  }
}

#endif // __NANO_RFID_HAL_H__