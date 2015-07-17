/*
 * throttle_driver.h
 *
 * Created: 7/8/2015 7:55:50 PM
 *  Author: Robert Phillips III
 */ 


#ifndef THROTTLE_H_
#define THROTTLE_H_

#define THROTTLE_MAX 1000
#define THROTTLE_MIN 0

typedef enum {
	A,
	B,
	C,
	D,	
} ThrottleOutput;

void throttle_init(void);

void throttle_set(ThrottleOutput output, uint16_t throttle);

#endif /* THROTTLE_H_ */