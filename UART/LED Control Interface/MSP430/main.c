//
// This program presents a simple User Interface (UI) for controlling the LEDs.
//
#include <msp430.h>
#include <stdio.h>

#include "uart.h"
#include "LED.h"
#include "timerA.h"

#define BUFFER_SIZE 80
#define TRUE 1
#define FALSE 0
#define NUMBER_OF_BLINKS 10

// Global variable
char char_has_not_been_received;
char receive_char;

// Static variables
unsigned int i;                     // Used for indexing
static char prompt_string[BUFFER_SIZE];    // Character buffer to hold each string to be printed
// static unsigned int delay = 250;           // Represents time delay in unit time increments
// static unsigned int resolution = 1000;     // Represents period of unit time increment

// Function prototypes
void ConfigureClockModule(void);
void SendStringUART(char *);

// Simplifies function calls
void SendStringUART(char * formatString){
        for (i = 0; i < BUFFER_SIZE; i++) prompt_string[i] = (char) NULL;
		sprintf(prompt_string, formatString);
		WriteStringUART(prompt_string);
}

void ConfigureClockModule()
{
	// Configure Digitally Controlled Oscillator (DCO) for 1 MHz using factory
	// calibrations.
	DCOCTL  = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;
}

void main(void)
{

	// Stop the watchdog timer, and configure the clock module.
	WDTCTL = WDTPW + WDTHOLD;
	ConfigureClockModule();

    // Initialize LEDs
    InitializeLEDPortPins();

	// Initialize USCI A0 to use UART.
	InitializeUART();

	_enable_interrupts();		// interrupts enabled

    // "Clear" the screen with a carriage return ('\r') and line feed ('\n'),
	// follwed by the prompt string.
	SendStringUART("\r\nHello World!");
    
	// Initialize receive_char, and continue with the UI until the user chooses to exit.
	receive_char = (char) NULL;
	while (TRUE) {
        
        SendStringUART("\r\nInput Character: ");
		// Wait for a character to be entered.
		char_has_not_been_received = TRUE;
		while (char_has_not_been_received);

		// Echo character
		SendStringUART("\r\n>>> ");
		WriteCharacterUART(receive_char);
	}
}


