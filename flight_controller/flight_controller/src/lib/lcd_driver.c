/*
 * lcd_driver.c
 *
 * Created: 7/14/2015 9:59:35 AM
 *  Author: Robert Phillips III
 */ 

#include <st7565r.h>

void lcd_init()
{
	st7565r_init();
	
	// set addresses at beginning of display
	st7565r_set_page_address(0);
	st7565r_set_column_address(0);
	
	uint8_t column_address;
	for (column_address = 0; column_address < 128; column_address++) {
		st7565r_set_column_address(column_address);
		/* fill every other pixel in the display. This will produce
		horizontal lines on the display. */
		st7565r_write_data(0xAA);
	}
}