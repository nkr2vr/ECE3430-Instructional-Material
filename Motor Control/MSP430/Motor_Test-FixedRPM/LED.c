#include "LED.h"

void InitializeLEDPortPins(void)
{
	// Set the LEDs as outputs
	SET_LED1_AS_AN_OUTPUT;
	TURN_OFF_LED1;
// 	SET_LED2_AS_AN_OUTPUT;
}

