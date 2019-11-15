/*
 * serial.c
 *
 * Created: 8/5/2016 12:52:07 AM
 *  Author: Robert
 */ 

#include "serial.h"

#include "twi_master_driver.h"

static SerialProtocol g_protocol;

void serial_init(SerialProtocol protocol)
{
	switch (protocol) {
	case I2C:
		twi_init();
		break;
		
	case SPI:
		// TODO: spi init
		break;
		
	default: break;
	}
	
	g_protocol = protocol;
}

SerialStatusCode serial_read(
	SerialDevice dev, 
	uint8_t reg, 
	uint8_t *buffer, 
	uint8_t length)
{
	switch (g_protocol) {
	case I2C:
		return twi_read(dev, reg, buffer, length);
		break;
		
	case SPI:
	
	default: break;
	}
	
	return OK;
}

SerialStatusCode serial_write(
	SerialDevice dev, 
	uint8_t reg, 
	uint8_t *buffer, 
	uint8_t length)
{
	switch (g_protocol)	{
	case I2C:
		return twi_write(dev, reg, buffer, length);
		break;
		
	case SPI:
		break;
		
	default: break;
	}
	
	return OK;
}