/*
 * filter.c
 *
 * Created: 7/9/2015 11:10:14 PM
 *  Author: Robert Phillips III
 */ 

#include <math.h>
#include <stdint.h>
#include "filter.h"

static const float g_rads = 180/M_PI;

static const float tau = 0.075;

/* make sure dt is in seconds */
void _time_slice(struct gyroscope *gyro, float dt)
{
	gyro->pitch = gyro->x*dt;
	gyro->roll  = gyro->y*dt;
	gyro->yaw   = gyro->z*dt;
}

void _find_angles(struct accelerometer *accel)
{
	float x = accel->x;
	float y = accel->y;
	float z = accel->z;
	
	accel->roll  = atan2(y, sqrt(x*x) + (z*z))*g_rads;
	accel->pitch = atan2(x, sqrt(y*y) + (z*z))*g_rads;
}

void complimentary(struct flight *data, struct gyroscope *gyro, struct accelerometer *accel, struct magnetometer *mag, float dt)
{
	_time_slice(gyro, dt);
	_find_angles(accel);
	
	float alpha = tau/(tau + dt);
	
	data->pitch = alpha*(data->pitch + gyro->pitch) + (1 - alpha)*accel->pitch;
	data->roll  = alpha*(data->roll + gyro->roll) + (1 - alpha)*accel->roll;
}