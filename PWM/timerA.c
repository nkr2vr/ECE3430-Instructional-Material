#include "timerA.h"

extern signed char TA0CCR1_delta;

void ConfigureTimerA(void)
{
    // Timer0_A3 Control Register
    TA0CTL = MC_0;     // Stop the timer
    TA0CTL |= TACLR;    // Clear the timer

    // PWM_PERIOD = TACCR0 + 1 => TACCR0 = PWM_PERIOD - 1 (see PWM_macros.h)
    TA0CCR0 = PWM_PERIOD-1;
    TA0CCR1 = 1;

#if PWMTYPE == HARDWARE
    //Hardware PWM output, before interrupts are enabled
    UNLOCK_PMAP_REG;                            //Input port mapping key
    MAP_LED_TO_TACCR1;                          //Select source
    P2SEL1 &= ~LED1_BIT; P2SEL0 |= LED1_BIT;    //Select PMAP for pin P2.0
    LOCK_PMAP_REG;                              //Lock port mapping register
#endif

//    These instructions may take some time, so we'll do these before starting the clock
    TA0CCTL0 |= CCIE; 	// Enable TACCR0 Compare/Capture Interrupt Flag (CCIFG0)
    NVIC_SetPriority(TA0_0_IRQn, 0);   //Set interrupt priority
    NVIC_EnableIRQ(TA0_0_IRQn); //Enable interrupt in NVIC

    TA0CTL |= TAIE;     // Enable Timer A Interrupt Flag (TAIFG)

#if PWMTYPE == SOFTWARE
    TA0CCTL1 = CCIE;   // Enable TACCR1 Compare/Capture Interrupt Flag (CCIFG1)
#else
    TA0CCTL1 = CCIE + OUTMOD_3;   // Enable TACCR1 Compare/Capture Interrupt Flag (CCIFG1) and Output Mode 3 for CCIFG1
    //Outmode 3: set OUT bit when CCR1, reset at TAIFG
#endif

    NVIC_SetPriority(TA0_N_IRQn, 1);   //Set interrupt priority
    NVIC_EnableIRQ(TA0_N_IRQn); //Enable interrupt in NVIC

    //Now we can start the timer
    TA0CTL |= (TASSEL_2 | ID_0 | MC_1);
    TA0EX0 = TAIDEX__1; //Divide further by 8
    /* TASSEL1 = 0x0200 Timer A clock source select 0
     * TASSEL0 = 0x0100 Timer A clock source select 1
     *  00 = TACLK
     *  01 = ACLK
     *  10 = SMCLK
     *  11 = INCLK
     * ID1 = 0x0080 Timer A clock input divider 1
     * ID0 = 0x0040 Timer A clock input divider 0
     *  00 => Input divider = 1
     *  01 => Input divider = 2
     *  10 => Input divider = 4
     *  11 => Input divider = 8
     * MC1 = 0x0020 Timer A mode control 1
     * MC0 = 0x0010 Timer A mode control 0
     *  00 = Stop
     *  01 = Up
     *  10 = Continuous
     *  11 = Up/Down
     * TACLR = 0x0004 Timer A counter clear
     * TAIE = 0x0002 Timer A counter interrupt enable
     * TAIFG = 0x0001 Timer A counter interrupt flag
     */
}

// Interrupt service routine for CCIFG0
void TA0_0_IRQHandler(void)
{
    CLEAR_CCIFG0;

    //Smaller code, does the same thing
//    if ((TA0CCR1 == 0) || (TA0CCR1 == TA0CCR0)) //Upon reaching an outer limit
//        TA0CCR1_delta = -TA0CCR1_delta;         //Negate delta i.e. change negative to positive, vice-versa
//    TA0CCR1 += TA0CCR1_delta;

    static enum
    {
        up, down
    } direction = up;

    if (direction == up)
    {		// Increasing duty cycle?
        TA0CCR1 += TA0CCR1_delta;	// Yes: increase TACCR1
        if (TA0CCR1 >= TA0CCR0)
        {	// Hit 100% limit?
            direction = down;	// Yes: decrease duty cycle next
            // Toggle LED2 to indicate change in direction.
//            TOGGLE_LED2;
//            TOGGLE_LED2;
        }
    }
    else //direction == down
    {
        TA0CCR1 -= TA0CCR1_delta;		// Decrease duty cycle
        if (TA0CCR1 == 0)
        {			// Hit 0 limit?
            direction = up;			// Yes: increase duty cycle next
            // Toggle LED2 to indicate change in direction.
//            TOGGLE_LED2;
//            TOGGLE_LED2;
        }
    }
}

// Interrupt service routine for CCIFG1 and TAIFG
void TA0_N_IRQHandler(void)
{
    //Check which interrupts are active in the vector
    switch (TA0IV)
    {
    case TA0IV_NONE:
        break;

    case TA0IFG: // TAIFG interrupt
        CLEAR_TAIFG;
#if PWMTYPE == SOFTWARE
        if (TA0CCR1 > 0)
        {
            TURN_ON_LED1;
        }
#endif
        break;

    case CCIFG1: // CCIFG1 interrupt
        CLEAR_CCIFG1;
#if PWMTYPE == SOFTWARE
        if (TA0CCR1 < TA0CCR0){
            TURN_OFF_LED1;
        }
#endif
        break;

    default:
        for (;;)
            ; // Should not be possible
    }
}
