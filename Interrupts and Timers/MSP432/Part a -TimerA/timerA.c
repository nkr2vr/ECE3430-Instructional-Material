#include "timerA.h"

extern char LEDBit;

void ConfigureTimerA(void){
    //Stop and clear timer
    TA0CTL = TIMER_A_CTL_MC__STOP | TIMER_A_CTL_CLR;

    /**
     * TIMER_A_CTL_SSEL__SMCLK  Select SMCLK as source for TIMER_A0
     * TIMER_A_CTL_ID__8        Use internal division by 8
     * TIMER_A_CTL_MC__UP       Set Timer_A0 to count-up mode
     */
    TA0CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID__8 | TIMER_A_CTL_MC__UP;

    // Divides clock further, div by 1
    TA0EX0 = 0;
    //As suggested in the manual, clear the timer again
    TA0CTL &= ~TIMER_A_CTL_CLR;

    //Enable TimerA CCR0 Interrupt
    TA0CCTL0 = TIMER_A_CCTLN_CCIE;
    //Enable TA0_0 interrupt
    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);

    /**
     * Note: despite the 432 being a 32-bit processor, TimerA is 16 bits.
     * Set the CCR0 value to f_SMCLK / (f_desired * Internal_division)
     * Round down by 1 to compensate for assembly instructions
     */
    TA0CCR0 = TA0CCR0_VALUE;
}

// Timer A0 interrupt service routine
void TA0_0_IRQHandler(void){

    //Clear interrupt flag
    TA0CCTL0 &= ~CCIFG;

	// Toggle active LED.
	if (LEDBit == LED1) {
		TOGGLE_LED1;
	}
	else {
		TOGGLE_LED2;
	}
}

//Generic ARM function, implemented for comparison
void SysTick_Handler(void){

    // Toggle active LED.
    if (LEDBit == LED1) {
        TOGGLE_LED1;
    }
    else {
        TOGGLE_LED2;
    }

}
