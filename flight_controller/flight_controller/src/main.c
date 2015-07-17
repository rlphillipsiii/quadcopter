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

int main(void)
{
	// auto generated asf board init
	board_init();

	timer_init();
	/*
	throttle_init();
	throttle_set(A, THROTTLE_MIN);
	*/
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
		
	lcd_clear();
		
	struct flight data;
	data.pitch = data.roll = data.yaw = 0;
	gyro.pitch = gyro.roll = gyro.yaw = 0;
	
	timer_reset();
	while (1) {
		imu_read_accel(&accel);
		imu_read_gyro(&gyro);
		//imu_read_mag(&mag);
		
		float dt = timer_clock();
		
		complimentary(&data, &gyro, &accel, &mag, dt);
			
		sprintf(string, "%f\n%f", data.roll, data.pitch);			
		//sprintf(string, "%f\n%f\n%f", gyro.x, gyro.y, dt);
		lcd_write(&string[0]);
	}
}
