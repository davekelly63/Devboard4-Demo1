/*
 *	LCD interface example
 *	Uses routines from delay.c
 *	This code will interface to a standard LCD controller
 *	like the Hitachi HD44780. It uses it in 4 bit mode, with
 *	the hardware connected as follows (the standard 14 pin
 *	LCD connector is used):
 *
 *	PORTB bits 0-3 are connected to the LCD data bits 4-7 (high nibble)
 *	PORTB bit 7 is connected to the LCD RS input (register select)
 *	PORTA bit 5 is connected to the LCD EN bit (enable)
 *
 *	To use these routines, set up the port I/O TRISB then
 *	call lcd_init(), then other routines as required.
 *
 *
 */
#ifndef _XTAL_FREQ

// Unless specified elsewhere, 4MHz system frequency is assumed
   #define _XTAL_FREQ   4000000
#endif
#include <htc.h>
#include "lcd.h"

#define LCD_RS       RA2
#define LCD_RW       RA3
#define LCD_EN       RA4

#define LCD_DATA     PORTB

#define LCD_STROBE() ((LCD_EN = 1), (LCD_EN = 0))

/* write a byte to the LCD in 4 bit mode */
unsigned char  portContents;  // Only using 4 bits, so keep the other nibble unaffected

/* */
void lcd_write (unsigned char c)
{
   portContents = LCD_DATA & 0x0F;  // Keep the lower nibble
   __delay_us (40);
   LCD_DATA = ((c) & 0xF0) | portContents;
   LCD_STROBE ();
   LCD_DATA = ((c << 4) & 0xF0) | portContents;
   LCD_STROBE ();
}

/*
 * Clear and home the LCD
 */
void lcd_clear (void)
{
   LCD_RS = 0; // Sending a command
   lcd_write (0x01);
   __delay_ms (2);
}

/* write a string of chars to the LCD */
void lcd_puts (const char *s)
{
   LCD_RS = 1; // write characters
   while (*s)
   {
      lcd_write (*s++);
   }
}

/* write one character to the LCD */
void lcd_putch (char c)
{
   LCD_RS = 1; // write characters
   lcd_write (c);
}

/*
 * Go to the specified position
 */
void lcd_goto (unsigned char pos)
{
   LCD_RS = 0;

   lcd_write (0x80 + pos);
}

/* */
void lcd_gotoxy (unsigned char x, unsigned char y)
{
   unsigned char  address;

   address = x + y * 0x40;

   lcd_goto (address);
}

/* initialise the LCD - put into 4 bit mode */
void lcd_init ()
{
   char  init_value;

   ADCON1 = 0x06;                   // Disable analog pins on PORTA
   init_value = 0x30;

   RA7 = 1;
   LCD_RS = 0;
   LCD_EN = 0;
   LCD_RW = 0;

   portContents = LCD_DATA & 0x0F;  // Only using the upper nibble, so save lower nibble bits
   __delay_ms (15);                 // wait 15mSec after power applied,
   LCD_DATA = init_value | portContents;
   LCD_STROBE ();
   __delay_ms (5);
   LCD_STROBE ();
   __delay_us (200);
   LCD_STROBE ();
   __delay_us (200);
   LCD_DATA = 0x20 | portContents;  // Four bit mode
   LCD_STROBE ();

   lcd_write (0x28);                // Set interface length
   lcd_write (0xC);                 // Display On, Cursor Off, Cursor no blink
   lcd_clear ();                    // Clear screen
   lcd_write (0x6);                 // Set entry Mode
}
