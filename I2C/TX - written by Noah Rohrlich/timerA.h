#ifndef TIMER_A_H
#define TIMER_A_H

#include <msp430.h>

#define TIMER_VALUE 1000 //Approx 1ms interrupt

// Prototypes
void TimeDelay(unsigned int delay, unsigned int resolution);

#endif
