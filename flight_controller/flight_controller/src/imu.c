/*
 * imu.c
 *
 * Created: 7/12/2015 7:58:07 PM
 *  Author: Robert Phillips III
 */ 

#include <status_codes.h>

#include "imu.h"
#include "lib/twi_master_driver.h"

#define XM_WRITE(reg, length)   twi_write(XM, reg, &g_data[0], length);
#define GYRO_WRITE(reg, length) twi_write(GYRO, reg, &g_data[0], length);

#define XM_READ(reg, length)    twi_read(XM, reg, &g_data[0], length);
#define GYRO_READ(reg, length)  twi_read(GYRO, reg, &g_data[0], length);

typedef enum {
	WHO_AM_I_G		= 0x0F,
	CTRL_REG1_G		= 0x20,
	CTRL_REG2_G		= 0x21,
	CTRL_REG3_G		= 0x22,
	CTRL_REG4_G		= 0x23,
	CTRL_REG5_G		= 0x24,
	REFERENCE_G		= 0x25,
	STATUS_REG_G	= 0x27,
	OUT_X_L_G		= 0x28,
	OUT_X_H_G		= 0x29,
	OUT_Y_L_G		= 0x2A,
	OUT_Y_H_G		= 0x2B,
	OUT_Z_L_G		= 0x2C,
	OUT_Z_H_G		= 0x2D,
	FIFO_CTRL_REG_G	= 0x2E,
	FIFO_SRC_REG_G	= 0x2F,
	INT1_CFG_G		= 0x30,
	INT1_SRC_G		= 0x31,
	INT1_THS_XH_G	= 0x32,
	INT1_THS_XL_G	= 0x33,
	INT1_THS_YH_G	= 0x34,
	INT1_THS_YL_G	= 0x35,
	INT1_THS_ZH_G	= 0x36,
	INT1_THS_ZL_G	= 0x37,
	INT1_DURATION_G	= 0x38,
} GyroRegister;

typedef enum {
	WHO_AM_I_XM			= 0x0F,
	OUT_TEMP_L_XM		= 0x05,
	OUT_TEMP_H_XM		= 0x06,
	STATUS_REG_M		= 0x07,
	OUT_X_L_M			= 0x08,
	OUT_X_H_M			= 0x09,
	OUT_Y_L_M			= 0x0A,
	OUT_Y_H_M			= 0x0B,
	OUT_Z_L_M			= 0x0C,
	OUT_Z_H_M			= 0x0D,
	INT_CTRL_REG_M		= 0x12,
	INT_SRC_REG_M		= 0x13,
	INT_THS_L_M			= 0x14,
	INT_THS_H_M			= 0x15,
	OFFSET_X_L_M		= 0x16,
	OFFSET_X_H_M		= 0x17,
	OFFSET_Y_L_M		= 0x18,
	OFFSET_Y_H_M		= 0x19,
	OFFSET_Z_L_M		= 0x1A,
	OFFSET_Z_H_M		= 0x1B,
	REFERENCE_X			= 0x1C,
	REFERENCE_Y			= 0x1D,
	REFERENCE_Z			= 0x1E,
	CTRL_REG0_XM		= 0x1F,
	CTRL_REG1_XM		= 0x20,
	CTRL_REG2_XM		= 0x21,
	CTRL_REG3_XM		= 0x22,
	CTRL_REG4_XM		= 0x23,
	CTRL_REG5_XM		= 0x24,
	CTRL_REG6_XM		= 0x25,
	CTRL_REG7_XM		= 0x26,
	STATUS_REG_A		= 0x27,
	OUT_X_L_A			= 0x28,
	OUT_X_H_A			= 0x29,
	OUT_Y_L_A			= 0x2A,
	OUT_Y_H_A			= 0x2B,
	OUT_Z_L_A			= 0x2C,
	OUT_Z_H_A			= 0x2D,
	FIFO_CTRL_REG		= 0x2E,
	FIFO_SRC_REG		= 0x2F,
	INT_GEN_1_REG		= 0x30,
	INT_GEN_1_SRC		= 0x31,
	INT_GEN_1_THS		= 0x32,
	INT_GEN_1_DURATION	= 0x33,
	INT_GEN_2_REG		= 0x34,
	INT_GEN_2_SRC		= 0x35,
	INT_GEN_2_THS		= 0x36,
	INT_GEN_2_DURATION	= 0x37,
	CLICK_CFG			= 0x38,
	CLICK_SRC			= 0x39,
	CLICK_THS			= 0x3A,
	TIME_LIMIT			= 0x3B,
	TIME_LATENCY		= 0x3C,
	TIME_WINDOW			= 0x3D,
	ACT_THS				= 0x3E,
	ACT_DUR				= 0x3F,
} XMRegister;

static uint8_t g_data[TWI_MAX_DATA_LENGTH];

static GyroScale g_gyro_scale   = G_SCALE_245DPS;
static AccelScale g_accel_scale = A_SCALE_2G;
static MagScale g_mag_scale     = M_SCALE_2GS;

static float g_gyro_res;
static float g_accel_res;
static float g_mag_res;

void _zero_buffer()
{
	uint8_t i;
	for (i = 0; i < TWI_MAX_DATA_LENGTH; i++) {
		g_data[i] = 0;
	}
}

void _setup_gyro_resolution()
{
	switch (g_gyro_scale) {
	case G_SCALE_245DPS:  g_gyro_res = 245.0 / 32768.0;  break;
	case G_SCALE_500DPS:  g_gyro_res = 500.0 / 32768.0;  break;
	case G_SCALE_2000DPS: g_gyro_res = 2000.0 / 32768.0; break;
	
	default: g_gyro_res = 1.0; break;
	}
}

