/*
 * timer.c
 *
 * Created: 7/17/2015 9:40:53 AM
 *  Author: Robert Phillips III
 */ 

#include <avr/io.h>
#include "timer.h"

#define TIMER           TCF0

#define TIMER_PRESCALER 8.0
#define TIMER_FREQUENCY (F_CPU/TIMER_PRESCALER)

static const float seconds_per_tick = 1.0/TIMER_FREQUENCY;

void timer_init(void)
{
	/* clk/8 prescaler */
	TIMER.CTRLA = TC_CLKSEL_DIV8_gc;
	
	/* normal mode */
	TIMER.CTRLB = TC_WGMODE_NORMAL_gc;
	
	/* max top for the timer */
	TIMER.PER = 0xFFFF;
}

void timer_reset(void)
{
	TIMER.CNT = 0;	
}

float timer_clock(void)
{
	uint16_t ticks = TIMER.CNT;
	
	TIMER.CNT = 0;
	
	return (ticks * seconds_per_tick);
}