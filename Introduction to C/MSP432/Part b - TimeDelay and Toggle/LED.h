#ifndef LED_H
#define LED_H

#include "msp.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                 LED #1 RED
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   Color     :  Red
 *   Polarity  :  Active High
 *   GPIO      :  P2.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define LED1 1
#define LED1_BIT				BIT0
#define LED1_PORT				P2OUT
#define LED1_DDR				P2DIR
#define SET_LED1_AS_AN_OUTPUT	LED1_DDR |= LED1_BIT
#define TURN_ON_LED1			LED1_PORT |= LED1_BIT
#define TURN_OFF_LED1			LED1_PORT &= ~LED1_BIT
#define TOGGLE_LED1             LED1_PORT ^= LED1_BIT

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                 LED #2 GREEN
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   Color     :  Green
 *   Polarity  :  Active High
 *   GPIO      :  P2.1
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define LED2 2
#define LED2_BIT				BIT1
#define LED2_PORT				P2OUT
#define LED2_DDR				P2DIR
#define SET_LED2_AS_AN_OUTPUT	LED2_DDR |= LED2_BIT
#define TURN_ON_LED2			LED2_PORT |= LED2_BIT
#define TURN_OFF_LED2			LED2_PORT &= ~LED2_BIT
#define TOGGLE_LED2             LED2_PORT ^= LED2_BIT

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                 LED #3 Blue
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   Color     :  Blue
 *   Polarity  :  Active High
 *   GPIO      :  P2.2
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define LED3 3
#define LED3_BIT                BIT2
#define LED3_PORT               P2OUT
#define LED3_DDR                P2DIR
#define SET_LED3_AS_AN_OUTPUT   LED3_DDR |= LED3_BIT
#define TURN_ON_LED3            LED3_PORT |= LED3_BIT
#define TURN_OFF_LED3           LED3_PORT &= ~LED3_BIT
#define TOGGLE_LED3             LED3_PORT ^= LED3_BIT

// Prototypes
void InitializeLEDPortPins(void);

#endif
