//***************************************
//*                                     *
//* GRBL Z-AXIS SERVO ADAPTER           *
//*                                     *
//***************************************
//* Description:                        *
//*                                     *
//* Simulates an axis, drives RC Servo. *
//*                                     *
//* Want to use a servo in GRBL for the *
//* Z axis, but don't want to change    *
//* your firmware?                      *
//*                                     *
//* This quick hack is designed as a    *
//* plug- in replacement for a stepper  *
//* driver such as the easydriver or    *
//* Pololu driver.                      *
//* It reads the DIR and STEP signals   *
//* and converts an internal step       *
//* counter into PWM for an RC servo.   *
//*                                     *
//* Great for use in existing hardware. *
//* No fundamental changes in firmware  *
//* are needed.                         *
//*                                     *
//***************************************
//* Pins:                               *
//*          ______                     *
//*   RESET-|      |-+5V                *
//*  HOME_Z-|ATTiny|-STEP               *
//*        -|25/85 |-DIR                *
//*     GND-|______|-Servo signal       *
//*                                     *
//* Connect the parts to the respective *
//* pins. Set step direction to         *
//* inverted in your Machine.           *
//*                                     *
//***************************************

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SERVO_PIN PB0
#define DIRECTION_PIN PB1
#define HOME_PIN PB3

// DO NOT CHANGE STEP_PIN (will trigger interrupt on signal edge)
#define STEP_PIN PB2

// If you define HOME_CLOCKWISE, the home position is at the PWM maximum
// (usually the far right/clockwise end position). If it is undefined,
// the home position is at the PWM minimum (usually left/counter-clockwise end).
//#define HOME_CLOCKWISE

// Experiment until you find the values for servo completely up or down.
// Default values should work OK when using a pen and reversed Z axis.
// The values are roughly microseconds. Typical servos need values between
// 1000 and 2000 us for a full sweep, with 1500 us being the center position.
#define PWM_MIN 1200
#define PWM_MAX 1450

// Servo movement per step
#define PWM_RESOLUTION 10

// Current pulse width setting
volatile int pwm;

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
  //IO-Ports setup
  DDRB |=(1<<SERVO_PIN) |(1<<HOME_PIN);
  //PORTB |=(1<<DIRECTION_PIN)|(1<<STEP_PIN);
    
  //Falling-edge Interrupt on PB2
  MCUCR |=(1<<ISC01);
  GIMSK |=(1<<INT0);
    
  //Timer0 setup
  TCCR0A |=(1<<WGM01); //CTC mode, set OC0A on match
  TCCR0B |=(1<<CS02)|(1<<CS00);  //Prescaler 1024
    
  TIMSK |=(1<<OCIE0A);  //CTC Interrupt enable
   
  OCR0A= (8000000U/(1024U*50U)-1U);  //Set Timer to ~100Hz
    
  //Set Z-servo to max
  pwm=PWM_MAX;  
}


int main (void)
{
  init();
  sei();
  while(1)
  {
    #ifdef HOME_CLOCKWISE
        
    if (pwm<=PWM_MIN)
    {
      pwm=PWM_MIN;
    }
    
    if (pwm <PWM_MAX)
    {
      PORTB |=(1<<HOME_PIN);
    }
    
    if (pwm>=PWM_MAX)
    {
      pwm=PWM_MAX;
      PORTB &= ~(1<<HOME_PIN);
    }
    #else 
    if (pwm<=PWM_MIN)
    {
      pwm=PWM_MIN;
      PORTB &= ~(1<<HOME_PIN);
    }
        
    if (pwm >PWM_MIN)
    {
      PORTB |=(1<<HOME_PIN);
    }
    
    if (pwm>=PWM_MAX)
    {
      pwm=PWM_MAX;
    }
    #endif
  }
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
  if (!(PINB & (1<< DIRECTION_PIN))) 
  {
    pwm-=PWM_RESOLUTION;
  }
  else 
  {
    pwm+=PWM_RESOLUTION;
  }
}


