/*
   Microstepping demo

   This requires that M0, M1 be connected in addition to STEP,DIR

   Copyright (C)2015 Laurentiu Badea

   This file may be redistributed under the terms of the MIT license.
   A copy of this license has been included with this distribution in the file LICENSE.
*/
#include <Arduino.h>
#include "A4988.h"

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define M1_ENBL 5
#define M2_ENBL 8

// 2-wire basic config, microstepping is hardwired on the driver
A4988 stepper1(MOTOR_STEPS, 3, 4);
A4988 stepper2(MOTOR_STEPS, 6, 7);

/*********************
  global variables
*********************/
unsigned long time_since_last_reset = 0;
const int buttonPin = 2;     // the number of the pushbutton pin
int sensorPin = A1;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

int phase = 1;
int buttonState = 0;         // variable for reading the pushbutton status
int motorSpeed = 0;

int interval_five = 6000;        //6 seconds
int rot90 = 200;
int rpm = 3;

void setup() {
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  stepper1.begin(rpm, 16);
  stepper2.begin(rpm, 16);
  // rpm: doesn't affect to rotation
  /* microstep
      1ms  = 200  steps for 1 revolution
      4ms  = 800  steps for 1 revolution
      8ms  = 1600 steps for 1 revolution
      16ms = 3200 steps for 1 revolution
      but speed are same
  */

}

void loop() {

  buttonState = digitalRead(buttonPin);
  time_since_last_reset = millis();
  if (buttonState == HIGH) {
    phase = phase + 1;
    Serial.println(phase);
  }

  if (phase == 1) {
    for (int i = 0; i < rot90; i++) {
      stepper1.move(1);
      stepper2.move(1);
    }
  } else if (phase == 2) {
    for (int i = 0; i < rot90; i++) {
      stepper1.move(-1);
      stepper2.move(-1);
    }
  } else if (phase == 3) {
    for (int i = 0; i < rot90; i++) {
      stepper1.move(1);
      stepper2.move(-1);
    }
  } else if (phase == 4) {
    for (int i = 0; i < rot90; i++) {
      stepper1.move(-1);
      stepper2.move(1);
    }
  } else if (phase == 5) {
    phase = 0;
  }

  //while ((millis() - time_since_last_reset) < interval_five) {
  //step each time through for

  //}                                              // loop, simulates both running
  //simultaneously
  //pause 1/2 second
  //delay(500);

}
