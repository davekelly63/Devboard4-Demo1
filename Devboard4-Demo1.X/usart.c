#include <htc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "usart.h"

/* Send a single character to the PC */
void putch (unsigned char byte)
{

   /* output one byte */

   // Wait for last byte to send before sending next byte
   while (!TXIF)
   {  /* set when register is empty */
      continue;
   }

   TXREG = byte;
}

/* Get a single character from the PC */
unsigned char getch ()
{
   // Use timer 1 to make sure not locking up
   
   TMR1 = 0;
   TMR1IF = 0;
   TMR1ON = 1;
   
   bool timedout = false;
   
   /* retrieve one byte */
   while (RCIF == 0)
   {  /* set when register is not empty */
      
      if (TMR1IF == 1)
      {
         timedout = true;
         break;
      }
   }

   uint8_t returnCode = (timedout == true) ? 0 : RCREG;
   
   return returnCode;
}

/* Get a single character for the RS232 port, and echos it back */
unsigned char getche (void)
{
   unsigned char  c;

   putch (c = getch ());

   return c;
}

/* */
void getstr (uint8_t *s)
{
   uint8_t  c;

   while (RCIF == 0)
   {
      continue;
   }

   while (c = getch ())
   {
      putch (c);
      *s = c;
      if (c == '\r')
      {
         *s = 0;
         break;
      }

      s++;
      
      if (TMR1IF == 1)
      {
         break;
      }
   }

   // Terminate the string
   *s = 0;
}
