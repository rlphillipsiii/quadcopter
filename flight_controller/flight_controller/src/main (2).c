/*
 * main.c
 *
 * Created: 7/6/2015 11:52:06 PM
 * Author: Robert Phillips III
 */ 

#define F_CPU 2000000

#include <avr/io.h>
#include <util/delay.h>

#include "general.h"
#include "throttle_driver.h"
#include "twi_master_driver.h"

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
#if 0
	enable_throttle();
	
	set_throttle(A, THROTTLE_MIN);
	
	bool setting = true;
	
    while (1) {
		if (poll_button()) {
			if (setting) {
				set_throttle(A, THROTTLE_MAX);
			} else {
				set_throttle(A, THROTTLE_MIN);
			}
			
			setting ^= 0x01;
		}
					
        _delay_ms(20);
    }
#endif

	irq_initialize_vectors();
	sei();
	
	while (1) {
		
	}
}