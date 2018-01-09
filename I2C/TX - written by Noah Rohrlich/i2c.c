#include "i2c.h"

void InitializeI2C(void)
{
    //Configure output pins from module, Direction Register is irrelevant
    USCIB0_I2C_SEL |= (USCIB0_I2C_SDA_BIT + USCIB0_I2C_SCL_BIT );
    USCIB0_I2C_SEL2 |= (USCIB0_I2C_SDA_BIT + USCIB0_I2C_SCL_BIT );

    //Set bit in order to configure register
    DISABLE_USCI;
    UCB0CTL0 = UCMODE_3 | UCSYNC;                       //I2C Mode, Synchronous
    UCB0CTL1 = UCSSEL_3;                                //BRCLK = SMCLK
    I2C_OWN_ADDRESS_REG = OWN_ADDRESS;
    I2C_OTHER_ADDRESS_REG = OTHER_ADDRESS;
    
    UCB0BR0 = BITCLOCK_PRESCALE;
    
    //Interrupt enable registers
    UCB0I2CIE = 0;                      // State change interrupt enable register
    IE2 = 0;                            // RX interrupt enable
    
    I2C_RECEIVE;                        // Default to slave receive
    RESET_RXIFG;
    ENABLE_USCI;                        // Ready for operation
}

void SendByteI2C(uint8_t dataByte){
    while (!TXBUF_EMPTY);
    UCB0TXBUF = dataByte;
}

//Returns 0 if data acknowledged as written, 1 otherwise
//Used only when interfacing with passive receiver devices with defined registers
uint8_t SendWriteCommandI2C(uint8_t deviceAddress, uint8_t regAddress, uint16_t txdata)
{
    while (I2C_BUSY);
    I2C_OTHER_ADDRESS_REG = deviceAddress;

    I2C_TRANSMIT;
    I2C_START;
    while (ADDRESS_SENDING);

    //MSP holds SCL low until TXBUF written
    //If NACK received, TXBUF discarded automatically by module
    if (NACK_RECEIVED){
        I2C_STOP;
        while (I2C_STOP_PENDING);       //Wait for STOP condition
        return 1;                       //error code 1: Byte was NACKed
    }
    SendByteI2C(regAddress);            //Put a register address in the device pointer

    if (NACK_RECEIVED){
        I2C_STOP;
        while (I2C_STOP_PENDING);       //Wait for STOP condition
        return 1;                       //error code 1: Byte was NACKed
    }
    //Register address sent, now send data, MSB first
    SendByteI2C(txdata >> 8);

    if (NACK_RECEIVED){
        I2C_STOP;
        while (I2C_STOP_PENDING);       //Wait for STOP condition
        return 1;                       //error code 1: Byte was NACKed
    }
    SendByteI2C(txdata & 0x00FF);
    I2C_STOP;                       //Set during transmission of LSB
    while (I2C_STOP_PENDING);       //Wait for STOP condition
    return 0;                   //FINAL RETURN, NO NACKS

}

/*
 * To read a register in an I2C device, an address must be written to the "pointer" first.
 * The STOP condition can be sent after.
 * Then, upon issuing a READ command, the data returned will be that from the register address
 * in the pointer.
 */
uint16_t SendReadCommandI2C(uint8_t deviceAddress, uint8_t regAddress)
{
    uint16_t result = UCB0RXBUF;        //Trivial operation, but reading it resets IFG.

    while (I2C_BUSY && (!TXBUF_EMPTY));
    I2C_OTHER_ADDRESS_REG = deviceAddress;
    I2C_TRANSMIT;
    I2C_START;
    while (ADDRESS_SENDING);
    //Proceed only if address was acknowledged (not NACK)
    if (NACK_RECEIVED)
    {
        I2C_STOP;
        while (I2C_STOP_PENDING);
        return 0;
    }
    //Proceed only if address was ACKed
    SendByteI2C(regAddress);             //Put a register address in the device pointer
    if (NACK_RECEIVED)
    {
        I2C_STOP;
        while (I2C_STOP_PENDING);
        return 0;
    }

    I2C_RECEIVE;
    I2C_START;                      //end partial WRITE sequence, start receive
    while (ADDRESS_SENDING);        //Wait for completion of address send
    while (!DATA_RECEIVED);        //Wait for character received
    result = (UCB0RXBUF << 8);      //Reading RXBUF clears flag and ACKs to device
    RESET_RXIFG;                    //MSP holds SCL low until flag is low, slave can transmit again

    while (!DATA_RECEIVED);
    result |= UCB0RXBUF;
    I2C_STOP;                       //Send STOP to be done after byte is received
    while (I2C_STOP_PENDING);       //Wait for STOP condition

    //Get information back from it
    return result;

}

//ISR for USCIB I2C Tx and Rx interrupts, and Tx interrupt for USCIA
// #pragma vector = USCIAB0TX_VECTOR
// __interrupt void USCIAB0TX_ISR(void){
//     //Handles multiple interrupts for multiple USCIs
//     //TxIFG is disabled for USCIA, as is TxIFG for USCIB
//     RESET_RXIFG;
    
//     receive_char = UCB0RXBUF;
//     char_has_not_been_received = FALSE;
//     //This will send ACK after reading
// }
