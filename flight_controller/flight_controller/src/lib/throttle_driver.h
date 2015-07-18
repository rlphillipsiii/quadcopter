/*
 * throttle_driver.h
 *
 * Created: 7/8/2015 7:55:50 PM
 *  Author: Robert Phillips III
 */ 


#ifndef THROTTLE_H_
#define THROTTLE_H_

#include "flight.h"

#define THROTTLE_MAX 1000
#define THROTTLE_MIN 0

#define NO_THROTTLE  2000
#define MAX_THROTTLE 4000

void throttle_init(void);

void throttle_set(uint16_t throttle);
void throttle_adjust(struct flight *data);

bool throttle_active(void);
void throttle_kill(void);

#endif /* THROTTLE_H_ */