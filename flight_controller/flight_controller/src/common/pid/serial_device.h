/*
 * serial_device.h
 *
 * Created: 8/5/2016 1:21:06 AM
 *  Author: Robert
 */ 


#ifndef SERIAL_DEVICE_H_
#define SERIAL_DEVICE_H_

typedef enum {
	I2C = 0x00,
	SPI = 0x01
} SerialProtocol;

typedef enum {
	GYRO = 0x00,
	XM   = 0x01,
	MAG  = 0x02
} SerialDevice;

typedef enum {
	OK    = 0x00,
	ERROR = 0x01
} SerialStatusCode;



#endif /* SERIAL_DEVICE_H_ */