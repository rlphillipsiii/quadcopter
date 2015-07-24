/*
 * throttle.c
 *
 * -------------------------------------------------------
 *             A       B
 *              \     /
 *               \   /
 *                \ /
 *                / \
 *               /   \
 *              /     \
 *             C       D
 * -------------------------------------------------------
 *
 * Created: 7/8/2015 7:55:38 PM
 *  Author: Robert Phillips III
 */ 

#include <avr/io.h>

#include <stdio.h>
#include "lib/lcd_driver.h"

#include "general.h"
#include "throttle_driver.h"

#if (F_CPU == 2000000)
#	define TICKS_PER_50Hz      40000
#	define CLK_PRESCALER       TC_CLKSEL_DIV1_gc
#   define THROTTLE_MULTIPLIER 2

#else if (F_CPU == 32000000)
#	define TICKS_PER_50Hz      10000
#   define CLK_PRESCALER       TC_CLKSEL_DIV64_gc
#	define THROTTLE_MULTIPLIER 0.5

#endif

#define TIMER      TCC0
#define TIMER_PORT PORTC

static uint16_t g_throttle_max = (uint16_t) TICKS_PER_50Hz*.1;
static uint16_t g_throttle_min = (uint16_t) TICKS_PER_50Hz*.05;

/* variable indicating if the throttle was killed */
static bool g_no_throttle = true;

/* base throttle for each motor */
static uint16_t g_target = 0;

/* throttle compensation for each motor */
static int16_t g_comp_a = 0;
static int16_t g_comp_b = 0;
static int16_t g_comp_c = 0;
static int16_t g_comp_d = 0;

void throttle_init()
{
	/* configures bits 0-3 of the timer port as outputs for pwm */
	TIMER_PORT.DIR |= 0x0F;
	
	/* no clock prescaler */
	TIMER.CTRLA = CLK_PRESCALER;
	
	/* enable single slope pwm mode */
	TIMER.CTRLB |= TC0_WGMODE0_bm | TC0_WGMODE1_bm;
	/* enable pin outputs for timer 0 A-D */
	TIMER.CTRLB |= TC0_CCAEN_bm | TC0_CCBEN_bm | TC0_CCCEN_bm | TC0_CCDEN_bm;
	
	/* set the top of the pwm to create a 50Hz signal */
	TIMER.PER = TICKS_PER_50Hz;
	
	throttle_kill();
}

void throttle_kill()
{
	g_target = 0;
	g_no_throttle = true;
	
	TIMER.CCA = g_throttle_min;
	TIMER.CCB = g_throttle_min;
	TIMER.CCC = g_throttle_min;
	TIMER.CCD = g_throttle_min;
}

uint16_t _throttle_correction(uint16_t requested)
{
	if (requested > g_throttle_max) return g_throttle_max;
	if (requested < g_throttle_min) return g_throttle_min;
	
	return requested;
}

void _throttle_update()
{
	uint16_t a = g_target + g_comp_a;
	uint16_t b = g_target + g_comp_b;
	uint16_t c = g_target + g_comp_c;
	uint16_t d = g_target + g_comp_d;

	TIMER.CCA = _throttle_correction(a);
	//TIMER.CCB = _throttle_correction(b);
	//TIMER.CCC = _throttle_correction(c);
	TIMER.CCD = _throttle_correction(d);
}

void throttle_adjust(struct flight *data)
{
	int16_t roll_factor  = (int16_t) (data->pid_roll->output + 0.5);
	int16_t pitch_factor = (int16_t) (data->pid_pitch->output + 0.5);
	
	g_comp_a = g_comp_b = g_comp_c = g_comp_d = 0;
	
	/* left motors */
	g_comp_a -= roll_factor;
	g_comp_c -= roll_factor;
	
	/* right motors */
	g_comp_b += roll_factor;
	g_comp_d += roll_factor;
	
	/* front motors */
	g_comp_a += pitch_factor;
	g_comp_b += pitch_factor;
	
	/* back motors */
	g_comp_c -= pitch_factor;
	g_comp_d -= pitch_factor;
	
	_throttle_update();
}

bool throttle_active()
{
	return g_no_throttle^0x01;
}

void throttle_set(uint16_t throttle)
{
	if (throttle > THROTTLE_MAX) {
		return;
	}
	
	if (throttle == 0) {
		throttle_kill();
		return;
	}
	
	g_no_throttle = false;
	g_target = g_throttle_min + ((uint16_t)(throttle*THROTTLE_MULTIPLIER));
	
	_throttle_update();
}