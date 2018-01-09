#include "msp.h" //included to configure the clock module
#include "LED.h"
#include <stdint.h>

// Function prototypes
void ConfigureClockSystem(void);
void TimeDelay(unsigned int delay, unsigned int resolution);

char *MyP2OUT, *MyP2DIR;        //Port 2 output and direction pointers
volatile uint16_t *MyWDTCTL = (volatile uint16_t *) WATCHDOG_CONTROL_REGISTER_ADDRESS; // Watchdog timer


void main(void)
{
    // Stop the watchdog timer, and configure the clock module.
    *MyWDTCTL = (uint16_t) (WATCHDOG_PASSWORD | WATCHDOG_HOLD);
    //    WDTCTL = WDTPW + WDTHOLD;

    //Assign register addresses to pointers
    MyP2OUT = (char *) PORT2_OUTPUT_REGISTER_ADDRESS;
    MyP2DIR = (char *) PORT2_DATA_DIRECTION_REGISTER_ADDRESS;


    unsigned int delay = 80;     //Represents time delay in unit time increments
    unsigned int resolution = 7; //Represents period of unit time increment

    ConfigureClockSystem();
    InitializeLEDPortPins();

	// Infinite loop
  	while (1)
  	{
        // Toggle the LED states to the complementary state (that is, if the LED is
        // on, it will turn off, and vice-versa).
        TOGGLE_LED1;
        TOGGLE_LED2;
        TOGGLE_LED3;

        // Wait for approximately 1/4 second
        TimeDelay(delay,resolution);
  	}
}

void TimeDelay(unsigned int delay, unsigned int resolution){

    /* The elapsed time for the inner loop represents the period of the unit
     * time increment (such as 1 microsecond or 1 millisecond). The outer loop
     * then represents the total elapsed time in unit time increments
     */

    volatile unsigned int i,j;
    for(i = 0; i < delay; i++){
        for(j = 0; j < resolution; j++){
            __no_operation();
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
