#ifndef TIMER_A_H
#define TIMER_A_H

#include <msp430.h>
#include "spi.h"
#include "DAC.h"
#include "debug.h"

//#define TACCR0_VALUE 	25000	// 12 MHz MCLK divided by Fs=480 Hz (8 sample points * 60 Hz)
#define TACCR0_VALUE 	3125	// 12 MHz MCLK divided by Fs=3840 Hz (64 sample points * 60 Hz)

// Use these defines to produce filtered input at output
//#define NUMBER_OF_BIT_SHIFTS 	3 	// Equals floor(log2(NUMBER_OF_SAMPLES))
//#define NUMBER_OF_SAMPLES 		8 	// Equals (1 << NUMBER_OF_BIT_SHIFTS) if power of 2
#define NUMBER_OF_BIT_SHIFTS 	6 	// Equals floor(log2(NUMBER_OF_SAMPLES))
#define NUMBER_OF_SAMPLES 	64 	// Equals (1 << NUMBER_OF_BIT_SHIFTS) if power of 2

// Use these defines to produce unfiltered input at output
//#define NUMBER_OF_BIT_SHIFTS 	0 	// Equals floor(log2(NUMBER_OF_SAMPLES))
//#define NUMBER_OF_SAMPLES 	1 	// Equals (1 << NUMBER_OF_BIT_SHIFTS) if power of 2

#define SCALING_FACTOR 6 // Scale 10-bit ADC values for 16-bit DAC.

// Function Prototypes
void ConfigureTimerA(void);
void filter(void);

#endif
