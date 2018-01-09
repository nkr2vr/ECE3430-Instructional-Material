#include "msp.h"
#include "LED.h"

// Function prototypes
void ConfigureClockSystem(void);

void main(void)
{
    unsigned int delay = 80;
    unsigned int resolution = 7; //Produces 5ms delay
    unsigned int i,j;

    // Stop the watchdog timer, and configure the clock module.
    WDTCTL = WDTPW + WDTHOLD;
	ConfigureClockSystem();
    InitializeLEDPortPins();

	// Infinite loop
  	while (1)
  	{
        TURN_ON_LED1;
        TURN_ON_LED2;
        TURN_ON_LED3;

        // Wait for approximately 1/4 second
        for (i = 0; i < delay; i++) {
            for (j = 0; j < resolution; j++){
                __no_operation();
            }
        }

        TURN_OFF_LED1;
        TURN_OFF_LED2;
        TURN_OFF_LED3;

        // Wait for approximately 1/4 second
        for (i = 0; i < delay; i++) {
            for (j = 0; j < resolution; j++){
                __no_operation();
            }
        }
  	}
}

void ConfigureClockSystem(void){

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
    int offset = 0xFEEE;
    CS->CTL0 = CS_CTL0_DCOEN | CS_CTL0_DCORSEL_0 | (CS_CTL0_DCOTUNE_MASK & offset);


    //Set DCO as source for SMCLK
    CS->CTL1 = CS_CTL1_SELS__DCOCLK;

    //Enable SMCLK
    CS->CLKEN = CS_CLKEN_SMCLK_EN;

    //Lock to prevent further changes.
    CS->KEY = 0;
}
