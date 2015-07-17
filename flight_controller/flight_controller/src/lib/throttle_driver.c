/*
 * throttle.c
 *
 * Created: 7/8/2015 7:55:38 PM
 *  Author: Robert Phillips III
 */ 

#include <avr/io.h>
#include "throttle_driver.h"

#define TICKS_PER_50Hz 40000

#define TIMER      TCC0
#define TIMER_PORT PORTC

void throttle_init()
{
	/* configures bits 0-3 of the timer port as outputs for pwm */
	TIMER_PORT.DIR |= 0x0F;
	
	/* no clock prescaler */
	TIMER.CTRLA = TC0_CLKSEL0_bm;
	
	/* enable single slope pwm mode */
	TIMER.CTRLB |= TC0_WGMODE0_bm | TC0_WGMODE1_bm;
	/* enable pin outputs for timer 0 A-D */
	TIMER.CTRLB |= TC0_CCAEN_bm | TC0_CCBEN_bm | TC0_CCCEN_bm | TC0_CCDEN_bm;
	
	/* set the top of the pwm to create a 50Hz signal */
	TIMER.PER = TICKS_PER_50Hz;
	
	throttle_set(A, THROTTLE_MIN);
	throttle_set(B, THROTTLE_MIN);
	throttle_set(C, THROTTLE_MIN);
	throttle_set(D, THROTTLE_MIN);
}

void throttle_set(ThrottleOutput output, uint16_t throttle)
{
	if (throttle > THROTTLE_MAX) {
		return;
	}
	
	uint16_t val = 2000 + throttle*2;
	
	switch (output) {
	case A: TIMER.CCA = val; break;
	case B: TIMER.CCB = val; break;
	case C: TIMER.CCC = val; break;
	case D: TIMER.CCD = val; break;
	}
}