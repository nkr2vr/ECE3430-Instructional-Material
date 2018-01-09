#ifndef TIMER_A_H
#define TIMER_A_H

#include <msp430.h>

#define TIMER_PERIOD    2000
#define PWM_PERIOD      TACCR0
#define PWM_DUTY        TACCR1
#define PWM_DELTA       10
#define PWM_PRESET      132
#define PWM_FULL        TIMER_PERIOD
#define PWM_HALF        (TIMER_PERIOD >> 1)

//For use in cycling pwm, if needed
typedef enum {up, down} pwmMode;

#define TABLE_LENGTH    16
//Lookup table for increments of PWM value
//16 values because TACCR1 is 16 bits
//Static const stores on flash instead of RAM
static const int timerIncrements[16] = {
    (PWM_HALF),
    (PWM_HALF >> 1),
    (PWM_HALF >> 2),
    (PWM_HALF >> 3),
    (PWM_HALF >> 4),
    (PWM_HALF >> 5),
    (PWM_HALF >> 6),
    (PWM_HALF >> 7),
    (PWM_HALF >> 8),
    (PWM_HALF >> 9),
    (PWM_HALF >> 10),
    (PWM_HALF >> 11),
    (PWM_HALF >> 12),
    (PWM_HALF >> 13),
    (PWM_HALF >> 14),
    (PWM_HALF >> 15)
};

// Prototypes
void ConfigureTimerA(void);

#endif
