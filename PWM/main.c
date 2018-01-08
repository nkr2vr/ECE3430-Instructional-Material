#include "msp.h"
#include "LED.h"
#include "pushbutton.h"
#include "timerA.h"

#define _1MHz_Offset    (signed int)(-274)      //Use with DCORSEL_0
#define _2MHz_Offset    (signed int)(-274)      //Use with DCORSEL_1
#define _4MHz_Offset    (signed int)(-274)       //Use with DCORSEL_2
#define _8MHz_Offset    (signed int)(-274)       //Use with DCORSEL_3
#define _12MHz_Offset   (signed int)(0)
#define _16MHz_Offset   (signed int)(-274)       //Use with DCORSEL_4
#define _24MHz_Offset   (signed int)(0)
#define _32MHz_Offset   (signed int)(-274)       //Use with DCORSEL_5
#define _48MHz_Offset   (signed int)(0)
#define _64MHz_Offset   (signed int)(137)


// Global variable
signed char TA0CCR1_delta = 1;

// Function prototypes
void ConfigureClockModule(void);

void main(void)
{
	// Stop the watchdog timer, and configure the clock module.
	WDTCTL = WDTPW + WDTHOLD;
	ConfigureClockModule();

    // Initialize port pins and hardware.
    InitializeLEDPortPins();
    InitializePushButtonPortPin();
	ConfigureTimerA();

	__enable_interrupts();

	// Infinite loop
	for(;;);

}

void ConfigureClockModule(void)
{
    //Stop Watchdog Timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    CS->KEY = CS_KEY_VAL;
    //Reset parameters
    CS->CTL0 = 0;
    CS->CTL1 = 0;
    CS->CTL2 = 0;
    CS->CTL3 = 0;

    /**
     * Configure DCO for ~1MHz
     * CS_CTL0_DCOEN         Set high to enable DCO
     * CS_CTL0_DCORSEL_0                        < Nominal DCO Frequency Range (MHz): 1 to 2
     * CS_CTL0_DCORSEL_1                        < Nominal DCO Frequency Range (MHz): 2 to 4
     * CS_CTL0_DCORSEL_2                        < Nominal DCO Frequency Range (MHz): 4 to 8
     * CS_CTL0_DCORSEL_3                        < Nominal DCO Frequency Range (MHz): 8 to 16
     * CS_CTL0_DCORSEL_4                        < Nominal DCO Frequency Range (MHz): 16 to 32
     * CS_CTL0_DCORSEL_5                        < Nominal DCO Frequency Range (MHz): 32 to 64
     * CS_CTL0_DCOTUNE_MASK                     10-bit, 2's complement frequency offset
     * See formula in section 5.2.8.3 for DCOTUNE value
     */
#if PWMTYPE == SOFTWARE
    CS->CTL0 = CS_CTL0_DCOEN | CS_CTL0_DCORSEL_0 | (CS_CTL0_DCOTUNE_MASK & _1MHz_Offset);
    //Set DCO as source for SMCLK
    CS->CTL1 = CS_CTL1_SELS__DCOCLK | CS_CTL1_DIVS__64 | CS_CTL1_SELM__DCOCLK | CS_CTL1_DIVM__1;
#else
    CS->CTL0 = CS_CTL0_DCOEN | CS_CTL0_DCORSEL_0 | (CS_CTL0_DCOTUNE_MASK & _1MHz_Offset);
    //Set DCO as source for SMCLK
    CS->CTL1 = CS_CTL1_SELS__DCOCLK | CS_CTL1_DIVS__1;
#endif

    //Enable SMCLK
    CS->CLKEN = CS_CLKEN_SMCLK_EN;

    //Lock to prevent further changes.
    CS->KEY = 0;
}

