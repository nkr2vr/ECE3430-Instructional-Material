#include <msp430.h>

#include "LED.h"
#include "timerA.h"
#include "ADC.h"
#include "spi.h"
#include "DAC.h"
#include "debug.h"

// Global Variables
int samples[NUMBER_OF_SAMPLES];
unsigned int CurrentSample;
unsigned int CurrentSampleIndex;
int CumulativeSum;
int Average;
int CurrentOutput;

// Local prototypes
void ConfigureClockModule();
void InitializeSampleBuffer(void);

int main(void)
{
	// Stop the watchdog timer, and configure the clock module.
	WDTCTL = WDTPW + WDTHOLD;
	ConfigureClockModule();

	InitializeLEDPortPins();
	ConfigureTimerA();
	ConfigureADC();
	InitializeSPI();
	InitializeDACPins();

	// Initialize the array used to store the ADC samples, along with the variables
	// used in filter().
	InitializeSampleBuffer();

	SET_DEBUG_LOW;
	SET_DEBUG_TO_AN_OUTPUT;

	_enable_interrupts();

	// Infinite loop
	//while (1) {
	//}
}

void ConfigureClockModule(void)
{
    // Configure Digitally Controlled Oscillator (DCO) using factory calibrations.
	DCOCTL  = CALDCO_12MHZ;
	BCSCTL1 = CALBC1_12MHZ;
}

void InitializeSampleBuffer(void)
{
	unsigned char i;

	for (i = 0; i < NUMBER_OF_SAMPLES; i++) samples[i] = 0;

	Average = 0;
	CumulativeSum = 0;
	CurrentSample = 0;
	CurrentSampleIndex = 0;
	CurrentOutput = 0;
}

