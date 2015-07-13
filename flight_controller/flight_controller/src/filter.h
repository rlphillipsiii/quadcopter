/*
 * filter.h
 *
 * Created: 7/9/2015 11:09:59 PM
 *  Author: Robert Phillips III
 */ 


#ifndef FILTER_H_
#define FILTER_H_

struct accelerometer {
	uint16_t x;
	uint16_t y;
	uint16_t z;
	
	float roll;
	float pitch;
};



#endif /* FILTER_H_ */