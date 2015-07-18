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
#include "lib/lcd_driver.h"
#include "lib/throttle_driver.h"
#include "imu.h"
#include "filter.h"
#include "timer.h"

bool poll_button()
{
	static uint16_t state = 0;
	state = (state << 1) | (!(PORTF.IN & PIN1_bm)) | 0xE000;
	
	if (state == 0xF000) {
		return true;
	}
	
	return false;
}

void motor_test()
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
	// auto generated asf board init
	board_init();

	timer_init();	
	throttle_init();

	irq_initialize_vectors();
	sei();
	
	uint16_t ids = imu_init();
	
	lcd_init();
	
	char string[100];
	sprintf(string, "ID:   0x%04x", ids);
	lcd_write(&string[0]);
	
	struct accelerometer accel;
	struct gyroscope gyro;
	struct magnetometer mag;
	
	uint8_t i;
	for (i = 0; i < 4; i++) {
		_delay_ms(250);
	}
		
	struct flight data;
	data.pitch = data.roll = data.yaw = 0;
	gyro.pitch = gyro.roll = gyro.yaw = 0;
	
	float imu_dt = 0;
	float pid_dt = 0;
	
	while (!poll_button()) _delay_ms(20);
	
	sprintf(string, "%s\nRunning", string);
	lcd_write(&string[0]);
	
	throttle_set(100);
	
	pid_init(&data);
	
	bool setting = true;
	
	float throttle_counter = 0;
	
	timer_reset();
	while (1) {
		if (throttle_active()) {
			imu_read_accel(&accel);
			imu_read_gyro(&gyro);
			//imu_read_mag(&mag);
		
			float imu_dt = timer_clock();
			complimentary(&data, &gyro, &accel, &mag, imu_dt + pid_dt);
			
			float pid_dt = timer_clock();
			pid_loop(&data, imu_dt + pid_dt);
			
			throttle_counter += imu_dt + pid_dt;
			if (throttle_counter > 0.1) {
				throttle_adjust(&data);
				throttle_counter = 0;
			}			
		} else {
			timer_reset();
		}
	}
}
