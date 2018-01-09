#include "LED.h"

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

void SetLEDState(char choice, LEDState state)
{
    switch (choice) {
        case LED1:
            if (state == On)
                TURN_ON_LED1;
            else
                TURN_OFF_LED1;
            break;
        case LED2:
            if (state == On)
                TURN_ON_LED2;
            else
                TURN_OFF_LED2;
            break;
        case LED3:
            if (state == On)
                TURN_ON_LED3;
            else
                TURN_OFF_LED3;
            break;
        default:
            break;
    }
    return;
}
