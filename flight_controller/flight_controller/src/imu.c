/*
 * imu.c
 *
 * Created: 7/12/2015 7:58:07 PM
 *  Author: Robert Phillips III
 */ 

#include "lib/twi_master_driver.h"

#define XM_WRITE(length)   twi_write(XM, &g_data[0], length); _zero_buffer();
#define GYRO_WRITE(length) twi_write(GYRO, &g_data[0], length); _zero_buffer();

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
	g_data[0] = CTRL_REG0_XM;
	g_data[1] = 0x00;
	XM_WRITE(2);
	
	// 100Hz data rate, x/y/z all enabled
	g_data[0] = CTRL_REG1_XM;
	g_data[1] = 0x57;
	XM_WRITE(2); 
					
	// Set scale to 2g
	g_data[0] = CTRL_REG2_XM;
	g_data[1] = 0x00;
	XM_WRITE(2); 
	
	// Accelerometer data ready on INT1_XM (0x04)
	g_data[0] = CTRL_REG3_XM;
	g_data[1] = 0x04;
	XM_WRITE(2);
}

void _gyro_init()
{
	// Normal mode, enable all axes
	g_data[0] = CTRL_REG1_G;
	g_data[1] = 0x0F;
	GYRO_WRITE(2);
	
	// Normal mode, high cutoff frequency
	g_data[0] = CTRL_REG2_G;
	g_data[1] = 0x00;
	GYRO_WRITE(2);

	// Int1 enabled (pp, active low), data read on DRDY_G:
	g_data[0] = CTRL_REG3_G;
	g_data[1] = 0x88;
	GYRO_WRITE(2);
	
	// Set scale to 245 dps
	g_data[0] = CTRL_REG4_G;
	g_data[1] = 0x00;
	GYRO_WRITE(2);
	
	g_data[0] = CTRL_REG5_G;
	g_data[1] = 0x00;
	GYRO_WRITE(2);
}

void _mag_init()
{
	// Mag data rate - 100 Hz, enable temperature sensor
	g_data[0] = CTRL_REG5_XM;
	g_data[1] = 0x94;
	XM_WRITE(2);
	
	// Mag scale to +/- 2GS		
	g_data[0] = CTRL_REG6_XM;
	g_data[1] = 0x00;
	XM_WRITE(2);

	// Continuous conversion mode
	g_data[0] = CTRL_REG7_XM;
	g_data[1] = 0x00;
	XM_WRITE(2);

	// Magnetometer data ready on INT2_XM (0x08)
	g_data[0] = CTRL_REG4_XM;
	g_data[1] = 0x04;
	XM_WRITE(2);

	// Enable interrupts for mag, active-low, push-pull
	g_data[0] = INT_CTRL_REG_M;
	g_data[1] = 0x09;
	XM_WRITE(2);
}

void imu_init()
{
	_setup_gyro_resolution();
	_setup_accel_resolution();
	_setup_mag_resolution();
	
	twi_init();
	
	_accel_init();
	_gyro_init();
	_mag_init();
}

void read_accel()
{
	uint8_t temp[6]; // We'll read six bytes from the accelerometer into temp
	xmReadBytes(OUT_X_L_A, temp, 6); // Read 6 bytes, beginning at OUT_X_L_A
	ax = (temp[1] << 8) | temp[0]; // Store x-axis values into ax
	ay = (temp[3] << 8) | temp[2]; // Store y-axis values into ay
	az = (temp[5] << 8) | temp[4]; // Store z-axis values into az
}

void read_mag()
{
	uint8_t temp[6]; // We'll read six bytes from the mag into temp
	xmReadBytes(OUT_X_L_M, temp, 6); // Read 6 bytes, beginning at OUT_X_L_M
	mx = (temp[1] << 8) | temp[0]; // Store x-axis values into mx
	my = (temp[3] << 8) | temp[2]; // Store y-axis values into my
	mz = (temp[5] << 8) | temp[4]; // Store z-axis values into mz
}

void read_gyro()
{
	uint8_t temp[6]; // We'll read six bytes from the gyro into temp
	gReadBytes(OUT_X_L_G, temp, 6); // Read 6 bytes, beginning at OUT_X_L_G
	gx = (temp[1] << 8) | temp[0]; // Store x-axis values into gx
	gy = (temp[3] << 8) | temp[2]; // Store y-axis values into gy
	gz = (temp[5] << 8) | temp[4]; // Store z-axis values into gz
}