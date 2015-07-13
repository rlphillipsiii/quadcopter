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

twi_options_t m_options = {
	.speed = TWI_SPEED,
	.chip  = TWI_MASTER_ADDR
};

void init_twi()
{
	TWI_MASTER_PORT.PIN0CTRL = PORT_OPC_WIREDANDPULL_gc;
	TWI_MASTER_PORT.PIN1CTRL = PORT_OPC_WIREDANDPULL_gc;

	m_options.speed_reg = TWI_BAUD(sysclk_get_cpu_hz(), TWI_SPEED);
	
	sysclk_enable_peripheral_clock(&TWI_MASTER);
	twi_master_init(&TWI_MASTER, &m_options);
	twi_master_enable(&TWI_MASTER);
}

void twi_init(TwiDevice dev, uint8_t *buffer) 
{
	twi_package_t packet = {
		.addr_length = 0,
		.buffer      = (void *)buffer,
		.length      = TWI_DATA_LENGTH,
		.no_wait     = false
	};
	
	switch (dev) {
		case GYRO: packet.chip = TWI_GYRO_ADDR; break;
		case XM:   packet.chip = TWI_XM_ADDR;   break;
	}
	
	twi_master_read(&TWI_MASTER, &packet);
}