/*
 * serial.h
 *
 * Created: 8/5/2016 12:52:19 AM
 *  Author: Robert
 */ 

#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdint.h>

#include "serial_device.h"

void serial_init(SerialProtocol protocol);

SerialStatusCode serial_read(SerialDevice dev, uint8_t reg, uint8_t *buffer, uint8_t length);
SerialStatusCode serial_write(SerialDevice dev, uint8_t reg, uint8_t *buffer, uint8_t length);

#endif /* SERIAL_H_ */