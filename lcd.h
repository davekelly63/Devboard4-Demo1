/*
 *	LCD interface header file
 *	See lcd.c for more info
 */

#ifndef _LCD_H
#define _LCD_H

/* write a byte to the LCD in 4 bit mode */

extern void lcd_write(unsigned char);

extern void lcd_send_nibble(unsigned char nibble);

/* Clear and home the LCD */

extern void lcd_clear(void);

/* write a string of characters to the LCD */

extern void lcd_puts(const char * s);

/* Go to the specified position */

extern void lcd_goto(unsigned char pos);

extern void lcd_gotoxy (unsigned char x, unsigned char y);

/* intialize the LCD - call before anything else */

extern void lcd_init(void);

extern void lcd_putch(char);

/*	Set the cursor position */

#define	lcd_cursor(x)	lcd_write(((x)&0x7F)|0x80)


#endif