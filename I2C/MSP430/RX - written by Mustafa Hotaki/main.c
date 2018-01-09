#include <msp430.h>
#include "I2C.h"
#include "timerA.h"

// Globals
unsigned char RXData;

// Function Protoypes
void ConfigureClockModule(void);

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    ConfigureClockModule();
    ConfigureTimerA();
    ConfigureI2C();
    _enable_interrupts();                     // Enable interrupts globally

    while(1);
}

// Configure System Clock
void ConfigureClockModule(void) {
    DCOCTL  = CALDCO_16MHZ;
    BCSCTL1 = CALBC1_16MHZ;
}
