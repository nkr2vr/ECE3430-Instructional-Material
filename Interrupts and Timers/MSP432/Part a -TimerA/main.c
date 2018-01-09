#include "msp.h"
#include "LED.h"
#include "pushbutton.h"
#include "timerA.h"

// Global variable
char LEDBit = LED1;

// Function prototypes
void ConfigureClockSystem(void);

void main(void)
{
	// Stop the watchdog timer, and configure the clock System.
	WDTCTL = WDTPW + WDTHOLD;
	ConfigureClockSystem();

    // Initialize port pins associated with the LEDs, and then turn off LEDs.
    InitializeLEDPortPins();

    // Initialize the port pin associated with the pushbutton
    InitializePushButtonPortPin();

    // Configure timer A to generate the required interrupt.
	ConfigureTimerA();
//    SysTick_Config(SYSTICK_VALUE); //Generic ARM function to configure SysTick, enables interrupt too

	__enable_interrupts();

	// Infinite loop
	while (1) {
	}
}

/**
 * ConfigureClockSystem
 *
 * Initializes the Digitally Controlled Oscillator for a speed of approx
 * 1MHz.
 */
void ConfigureClockSystem(void){

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
     * CS_CTL0_DCORSEL_0     nominal 1.5MHz, range 1 to 2Mhz
     * CS_CTL0_DCOTUNE_MASK  10-bit, 2's complement frequency offset
     * See formula in section 5.2.8.3 for DCOTUNE value
     */
    int N = 0xFEEE;
    CS->CTL0 = CS_CTL0_DCOEN | CS_CTL0_DCORSEL_0 | (CS_CTL0_DCOTUNE_MASK & N);

    //Set DCO as source for SMCLK
    CS->CTL1 = CS_CTL1_SELS__DCOCLK;

    //Enable SMCLK
    CS->CLKEN = CS_CLKEN_SMCLK_EN;

    //Lock to prevent further changes.
    CS->KEY = 0;
}
