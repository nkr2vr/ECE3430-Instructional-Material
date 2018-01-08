#include "pushbutton.h"

extern char LEDBit;

void InitializePushButtonPortPin (void)
{
	// Configure port pin for pushbutton
	ENABLE_PULL_UP_PULL_DOWN_RESISTORS;
	SELECT_PULL_UP_RESISTORS;
	SET_PUSHBUTTON_TO_AN_INPUT;
	ENABLE_PUSHBUTTON_INTERRUPT;
	NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);
}

// Button interrupt service routine, add code here for POST LAB
void PORT1_IRQHandler(void){
    CLEAR_PUSHBUTTON_INTERRUPT_FLAG;
    if (LEDBit == LED1_BIT)
    {
    	// Assign new value to variable, LEDBit
    	LEDBit = LED2;

    	TURN_OFF_LED1;
    	TURN_ON_LED2;
    }
    else
    {
    	// Assign new value to variable, LEDBit
    	LEDBit = LED1;

    	TURN_OFF_LED2;
    	TURN_ON_LED1;
    }
}
