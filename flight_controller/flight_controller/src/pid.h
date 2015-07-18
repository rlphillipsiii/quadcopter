/*
 * pid.h
 *
 * Created: 7/17/2015 1:49:31 PM
 *  Author: Robert Phillips III
 */ 


#ifndef PID_H_
#define PID_H_

#include "general.h"
#include "flight.h"

void pid_loop(struct flight *meas, float dt);

void pid_init(struct flight *meas);

#endif /* PID_H_ */