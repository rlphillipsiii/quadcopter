/*
 * filter.c
 *
 * Created: 7/9/2015 11:10:14 PM
 *  Author: Robert Phillips III
 */ 

#include <avr/io.h>

#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "filter.h"

#define TIMER           TCF0
#define TIMER_TOP       500
#define TIMER_PRESCALER 64.0
#define TIMER_FREQUENCY ((F_CPU/TIMER_PRESCALER)/TIMER_TOP)
#define TIMER_PERIOD    1.0/TIMER_FREQUENCY

extern float g_filter_dt = TIMER_PERIOD;

static const float g_rads = 180/M_PI;

static const float tau = 0.075;

/* make sure dt is in seconds */
void _time_slice(struct gyroscope *gyro, float dt)
{
	gyro->roll  = gyro->x*dt;
	gyro->pitch = gyro->y*dt;
	gyro->yaw   = gyro->z*dt;
}

void _find_angles(struct accelerometer *accel)
{
	float x = accel->x;
	float y = accel->y;
	float z = accel->z;
	
	accel->roll  = atan2f(x, sqrt((y*y) + (z*z)))*g_rads;
	accel->pitch = atan2f(y, sqrt((x*x) + (z*z)))*g_rads;
}

void complimentary(
	struct flight *data, 
	struct gyroscope *gyro, 
	struct accelerometer *accel, 
	struct magnetometer *mag, 
	float dt)
{
	_time_slice(gyro, dt);
	_find_angles(accel);
		
	float alpha = tau/(tau + dt);
	alpha = 0.987;
	
	data->pitch = alpha*(data->pitch + gyro->pitch) + (1 - alpha)*accel->pitch;
	data->roll  = alpha*(data->roll + gyro->roll) + (1 - alpha)*accel->roll;
}

void filter_init()
{
// 	TIMER.CTRLA    = TC_CLKSEL_DIV64_gc;
// 	TIMER.INTCTRLA = 0x01;
// 	TIMER.PER      = TIMER_TOP;
}