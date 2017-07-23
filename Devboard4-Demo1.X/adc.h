/*
 * Analog conversion stuff for 16C71 - see adc.c for more info
 */

/*
 * Read the adc on the specified channel - result is in ADRES
 */

extern unsigned char adc_read(unsigned char channel);
extern void init_adc (void);