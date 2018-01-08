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

void SetLEDState(LED *MyLED, LEDState state)
{
    if (state == On) {
        *(MyLED->LED_Port) |= MyLED->LED_Bit;
    }
    else {
        *(MyLED->LED_Port) &= ~(MyLED->LED_Bit);
    }
}
