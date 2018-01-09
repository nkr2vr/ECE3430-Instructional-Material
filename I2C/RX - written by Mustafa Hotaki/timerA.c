#include "timerA.h"

void ConfigureTimerA(void) {
	TA0CTL = MC_0 | TACLR;                   // Set timer to stop mode
	
	TA0CCR0 = 255;                         // 2000 cycles * (16 Mhz/8) = 1 ms
	TA0CCR1 = 0;                            // Starting point

	TA0CTL = (TASSEL_2 | ID_3 | MC_1);      // SMCLK | IDx (/8) | MCx (Up mode)(01) | interupt enabled
	TA0CCTL1 |= OUTMOD_7;                   // Output mode 7: reset on CCR1/set on CCR0
	P1SEL |= BIT2;                          // Allow the OUTMOD to take over output
	P1DIR |= BIT2;
}
