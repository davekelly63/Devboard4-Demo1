/* Process RS232 commands */
#include <xc.h>
#include <stdio.h>
#include <stdint.h>

#include "main.h"
#include "lcd.h"
#include "usart.h"

/* */

void CommandProcessor (uint8_t ch)
{
   uint8_t  cmd = 0;
   uint8_t  relayState = 0;      // Digital I/O state
   uint8_t  channelNumber = 0;   // Digital I/O channel
   switch (ch)
   {
      case DIGITAL_IO:

         // Digital I/O
         // Get the next character which indicates the channel and the state
         // Disable the serial interrupt, else the chaaracter will be consumed by the interrupt
         RCIE = 0;
         cmd = getch ();
         RCIE = 1;

         // MSB is the state, rest is the relay number. Starting number is 16
         relayState = cmd >> 7;
         channelNumber = cmd & 0x7F;

         // 0 -> 8 are switch inputs
         // 0x10 -> 0x18 are relays
         // 0x20 -> 0x28 are LED
         // Just handling a relay for now
         if (channelNumber >= 0x10)
         {
            switch (channelNumber)
            {
               case 16:
               case 32:
                  RB0 = relayState;
                  break;

               case 17:
               case 33:
                  RB1 = relayState;
                  break;

               default:

                  // Invalid relay
                  break;
            }
            
            // printf ("Ch %u\n", channelNumber);
         }
         
         break;

      case LCD_MESSAGE:

         // Put thee following message on the LCD
         RCIE = 0;
         uint8_t lineNumber = getch ();
         printf ("Line %u\n", lineNumber);
         uint8_t  message[50];
         getstr (message);
         printf ("Message: %s", message);
         lcd_gotoxy (0, lineNumber);
         lcd_puts ("                ");
         lcd_gotoxy (0, lineNumber);
         //lcd_clear ();
         lcd_puts (message);
         RCIE = 1;
         break;

      default:
         break;
   }
}
