#ifndef TIMER_A_H
#define TIMER_A_H

#include "msp.h"
#include "LED.h"

// Prototypes
void ConfigureTimerA(void);

#define TA0CCR0_VALUE (31250) // Period = CCR0 + 1
#define SYSTICK_VALUE (7999)

#endif
