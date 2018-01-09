#ifndef TIMER_A_H
#define TIMER_A_H

#include <msp430.h>

#define PWM_PERIOD      TACCR0
#define PWM_DUTY        TACCR1
#define PWM_DELTA       10
#define PWM_PRESET      100

typedef enum {up, down} pwmMode;

// Prototypes
void ConfigureTimerA(void);

#endif