void _setup_accel_resolution()
{
	g_accel_res = (g_accel_scale == A_SCALE_16G) ? 
					16.0 / 32768.0 : (((float) g_accel_scale + 1.0) * 2.0) / 32768.0;
}

void _setup_mag_resolution()
{
	g_mag_res = (g_mag_res == M_SCALE_2GS) ?
					2.0 / 32768.0 :	(float) (g_mag_scale << 2) / 32768.0;
}

void _accel_init()
{
	g_data[0] = 0x00;
	XM_WRITE(CTRL_REG0_XM, 1);
	
	// 50Hz data rate, x/y/z all enabled
	g_data[0] = 0x57;
	XM_WRITE(CTRL_REG1_XM, 1); 
					
	// Set scale to 2g
	g_data[0] = 0x00;
	XM_WRITE(CTRL_REG2_XM, 1); 
	
	// Accelerometer data ready on INT1_XM (0x04)
	g_data[0] = 0x04;
	XM_WRITE(CTRL_REG3_XM, 1);
}

void _gyro_init()
{
	// Normal mode, enable all axes
	g_data[0] = 0x0F;
	GYRO_WRITE(CTRL_REG1_G, 1);
	
	// Normal mode, high cutoff frequency
	g_data[0] = 0x00;
	GYRO_WRITE(CTRL_REG2_G, 1);

	// Int1 enabled (pp, active low), data read on DRDY_G:
	g_data[0] = 0x88;
	GYRO_WRITE(CTRL_REG3_G, 1);
	
	// Set scale to 245 dps
	g_data[0] = 0x00;
	GYRO_WRITE(CTRL_REG4_G, 1);
	
	g_data[0] = 0x00;
	GYRO_WRITE(CTRL_REG5_G, 1);
}

void _mag_init()
{
	// Mag data rate - 100 Hz, enable temperature sensor
	g_data[0] = 0x94;
	XM_WRITE(CTRL_REG5_XM, 1);
	
	// Mag scale to +/- 2GS		
	g_data[0] = 0x00;
	XM_WRITE(CTRL_REG6_XM, 1);

	// Continuous conversion mode
	g_data[0] = 0x00;
	XM_WRITE(CTRL_REG7_XM, 1);

	// Magnetometer data ready on INT2_XM (0x08)
	g_data[0] = 0x04;
	XM_WRITE(CTRL_REG4_XM, 1);

	// Enable interrupts for mag, active-low, push-pull
	g_data[0] = 0x09;
	XM_WRITE(INT_CTRL_REG_M, 1);
}

uint16_t imu_init()
{
	_setup_gyro_resolution();
	_setup_accel_resolution();
	_setup_mag_resolution();
	
	twi_init();
	
	uint8_t xm   = imu_whoami_xm();
	uint8_t gyro = imu_whoami_gyro();
	
	_accel_init();
	_gyro_init();
	_mag_init();
	
	return (gyro << 8) | xm;
}

uint8_t imu_whoami_gyro()
{
	_zero_buffer();
	
	status_code_t status = GYRO_READ(WHO_AM_I_G, 1);
	if (status != STATUS_OK) {
		uint8_t code = status+13;
		return code;
	}
	
	return g_data[0];
}

uint8_t imu_whoami_xm()
{
	_zero_buffer();
	
	status_code_t status = XM_READ(WHO_AM_I_XM, 1);
	if (status != STATUS_OK) {
		uint8_t code = status+13;
		return code;
	}
	
	return g_data[0];
}

bool imu_read_accel(struct accelerometer *accel)
{
	_zero_buffer();
	status_code_t status = XM_READ(OUT_X_L_A, 6);
	
	int16_t x = ((g_data[1] << 8) | g_data[0]);
	int16_t y = ((g_data[3] << 8) | g_data[2]);
	int16_t z = ((g_data[5] << 8) | g_data[4]);
	
	accel->x = x * g_accel_res; // Store x-axis values
	accel->y = y * g_accel_res; // Store y-axis values
	accel->z = z * g_accel_res; // Store z-axis values
	
	_zero_buffer();
	return (status == STATUS_OK);
}

bool imu_read_mag(struct magnetometer *mag)
{
	_zero_buffer();
	status_code_t status =XM_READ(OUT_X_L_M, 6);
	
	int16_t x = ((g_data[1] << 8) | g_data[0]);
	int16_t y = ((g_data[3] << 8) | g_data[2]);
	int16_t z = ((g_data[5] << 8) | g_data[4]);
	
	mag->x = x * g_mag_res; // Store x-axis values
	mag->y = y * g_mag_res; // Store y-axis values
	mag->z = z * g_mag_res; // Store z-axis values
	
	_zero_buffer();
	return (status == STATUS_OK);
}

bool imu_read_gyro(struct gyroscope *gyro)
{
	_zero_buffer();
	status_code_t status = GYRO_READ(OUT_X_L_G, 6);
	
	int16_t x = ((g_data[1] << 8) | g_data[0]);
	int16_t y = ((g_data[3] << 8) | g_data[2]);
	int16_t z = ((g_data[5] << 8) | g_data[4]);
	
	gyro->x = x * g_gyro_res; // Store x-axis values
	gyro->y = y * g_gyro_res; // Store y-axis values
	gyro->z = z * g_gyro_res; // Store z-axis values
	
	_zero_buffer();
	return (status == STATUS_OK);
}