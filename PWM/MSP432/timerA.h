#ifndef TIMER_A_H
#define TIMER_A_H

#include "msp.h"
#include "LED.h"

#define PWMTYPE SOFTWARE
#define SOFTWARE    1
#define HARDWARE    0

#if PWMTYPE == SOFTWARE
#define PWM_PERIOD      100 // Equals (SMCLK_FREQUENCY*DUTY_CYCLE_PERIOD)/TIMERA_CLOCK_DIVIDE
#else
#define PWM_PERIOD      156
#endif

#define CCIFG1          0x02    //TAIV code for CCIFG
#define TA0IFG          0x0E    //TAIV code for TAIFG
#define TA0IV_NONE      0
#define CLEAR_TAIFG     (TA0CTL &= ~TAIFG)
#define CLEAR_CCIFG0    (TA0CCTL0 &= ~CCIFG)
#define CLEAR_CCIFG1    (TA0CCTL1 &= ~CCIFG)

//Hardware PWM Port Mapping to send output to LED
#define UNLOCK_PMAP_REG             PMAPKEYID = PMAPKEY
#define LOCK_PMAP_REG               PMAPKEYID = 0
#define MAP_LED_TO_TACCR1           P2MAP01 = (PMAP_TA0CCR1A) //To P2.0
#define REMOVE_MAPPING              P2MAP01 = PMAP_NONE;

// Prototypes
void ConfigureTimerA(void);

#endif
