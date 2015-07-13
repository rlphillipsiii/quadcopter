/*
 * main.c
 *
 * Created: 7/6/2015 11:52:06 PM
 * Author: Robert Phillips III
 */ 

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <util/delay.h>

#include "lib/throttle_driver.h"
#include "lib/twi_master_driver.h"

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
	board_init();

	enable_throttle();
	set_throttle(A, THROTTLE_MIN);

	twi_init();
	
	irq_initialize_vectors();
	sei();

	uint8_t data[TWI_DATA_LENGTH];
	
	//twi_read()
	while (1) {
		
	}
}
