#ifndef USCI_UART_H
#define USCI_UART_H
#include "msp.h"

/* Baud rate = SMCLK / 104 = ~9600 for SMCLK = 1 MHz in low-frequency mode
 * (UCOS16 = 0). Also, UCBRSx = 1 and UCBRFx = 0 (ignored for UCOS16 = 0).
 * See Table 15-4 in the MSP430x2xx Family User's Guide (104 = 0x0068)
 */
//Baud rate 9600 with 1MHz clock-> 104 = BR0 + 256*BR1
//#define UCA0BR1_VALUE 0
//#define UCA0BR0_VALUE 104

//Baud rate 9600 with 8MHz clock-> 833
#define UCA0BR0_VALUE 52

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * USCI A0 UART mode: receive data input
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * GPIO      :  P1.1
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define USCIA0_RXD_BIT		BIT2
#define USCIA0_RXD_P1SEL0   P1SEL0
#define USCIA0_RXD_P1SEL1   P1SEL1

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * USCI A0 USCI_A0 UART mode: transmit data output
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * GPIO      :  P1.2
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define USCIA0_TXD_BIT		BIT3
#define USCIA0_TXD_P1SEL0   P1SEL0
#define USCIA0_TXD_P1SEL1   P1SEL1

void InitializeUART(void);
void ShutdownUART(void);
void WriteStringUART(char *pt);
void WriteCharacterUART(char data);
  
#endif /* USCI_UART_H */
