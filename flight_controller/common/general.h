/*
 * general.h
 *
 * Created: 7/8/2015 8:48:48 PM
 *  Author: Robert Phillips III
 */ 


#ifndef GENERAL_H_
#define GENERAL_H_

#include <stdint.h>

#include "pid.h"

#ifndef bool
#	define bool uint8_t
#endif

#ifndef true
#	define true  1
#endif

#ifndef false
#	define false 0
#endif

#define ISSET(reg, bit) (reg & (1 << bit))
#define CTRL_LOOP_HZ 200

#endif /* GENERAL_H_ */