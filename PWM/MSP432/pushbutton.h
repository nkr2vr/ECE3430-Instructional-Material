#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include "msp.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                 Pushbutton
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   GPIO      :  P1.3
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define PUSHBUTTON_BIT						BIT1	// Port pin bit location for pushbutton
#define PUSHBUTTON_REN  					P1REN   // Register to enable resistors for pushbutton
#define PUSHBUTTON_PORT						P1OUT 	// Register to select pull-up/pull-down
#define PUSHBUTTON_DDR						P1DIR 	// Data Direction Register (DDR) for pushbutton.
#define PUSHBUTTON_IE   					P1IE    // Port interrupt enable
#define PUSHBUTTON_IFG  					P1IFG   // Port interrup flag
#define SET_PUSHBUTTON_TO_AN_INPUT			PUSHBUTTON_DDR &= ~PUSHBUTTON_BIT
#define ENABLE_PULL_UP_PULL_DOWN_RESISTORS	PUSHBUTTON_REN |= PUSHBUTTON_BIT
#define SELECT_PULL_UP_RESISTORS			PUSHBUTTON_PORT |= PUSHBUTTON_BIT
#define ENABLE_PUSHBUTTON_INTERRUPT			PUSHBUTTON_IE |= PUSHBUTTON_BIT
#define CLEAR_PUSHBUTTON_INTERRUPT_FLAG		PUSHBUTTON_IFG &= ~PUSHBUTTON_BIT

#define DELTA_VALUE    1

// Prototypes
void InitializePushButtonPortPin(void);

#endif
