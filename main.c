//***************************************
//*                                     *
//* GRBL Z-AXIS SERVO ADAPTER           *
//*                                     *
//***************************************
//* Description:						*
//* 									*
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
//* are needed.  						*
//*										*
//***************************************
//* Pins:								*
//*          ______						*
//*   RESET-|      |-+5V				*
//*  HOME_Z-|ATTiny|-STEP				*
//*        -|25/85 |-DIR				*
//*     GND-|______|-Servo signal		*
//*										*
//* Connect the parts to the respective	*
//* pins. Set step direction to 		*
//* inverted in your Machine.           *
//* 									*
//***************************************




#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define SERVO_PIN PB0
#define DIRECTION_PIN PB1
#define HOME_PIN PB3

//DO NOT CHANGE STEP_PIN
#define STEP_PIN PB2

//Uncomment if you're using the Z-Axis for a pen and want to move it up when homing
#define REVERSE_Z_BEHAVIOUR


//Experiment until you find the values for servo completely up or down.
//Default values should work OK when using a pen and reversed Z axis
#define PWM_MIN 50
#define PWM_MAX 400

//Changes "sensivity" of servo movement per step
#define PWM_RESOLUTION 20
volatile int pwm;


void delayus(int useconds)
{
for (int _temp=0;_temp<useconds;_temp++)
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
	
	OCR0A= (8000000/(1024*50)-1);  //Set Timer to ~100Hz
	
	//Set Z-servo to max
	pwm=PWM_MAX;
	
}



int main (void)
{
	init();
	sei();
	while(1)
	{
	
		#ifdef REVERSE_Z_BEHAVIOUR
		
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

//Software PWM
ISR(TIMER0_COMPA_vect)
{
PORTB |=(1<<SERVO_PIN); //Servo pin on

_delay_us(550); 		//Wait for PWM to finish	
delayus(pwm);

PORTB &=~(1<<SERVO_PIN); //Servo pin off

}


//Falling edge interrupt
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


