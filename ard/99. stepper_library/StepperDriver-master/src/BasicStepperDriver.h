/*
 * Generic Stepper Motor Driver Driver
 * Indexer mode only.
 *
 * Copyright (C)2015-2017 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#ifndef STEPPER_DRIVER_BASE_H
#define STEPPER_DRIVER_BASE_H
#include <Arduino.h>

// used internally by the library to mark unconnected pins
#define PIN_UNCONNECTED -1
#define IS_CONNECTED(pin) (pin != PIN_UNCONNECTED)

enum Direction {DIR_FORWARD, DIR_REVERSE};
enum Mode {CONSTANT_SPEED, LINEAR_SPEED};

/*
 * calculate the step pulse in microseconds for a given rpm value.
 * 60[s/min] * 1000000[us/s] / microsteps / steps / rpm
 */
#define STEP_PULSE(steps, microsteps, rpm) (60*1000000L/steps/microsteps/rpm)

inline void microWaitUntil(unsigned long target_micros){
    yield();
    while (micros() < target_micros);
}
#define DELAY_MICROS(us) microWaitUntil(micros() + us)

/*
 * Basic Stepper Driver class.
 * Microstepping level should be externally controlled or hardwired.
 */
class BasicStepperDriver {
protected:
    /*
     * Motor Configuration
     */
    short motor_steps;           // motor steps per revolution (usually 200)
    short accel = 1000;     // maximum acceleration [steps/s^2]
    short decel = 1000;     // maximum deceleration [steps/s^2]

    /*
     * Driver Configuration
     */
    short dir_pin;
    short step_pin;
    short enable_pin = PIN_UNCONNECTED;
    // Get max microsteps supported by the device
    virtual short getMaxMicrostep();
    // current microstep level (1,2,4,8,...), must be < getMaxMicrostep()
    short microsteps = 1;
    // tWH(STEP) pulse duration, STEP high, min value (us)
    static const int step_high_min = 1;
    // tWL(STEP) pulse duration, STEP low, min value (us)
    static const int step_low_min = 1;
    // tWAKE wakeup time, nSLEEP inactive to STEP (us)
    static const int wakeup_time = 0;

    short rpm = 0;

    /*
     * Movement state
     */
    Mode mode = CONSTANT_SPEED;
    long step_count;        // current position
    long steps_remaining;   // to complete the current move (absolute value)
    long steps_to_cruise;   // steps to reach cruising (max) rpm
    long steps_to_brake;    // steps needed to come to a full stop
    long step_pulse;        // step pulse duration (microseconds)

    // DIR pin state
    short dir_state;
    // STEP pin state (HIGH / LOW)
    short step_state = LOW;

    void calcStepPulse(void);

private:
    // microstep range (1, 16, 32 etc)
    static const short MAX_MICROSTEP = 128;

public:
    /*
     * Basic connection: DIR, STEP are connected.
     */
    BasicStepperDriver(short steps, short dir_pin, short step_pin);
    BasicStepperDriver(short steps, short dir_pin, short step_pin, short enable_pin);
    /*
     * Initialize pins, calculate timings etc
     */
    void begin(short rpm=60, short microsteps=1);
    /*
     * Set current microstep level, 1=full speed, 32=fine microstepping
     * Returns new level or previous level if value out of range
     */
    virtual short setMicrostep(short microsteps);
    /*
     * Set target motor RPM (1-200 is a reasonable range)
     */
    void setRPM(short rpm);
    short getRPM(void){
        return rpm;
    };
    short getCurrentRPM(void){
        return (short)(60*1000000L / step_pulse / microsteps / motor_steps);
    }
    /*
     * Set speed profile - CONSTANT_SPEED, LINEAR_SPEED (accelerated)
     * accel and decel are given in [full steps/s^2]
     */
    void setSpeedProfile(Mode mode, short accel=1000, short decel=1000);
    /*
     * Move the motor a given number of steps.
     * positive to move forward, negative to reverse
     */
    void move(long steps);
    /*
     * Rotate the motor a given number of degrees (1-360)
     */
    void rotate(long deg);
    inline void rotate(int deg){
        rotate((long)deg);
    };
    /*
     * Rotate using a float or double for increased movement precision.
     */
    void rotate(double deg);
    /*
     * Turn off/on motor to allow the motor to be moved by hand/hold the position in place
     */
    void enable(void);
    void disable(void);
    /*
     * Methods to allow external timing control.
     * These should not be needed for normal use.
     */
    /*
     * Initiate a move (calculate and save the parameters)
     */
    void startMove(long steps);
    inline void startRotate(int deg){ 
        startRotate((long)deg);
    };
    void startRotate(long deg);
    void startRotate(double deg);
    /*
     * Toggle step and return time until next change is needed (micros)
     */
    long nextAction(void);

    /*
     * Return calculated time to complete the given move
     */
    long getTimeForMove(long steps);
    /*
     * Calculate steps needed to rotate requested angle, given in degrees
     */
    long calcStepsForRotation(long deg){
        return deg * motor_steps * (long)microsteps / 360;
    }
    long calcStepsForRotation(double deg){
        return deg * motor_steps * microsteps / 360;
    }
};
#endif // STEPPER_DRIVER_BASE_H
