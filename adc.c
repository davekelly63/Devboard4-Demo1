/*
 * Read the ADC on a 16C71.
 */


#include	<htc.h>
#include <stdint.h>
#include	"adc.h"

#define _XTAL_FREQ 4000000           // For the delays library


uint8_t adc_read (uint8_t channel)
{

   ADCON0 = (channel << 2) + 0xC1; // enable ADC, RC osc.

   GO_DONE = 1;
   
   while (GO_DONE)
      continue; // wait for conversion complete

   return ADRESH;
}



/* Sample code to set up the A2D module */
void init_adc (void)
{
   ADCON0 = 0; // select Fosc/2
   ADCON1 = 0; // select left justify result. A/D port configuration 0
   ADON = 1; // turn on the A2D conversion module
}