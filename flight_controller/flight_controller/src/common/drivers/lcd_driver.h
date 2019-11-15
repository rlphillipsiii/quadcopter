/*
 * lcd_driver.h
 *
 * Created: 7/14/2015 9:59:20 AM
 *  Author: Robert Phillips III
 */ 


#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

void lcd_init(void);
void lcd_clear(void);

void lcd_write(char *string);


#endif /* LCD_DRIVER_H_ */