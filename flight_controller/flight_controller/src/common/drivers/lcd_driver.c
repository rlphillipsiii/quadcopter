/*
 * lcd_driver.c
 *
 * Created: 7/14/2015 9:59:35 AM
 *  Author: Robert Phillips III
 */ 

#include <gfx_mono.h>
#include <gfx_mono_text.h>
#include <sysfont.h>
#include <st7565r.h>
#include <xmega_gpio/xmega_gpio.h>

#include "lcd_driver.h"

void lcd_clear()
{
	gfx_mono_draw_string("                    \n"
						 "                    \n"
						 "                    \n"
						 "                    ", 0, 0, &sysfont);
}

void lcd_write(char *string)
{
	gfx_mono_draw_string(string, 0, 0, &sysfont);
}

void lcd_init()
{
	gfx_mono_init();

	gpio_set_pin_high(NHD_C12832A1Z_BACKLIGHT);
	
	//st7565r_init();
	st7565r_set_contrast(ST7565R_DISPLAY_CONTRAST_MIN);
}