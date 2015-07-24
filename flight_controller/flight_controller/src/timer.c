/*
 * timer.c
 *
 * Created: 7/17/2015 9:40:53 AM
 *  Author: Robert Phillips III
 */ 

#include <avr/io.h>

#define TIMER           TCF0

#if (F_CPU == 2000000)
#	define TIMER_PRESCALER 8.0
#	define CLK_PRESCALER   TC_CLKSEL_DIV8_gc
#else if (F_CPU == 32000000)
#	define TIMER_PRESCALER 256.0
#	define CLK_PRESCALER   TC_CLKSEL_DIV256_gc
#endif

#define TIMER_FREQUENCY (F_CPU/TIMER_PRESCALER)

static const float seconds_per_tick = 1.0/TIMER_FREQUENCY;

void timer_init()
{
	/* clk/256 prescaler */
	TIMER.CTRLA = CLK_PRESCALER;
	
	/* normal mode */
	TIMER.CTRLB = TC_WGMODE_NORMAL_gc;
	
	/* max top for the timer */
	TIMER.PER = 0xFFFF;
}

void timer_reset()
{
	TIMER.CNT = 0;	
}

float timer_clock()
{
	uint16_t ticks = TIMER.CNT;
	
	TIMER.CNT = 0;
	
	return (ticks * seconds_per_tick);
}