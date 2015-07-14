/*
 * twi_master_driver.h
 *
 * Created: 7/8/2015 7:55:38 PM
 *  Author: Robert Phillips III
 */ 

#ifndef TWI_MASTER_DRIVER_H_
#define TWI_MASTER_DRIVER_H_

#include "general.h"
#include <stdint.h>

#define TWI_MAX_DATA_LENGTH 8

#define TWI_GYRO_ADDR 0x6B
#define TWI_XM_ADDR   0x1D

typedef enum {
	GYRO,
	XM,
} TwiDevice;

void twi_init(void);

void twi_read(TwiDevice dev, uint8_t *buffer, uint8_t length);
void twi_write(TwiDevice dev, uint8_t *buffer, uint8_t length);

#endif