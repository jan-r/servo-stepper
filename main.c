//***************************************************************************
//*                                                                         *
//* Z-AXIS STEPPER-TO-SERVO ADAPTER                                         *
//*                                                                         *
//***************************************************************************
//* Description:                                                            *
//*                                                                         *
//* Simulates an axis, drives RC Servo.                                     *
//*                                                                         *
//* Want to use a servo in GRBL for the Z axis, but don't want to change    *
//* your firmware?                                                          *
//*                                                                         *
//* This quick hack is designed as a plug-in replacement for a stepper      *
//* driver such as the easydriver or Pololu driver. It reads the DIR and    *
//* STEP signals and converts an internal step counter into PWM for an      *
//* RC servo.                                                               *
//*                                                                         *
//* Great for use in existing hardware. No fundamental changes in firmware  *
//* are needed.                                                             *
//*                                                                         *
//***************************************************************************
//* Pins (unless changed in config.h):                                      *
//*          ______                                                         *
//*   RESET-|      |-+5V                                                    *
//*  HOME_Z-|ATTiny|-STEP                                                   *
//*        -|25/85 |-DIR                                                    *
//*     GND-|______|-Servo signal                                           *
//*                                                                         *
//* Connect the parts to the respective pins. Set step direction to         *
//* inverted in your Machine.                                               *
//*                                                                         *
//***************************************************************************
//* Fuse values:                                                            *
//*                                                                         *
//* lfuse: 0xe2                                                             *
//* hfuse: 0xdf                                                             *
//*                                                                         *
//***************************************************************************
//* Original code published by NilsRoe on Thingiverse                       *
//* (http://www.thingiverse.com/thing:224969)                               *
//* Extended by Jan Reucker.                                                *
//* Licensed under Creative Commons - Attribution - Non-Commercial          *
//* (https://creativecommons.org/licenses/by-nc/3.0/legalcode)              *
//***************************************************************************
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"

// DO NOT CHANGE STEP_PIN (will trigger interrupt on signal edge)
#define STEP_PIN PB2

// Set the "home" signal (pull home pin LOW) --> axis homed
#define SIGNAL_HOME()       do{PORTB &= ~(1<<HOME_PIN);}while(0)

// Reset the "home" signal (pull home pin HIGH) --> axis not homed
#define SIGNAL_NOT_HOME()   do{PORTB |= (1<<HOME_PIN);}while(0)

// Current pulse width setting
int pwm;

// Block for the given amount of microseconds.
// _delay_us() is limited to 768us/(F_CPU in MHz) = 96us @8MHz.
// This function therefore breaks down the delay into several calls to
// _delay_us().
void delayus(unsigned int useconds)
{
  // coarse delay in 50us intervals
  while (useconds > 50)
  {
    useconds -= 50;
    _delay_us(50);
  }
  while (useconds-- > 0)
  {
    _delay_us(1);
  }
}

void init (void)
{
  // IO-Ports setup
  DDRB |=(1<<SERVO_PIN) |(1<<HOME_PIN);
  // PORTB |=(1<<DIRECTION_PIN)|(1<<STEP_PIN);
    
  // Falling-edge Interrupt on PB2
  MCUCR |=(1<<ISC01);
  GIMSK |=(1<<INT0);
    
  // Timer0 setup
  TCCR0A |=(1<<WGM01);          // CTC mode, set OC0A on match
  TCCR0B |=(1<<CS02)|(1<<CS00); // Prescaler 1024
    
  TIMSK |=(1<<OCIE0A);  //CTC Interrupt enable
   
  OCR0A= (8000000U/(1024U*50U)-1U);  //Set Timer to ~100Hz
    
  // Set Z-servo to home position and activate home signal
  #ifdef HOME_CLOCKWISE
  pwm = PWM_MIN;
  #else
  pwm = PWM_MAX;
  #endif
  SIGNAL_HOME();
}


int main (void)
{
  init();
  sei();

  // endless loop; work is done in the ISRs
  while(1)
    ;

  return 0;
}

// Software PWM
ISR(TIMER0_COMPA_vect)
{
  PORTB |=(1<<SERVO_PIN); //Servo pin on

  //Wait for PWM to finish    
  delayus(pwm);

  PORTB &=~(1<<SERVO_PIN); //Servo pin off
}


// Falling edge interrupt
// Count step, depending on direction setting.
ISR(INT0_vect)
{
  uint8_t u8_homed = 0;

  if (!(PINB & (1<< DIRECTION_PIN))) 
  {
    pwm -= PWM_RESOLUTION;
    if (pwm <= PWM_MIN)
    {
      pwm = PWM_MIN;
      #ifdef HOME_CLOCKWISE
      u8_homed = 1U;
      #endif
    }
  }
  else 
  {
    pwm += PWM_RESOLUTION;
    if (pwm >= PWM_MAX)
    {
      pwm = PWM_MAX;
      #ifndef HOME_CLOCKWISE
      u8_homed = 1U;
      #endif
    }
  }

  // handle home pin
  if (u8_homed)
  {
    // pull low (signal "at home position")
    SIGNAL_HOME();
  }
  else
  {
    // set high (not homed)
    SIGNAL_NOT_HOME();
  }
}


