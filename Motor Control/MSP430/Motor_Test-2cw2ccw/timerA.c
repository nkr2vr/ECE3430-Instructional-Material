#include "timerA.h"
#include "LED.h"

extern unsigned int g1mSTimeout; //queue of ms's to be added

void ConfigureTimerA(void)
{
	TACTL = MC_0 | TACLR;       //set timer to stop mode
	PWM_PERIOD  = 2000;         //2000 cycles * (16 Mhz/8) = 1 ms
	PWM_DUTY = 0;             //Starting point
	
	//Unused
	//TACCR1 = 0xA40; //initial value for part of the duty cycle
	//TACCR2 = 0x8000; // 65636 cycles w/ /8 modifier.
	
	TACTL = (TASSEL_2 | ID_3 | MC_1);   //SMCLK | IDx (/8) | MCx (Up mode)(01) | interupt enabled
	TACCTL0 = CCIE;                     //Enable interrupt
	TACCTL1 = OUTMOD_7;          //Enable interrupt, Output mode 7: reset/set
	P1SEL |= BIT2;
}

#pragma vector = TIMER0_A0_VECTOR
// Timer A interrupt service routine (every 1ms)
__interrupt void TimerA0_routine(void)
{
    TACCTL0 &= ~CCIFG; //reset
	g1mSTimeout += 1;
}

// #pragma vector = TIMER0_A1_VECTOR
// // Timer A interrupt service routine (every 1ms)
// __interrupt void TimerA1_routine(void)
// {
//     TACCTL1 &= ~CCIFG; //reset
// }
