/*
 * main.c
 *
 * Created: 7/6/2015 11:52:06 PM
 * Author: Robert Phillips III
 */ 

/* 
 * F_CPU is defined by a compile time flag instead of here
 * #define F_CPU 2000000
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <util/delay.h>

#include <stdio.h>

#include "general.h"
#include "lcd_driver.h"
#include "throttle_driver.h"
#include "imu.h"
#include "filter.h"
#include "timer.h"
#include "pid.h"

struct accelerometer accel;
struct gyroscope gyro;
struct magnetometer mag;

struct flight data;

float pid_dt;
float filt_dt;
float throt_dt;

float dt;

bool poll_button(void)
{
	static uint16_t state = 0;
	state = (state << 1) | (!(PORTF.IN & PIN1_bm)) | 0xE000;
	
	if (state == 0xF000) {
		return true;
	}
	
	return false;
}

void motor_test(void)
{
	static bool setting = true;
	
	if (poll_button()) {
		if (setting) {
			throttle_set(100);
		} else {
			throttle_set(THROTTLE_MIN);
		}
		
		setting ^= 0x01;
	}
	
	_delay_ms(20);
}

int main(void)
{
	// enable the 32MHz oscillator and wait for it to be ready
	OSC.CTRL |= OSC_ID_RC32MHZ;
	while (!ISSET(OSC.STATUS, OSC_RC32MRDY_bp));
	
	// change the system clock to 32MHz
	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
	
	// prescale the system clock by 2 (16MHz clock)
	//CCP = CCP_IOREG_gc;
	//CLK.PSCTRL = CLK_PSADIV_2_gc; 
	
	// disable the 2MHz oscillator
	OSC.CTRL &= ~(OSC_ID_RC2MHZ);
	
	// auto generated asf board init
	board_init();

	timer_init();	
	throttle_init();

	irq_initialize_vectors();
	sei();
	
	//uint16_t ids = imu_init();
		
	data.pitch = data.roll = data.yaw = 0;
	gyro.pitch = gyro.roll = gyro.yaw = 0;
	accel.pitch = accel.roll = 0;
		
	pid_init(&data);
		
	while (1) {
		//imu_read_mag(&mag);
		
		timer_reset();
		complimentary(&data, &gyro, &accel, &mag, 5.0e-3);
		filt_dt = timer_clock();
		
		timer_reset();
		pid_loop(&data, 5.0e-3);
		pid_dt = timer_clock();
		
		timer_reset();
		throttle_adjust(&data);
		throt_dt = timer_clock();
		
		dt = filt_dt + pid_dt + throt_dt;
		
		timer_reset();
	}
}
