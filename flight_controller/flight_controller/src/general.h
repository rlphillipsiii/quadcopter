/*
 * general.h
 *
 * Created: 7/8/2015 8:48:48 PM
 *  Author: Robert Phillips III
 */ 


#ifndef GENERAL_H_
#define GENERAL_H_

#include <stdint.h>

#ifndef bool
#	define bool uint8_t
#endif

struct accelerometer {
	float x;
	float y;
	float z;
	
	float roll;
	float pitch;
};

struct magnetometer {
	float x;
	float y;
	float z;
};

struct gyroscope {
	float x;
	float y;
	float z;
};

#define true  1
#define false 0

#endif /* GENERAL_H_ */