#include "pushbutton.h"

extern char TA0CCR1_delta;

void InitializePushButtonPortPin (void)
{
	// Configure port pin for pushbutton
	ENABLE_PULL_UP_PULL_DOWN_RESISTORS;
	SELECT_PULL_UP_RESISTORS;
	SET_PUSHBUTTON_TO_AN_INPUT;
	ENABLE_PUSHBUTTON_INTERRUPT;
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);
}

//Port 1 interrupt service routine
void PORT1_IRQHandler(void)
{
    CLEAR_PUSHBUTTON_INTERRUPT_FLAG;
    if (TA0CCR1_delta == 0)
    	TA0CCR1_delta = DELTA_VALUE;
    else
    	TA0CCR1_delta = 0;
    
}
