#include <Arduino.h>
#include "A4988.h"

// using a 200-step motor (most common)
// pins used are DIR, STEP, MS1, MS2, MS3 in that order
A4988 stepper(200, 6, 7, 22, 23, 24);

void setup() {
  //    // Set target motor RPM to 1RPM
  //    stepper.setRPM(20);
  //    // Set full speed mode (microstepping also works for smoother hand movement
  //    stepper.setMicrostep(16);
  stepper.begin(20, 16);
}

void loop() {


  // Tell motor to rotate 360 degrees. That's it.
  stepper.move(50);
  //stepper.rotate(90);
}
