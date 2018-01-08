#ifndef LED_H
#define LED_H

//#include <stdint.h>

// Local definitions that do not use definitions in "msp.h"
#define PORT_REGISTER_BASE                      0x40004C00
#define PORT2_OUTPUT_REGISTER_OFFSET            0x003
#define PORT2_DIRECTION_REGISTER_OFFSET         0x005
#define PORT2_OUTPUT_REGISTER_ADDRESS           (PORT_REGISTER_BASE + PORT2_OUTPUT_REGISTER_OFFSET)
#define PORT2_DATA_DIRECTION_REGISTER_ADDRESS   (PORT_REGISTER_BASE + PORT2_DIRECTION_REGISTER_OFFSET)
#define WATCHDOG_REGISTER_BASE                  (uint16_t) 0x40004800
#define WATCHDOG_CONTROL_REGISTER_OFFSET        (uint16_t) 0x00C
#define WATCHDOG_CONTROL_REGISTER_ADDRESS       (WATCHDOG_REGISTER_BASE + WATCHDOG_CONTROL_REGISTER_OFFSET)
#define WATCHDOG_PASSWORD                       (0x005A)
#define WATCHDOG_HOLD                           (0x0080)
#define MY_BIT0                                 (0x0001)
#define MY_BIT1                                 (0x0002)
#define MY_BIT2                                 (0x0004)
#define MY_P2OUT *MyP2OUT
#define MY_P2DIR *MyP2DIR

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                 LED #1 RED
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   Color     :  Red
 *   Polarity  :  Active High
 *   GPIO      :  P2.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define LED1 1
#define LED1_BIT				MY_BIT0
#define LED1_PORT				MY_P2OUT
#define LED1_DDR				MY_P2DIR
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
#define LED2_BIT				MY_BIT1
#define LED2_PORT				MY_P2OUT
#define LED2_DDR				MY_P2DIR
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
#define LED3_BIT                MY_BIT2
#define LED3_PORT               MY_P2OUT
#define LED3_DDR                MY_P2DIR
#define SET_LED3_AS_AN_OUTPUT   LED3_DDR |= LED3_BIT
#define TURN_ON_LED3            LED3_PORT |= LED3_BIT
#define TURN_OFF_LED3           LED3_PORT &= ~LED3_BIT
#define TOGGLE_LED3             LED3_PORT ^= LED3_BIT

// Prototypes
void InitializeLEDPortPins(void);

#endif
