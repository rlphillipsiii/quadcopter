/*
 * twi_master_driver.c
 *
 * Created: 7/8/2015 7:55:38 PM
 *  Author: Robert Phillips III
 */ 

#include <sysclk.h>
#include <twim.h>
#include "twi_master_driver.h"

#define TWI_MASTER       TWIE
#define TWI_MASTER_PORT  PORTE
#define TWI_SPEED        50000
#define TWI_MASTER_ADDR  0x50

#define TWI_READ  0
#define TWI_WRITE 1

twi_options_t m_options = {
	.speed = TWI_SPEED,
	.chip  = TWI_MASTER_ADDR
};

void twi_init()
{
	TWI_MASTER_PORT.PIN0CTRL = PORT_OPC_WIREDANDPULL_gc;
	TWI_MASTER_PORT.PIN1CTRL = PORT_OPC_WIREDANDPULL_gc;

	m_options.speed_reg = TWI_BAUD(sysclk_get_cpu_hz(), TWI_SPEED);
	
	sysclk_enable_peripheral_clock(&TWI_MASTER);
	twi_master_init(&TWI_MASTER, &m_options);
	twi_master_enable(&TWI_MASTER);
}

void _twi_transaction(uint8_t type, TwiDevice dev, uint8_t reg, uint8_t *buffer, const uint8_t length)
{
	uint8_t i;
	uint8_t buf[length+1];
	
	twi_package_t packet = {
		.addr_length = 0,
		.buffer      = (void *)buffer,
		.length      = length + 1,
		.no_wait     = false
	};
	
	switch (dev) {
		case GYRO: packet.chip = TWI_GYRO_ADDR; break;
		case XM:   packet.chip = TWI_XM_ADDR;   break;
	}
	
	/* if we are performing a write then append the register address */
	if (type == TWI_WRITE) {
		buf[0] = reg;
		for (i = 0; i < length; i++) {
			buf[i+1] = buffer[i];
		}
		
		packet.buffer = (void *)&buf[0];
		twi_master_write(&TWI_MASTER, &packet);
	} else {
		twi_master_read(&TWI_MASTER, &packet);
	}
}

void twi_read(TwiDevice dev, uint8_t reg, uint8_t *buffer, uint8_t length) 
{
	uint8_t buf[1];
	_twi_transaction(TWI_WRITE, dev | 0x80, reg, &buf[0], 0);
	_twi_transaction(TWI_READ, dev, reg, buffer, length);
}

void twi_write(TwiDevice dev, uint8_t reg, uint8_t *buffer, uint8_t length)
{
	_twi_transaction(TWI_WRITE, dev, reg, buffer, length);
}

