//***************************************************************************
//*                                                                         *
//* Z-AXIS STEPPER-TO-SERVO ADAPTER                                         *
//*                                                                         *
//* Configuration header -- adapt these settings to your needs              *
//*                                                                         *
//***************************************************************************
//* Original code published by NilsRoe on Thingiverse                       *
//* (http://www.thingiverse.com/thing:224969)                               *
//* Extended by Jan Reucker.                                                *
//* Licensed under Creative Commons - Attribution - Non-Commercial          *
//* (https://creativecommons.org/licenses/by-nc/3.0/legalcode)              *
//***************************************************************************
#ifndef CONFIG_H
#define CONFIG_H

// If you define HOME_CLOCKWISE, the home position is at the PWM minimum
// (usually the far right/clockwise end position). If it is undefined,
// the home position is at the PWM maximum (usually left/counter-clockwise end).
#define HOME_CLOCKWISE


// Experiment until you find the values for servo completely up or down.
// Default values should work OK when using a pen and reversed Z axis.
// The values are roughly microseconds. Typical servos need values between
// 1000 and 2000 us for a full sweep, with 1500 us being the center position.
// Choose different values if you don't need the full sweep.
#define PWM_MIN   900
#define PWM_MAX   1560


// Servo movement per step
// The amount of microseconds by which the PWM output will be changed with
// each falling edge of the clock signal. Bigger values result in more
// movement per step. Smaller values increase precision.
#define PWM_RESOLUTION 10


// Behaviour at end of travel range
// IF LIMITED_TRAVEL_RANGE is defined, the servo will behave like a stepper
// when it reaches the end of its travel range: incoming step pulses are lost,
// so the servo will lose its position reference.
// If the macro is commented out, incoming pulses will still be counted after
// the servo has reached the end of its travel range. Although it will not
// follow the incoming step commands outside its travel range, it will not lose
// its position reference. When pulsing into the opposite direction, the servo
// will start to move as soon as the axis position arives within the servo's
// travel range again.
//#define LIMITED_TRAVEL_RANGE


// Pin configuration
// Available pins: PB0, PB1, PB3, PB4
// PB2 is fixed to the STEP signal, because it provides the external IRQ source
#define SERVO_PIN       PB0
#define DIRECTION_PIN   PB1
#define HOME_PIN        PB3


#endif // CONFIG_H

