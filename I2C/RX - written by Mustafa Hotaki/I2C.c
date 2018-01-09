#include "I2C.h"

// Configure I2C as slave receiver
void ConfigureI2C(void) {
    P1SEL |= BIT6 + BIT7;                     // P1.6 and P1.7 in I2C functionality
    P1SEL2|= BIT6 + BIT7;                     // P1.6 and P1.7 in I2C functionality
    UCB0CTL1 |= UCSWRST;                      // Enable SW reset
    UCB0CTL0 = UCMODE_3 + UCSYNC;             // I2C Slave, synchronous mode
    UCB0I2COA = 0x48;                         // Own Address is 048h
    UCB0CTL1 &= ~UCSWRST;                     // Release from software reset
    IE2 |= UCB0RXIE;                          // Enable I2C RX interrupt
}

// USCI_B0 Data ISR
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void) {
  RXData = UCB0RXBUF;                       // Get RX data
  TA0CCR1 = RXData;
}
