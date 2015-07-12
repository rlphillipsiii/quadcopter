/*
 * filter.c
 *
 * Created: 7/9/2015 11:10:14 PM
 *  Author: Robert Phillips III
 */ 

#include <math.h>
#include <stdint.h>
#include "filter.h"

const double g_rads = 180/M_PI;

void find_angles(struct accelerometer *accel)
{
	uint16_t x = accel->x;
	uint16_t y = accel->y;
	uint16_t z = accel->z;
	
	accel->roll  = atan2(y, z)*g_rads;
	accel->pitch = atan2(-x, sqrt(y*y + z*z))*g_rads;
}