#include "msp.h"
#include "LED.h"
#include "timerA.h"

// Function prototypes
void ConfigureClockSystem(void);

void main(void)
{
	unsigned int delay = 250; // Represents time delay in unit time increments
	unsigned int resolution = 100; // Represents period of unit time increment
	
	// Stop the watchdog timer, and configure the clock module.
	WDTCTL = WDTPW + WDTHOLD;
	ConfigureClockSystem();

    InitializeLEDPortPins();

	// Infinite loop
  	while (1)
  	{
  		// Toggle the LED states to the complementary state (that is, if the LED is
  		// on, it will turn off, and vice-versa).
  		TOGGLE_LED1;
  		TOGGLE_LED2;

  		// Wait for approximately 1/4 second
		TimeDelay(delay,resolution);
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
