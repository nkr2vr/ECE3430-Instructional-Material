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
#define UART 0

// Global variable
char char_has_not_been_received;
char receive_char;

// Function prototypes
void ConfigureClockModule(void);
void PrintString(char *, unsigned char);

void main(void)
{
	unsigned char PrintDevice;

	// Stop the watchdog timer, and configure the clock module.
	WDTCTL = WDTPW + WDTHOLD;
	ConfigureClockModule();

    // Initialize LEDs
    InitializeLEDPortPins();

	// Initialize USCI A0 to use UART.
	InitializeUART(); PrintDevice = UART;

	_enable_interrupts();		// interrupts enabled

    // "Clear" the screen with a carriage return ('\r') and line feed ('\n'),
	// followed by the prompt string.
	PrintString("\r\nHello World!",PrintDevice);
    
	// Initialize receive_char, and continue with the UI until the user chooses to exit.
	receive_char = (char) NULL;
	while (TRUE) {
        
		PrintString("\r\nInput Character: ",PrintDevice);
		// Wait for a character to be entered.
		char_has_not_been_received = TRUE;
		while (char_has_not_been_received);

		// Echo character
		PrintString("\r\n>>> ",PrintDevice);
		WriteCharacterUART(receive_char);
	}
}

void PrintString(char * StringToPrint, unsigned char PrintDeviceType)
{
	unsigned int i;                     // Used for indexing
	char StringBuffer[BUFFER_SIZE];    // Character buffer to hold each string to be printed

	for (i = 0; i < BUFFER_SIZE; i++) StringBuffer[i] = (char) NULL;
	sprintf(StringBuffer, StringToPrint);
	switch (PrintDeviceType) {
	case UART:
		WriteStringUART(StringBuffer);
		break;
	default: break;
	}
}

void ConfigureClockModule()
{
	// Configure Digitally Controlled Oscillator (DCO) for 1 MHz using factory
	// calibrations.
	DCOCTL  = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;
}

