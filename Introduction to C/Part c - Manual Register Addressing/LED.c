#include "LED.h"

// declared as global variables in main.c
extern char MY_P2OUT, MY_P2DIR;

void InitializeLEDPortPins(void)
{
	// Set the initial value and port pin direction for the LEDs.
	TURN_OFF_LED1;
	SET_LED1_AS_AN_OUTPUT;

	TURN_OFF_LED2;
	SET_LED2_AS_AN_OUTPUT;

	TURN_OFF_LED3;
	SET_LED3_AS_AN_OUTPUT;
}
