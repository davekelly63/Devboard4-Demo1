/*
 * File:   main.h
 * Author: Dave
 *
 * Created on 23 July 2017, 11:09
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>

// Set the config fuses
__CONFIG (FOSC_INTRC_NOCLKOUT & WDTE_OFF & PWRTE_OFF & BOREN_OFF & LVP_OFF & WRT_OFF & DEBUG_OFF & CPD_OFF & CP_OFF & IESO_OFF & FCMEN_OFF);

#define _XTAL_FREQ 4000000           // For the delays library

enum COMMANDS_T
{
    DIGITAL_IO = 0x90,
    POT_VR2 = 0x91,
    LCD_MESSAGE = 0x71
};


// Function prototypes
void CommandProcessor (uint8_t ch);

// Global variables
uint8_t command = 0;
bool commandReceived = false;
uint8_t reading = 0;

