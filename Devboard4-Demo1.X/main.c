/*
 * A simple demonstration of serial communications for EQ PIC Development Board 
 * which incorporates the on-board hardware USART of the Microchip
 * PIC16Fxxx series of devices.
 *
 * DKSS July 2012
 *
 * Outputs info to the serial program, then echos anything sent to it
 *
 * Baud rate 57600
 *
 */
#include "main.h"
#include "usart.h"
#include "adc.h"
#include "lcd.h"

/// Command received from serial port
void Serial_Int ()
{
   command = getch ();
   commandReceived = true;
   RCIF = 0;
}

/// Timer 0 overflows every 65.536 ms
void Timer0_Int ()
{
   reading = adc_read (0);
   T0IF = 0;
}

/* */
void interrupt interruptHandler (void)
{
   if ((T0IE == 1) && (T0IF == 1))
   {

      // Timer 0 interrupt
      Timer0_Int ();
   }

   if ((RCIE == 1) && (RCIF == 1))
   {

      // Serial port interrupt
      Serial_Int ();
   }
}

/* A simple demonstration of serial communications which
 * incorporates the on-board hardware USART of the Microchip
 * PIC16Fxxx series of devices. */
void main (void)
{
   uint8_t  vr1 = 0;

   ANSEL = 0x03;           // RA0, RA1 are analogue inputs
   ANSELH = 0x00;          // All other ports are digital I/O
   TRISA = 0b01000011;    // Set RA0, RA1 RA6 as inputs
   TRISB = 0x00;
   PORTB = 0xFF;

   INTCONbits.GIE = 1;     // disable the interrupts.
   init_comms ();          // set up the USART - settings defined in usart.h
   init_adc ();

   lcd_init ();
   lcd_goto (0);           // select first line
   lcd_puts ("EQ Dev Board");
   lcd_gotoxy (0, 1);      // Select second line
   lcd_puts ("(c) 2017");

   OPTION_REG = 0b0111;   // prescale by 256
   T0CS = 0;               // select internal clock
   T0IE = 1;               // enable timer interrupt
   INTCONbits.PEIE = 1;
   RCIE = 1;               // Enabled UART interrupt
   __delay_ms (100);

   printf ("EQ Dev Board (c) 2017\r");
   __delay_ms (150);
   
   while (true)
   {
      if (commandReceived == 1)
      {
         CommandProcessor (command);
         commandReceived = 0;
      }

      if (reading != vr1)
      {
         vr1 = reading;
         printf ("%c%c", 0x92, vr1);
      }
   }
}
