/*
 * twi_master_driver.h
 *
 * Created: 7/8/2015 7:55:38 PM
 *  Author: Robert Phillips III
 */ 

#ifndef TWI_MASTER_DRIVER_H_
#define TWI_MASTER_DRIVER_H_

#include <stdint.h>

#include "general.h"

#include "serial_device.h"

#define TWI_MAX_DATA_LENGTH 8

#define TWI_GYRO_ADDR 0x6B
#define TWI_XM_ADDR   0x1D

void twi_init(void);

SerialStatusCode twi_read(uint8_t dev, uint8_t reg, uint8_t *buffer, uint8_t length);
SerialStatusCode twi_write(uint8_t dev, uint8_t reg, uint8_t *buffer, uint8_t length);

#endif