/*
 * imu.h
 *
 * Created: 7/12/2015 7:58:18 PM
 *  Author: Robert Phillips III
 */ 


#ifndef IMU_H_
#define IMU_H_

#include "general.h"
#include "flight.h"

typedef enum
{
	G_SCALE_245DPS  = 0x00,	// 245 degrees per second
	G_SCALE_500DPS  = 0x01,	// 500 dps
	G_SCALE_2000DPS = 0x02,	// 2000 dps
} GyroScale;

typedef enum
{
	A_SCALE_2G  = 0x00,	// 2g
	A_SCALE_4G  = 0x01,	// 4g
	A_SCALE_6G  = 0x02,	// 6g
	A_SCALE_8G  = 0x03,	// 8g
	A_SCALE_16G = 0x04	// 16g
} AccelScale;

typedef enum
{
	M_SCALE_2GS  = 0x00, // 2Gs
	M_SCALE_4GS  = 0x01, // 4Gs
	M_SCALE_8GS  = 0x02, // 8Gs
	M_SCALE_12GS = 0x03, // 12Gs
} MagScale;

uint16_t imu_init(void);

uint8_t imu_whoami_gyro(void);
uint8_t imu_whoami_xm(void);

bool imu_read_accel(struct accelerometer *accel);
bool imu_read_gyro(struct gyroscope *gyro);
bool imu_read_mag(struct magnetometer *mag);


#endif /* IMU_H_ */