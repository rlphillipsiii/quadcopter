/*
 * filter.h
 *
 * Created: 7/9/2015 11:09:59 PM
 *  Author: Robert Phillips III
 */ 


#ifndef FILTER_H_
#define FILTER_H_

#include "general.h"
#include "flight.h"

void filter_init(void);

void complimentary(struct flight *data, struct gyroscope *gyro, struct accelerometer *accel, struct magnetometer *mag, float dt);

#endif /* FILTER_H_ */