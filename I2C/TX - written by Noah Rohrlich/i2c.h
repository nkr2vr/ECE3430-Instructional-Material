#ifndef I2C_H
#define I2C_H
#include <msp430.h>
#include <stdint.h>

#define TRUE    1
#define FALSE   0

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * USCI B1 I2C mode
 * ~~~~~~~~~~~~~~~~~
 * SDA:     P1.7
 * SCL:     P1.6
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
 
#define USCIB0_I2C_SDA_BIT      BIT7
#define USCIB0_I2C_SCL_BIT      BIT6
//Port selection
#define USCIB0_I2C_SEL          P1SEL
#define USCIB0_I2C_SEL2         P1SEL2

//Own and other address
#define OWN_ADDRESS             0x40
#define OTHER_ADDRESS           0x48

#define I2C_CONTROL0_REG        UCB0CTL0
#define I2C_CONTROL1_REG        UCB0CTL1
#define I2C_STATUS_REGISTER     UCB0STAT
#define I2C_OWN_ADDRESS_REG     UCB0I2COA
#define I2C_OTHER_ADDRESS_REG   UCB0I2CSA
#define I2C_INT_FLAG_REG        IFG2
#define BITCLOCK_PRESCALE       160         //I2C clock @100,000Hz

//Manual I2C controls
#define DISABLE_USCI            I2C_CONTROL1_REG |= UCSWRST
#define ENABLE_USCI             I2C_CONTROL1_REG &= ~UCSWRST
#define MASTER_MODE             I2C_CONTROL0_REG |= UCMST
#define SLAVE_MODE              I2C_CONTROL0_REG &= ~UCMST
#define I2C_START               I2C_CONTROL1_REG |= UCTXSTT
#define I2C_STOP                I2C_CONTROL1_REG |= UCTXSTP
#define I2C_NACK                I2C_CONTROL1_REG |= UCTXNACK
//These bits will automatically set R/W bit after slave address sent
#define I2C_TRANSMIT            I2C_CONTROL1_REG |= UCTR
#define I2C_RECEIVE             I2C_CONTROL1_REG &= ~UCTR

//Status polls
#define ADDRESS_SENDING         (I2C_CONTROL1_REG & UCTXSTT)            //Autocleared after address
#define I2C_STOP_PENDING        (I2C_CONTROL1_REG & UCTXSTP)            //Autocleared after STOP sent
#define I2C_NACK_PENDING        (I2C_CONTROL1_REG & UCTXNACK)           //Autocleared after NACK sent

#define I2C_BUS_HELD            (I2C_STATUS_REGISTER & UCSCLLOW)        //TX holding SCL until data placed in buffer
#define I2C_BUSY                (I2C_STATUS_REGISTER & UCBBUSY)
#define NACK_RECEIVED           (I2C_STATUS_REGISTER & UCNACKIFG)       //Autocleared on STT
#define I2C_STOP_CONDITION      (I2C_STATUS_REGISTER & UCSTPIFG)        //Reset on START condition
#define I2C_START_CONDITION     (I2C_STATUS_REGISTER & UCSTTIFG)        //Reset on STOP condition

#define TXBUF_EMPTY             (I2C_INT_FLAG_REG & UCB0TXIFG)          //Set when TXBUF is empty
#define DATA_RECEIVED           (I2C_INT_FLAG_REG & UCB0RXIFG)
#define RESET_RXIFG             (I2C_INT_FLAG_REG &= ~UCB0RXIFG)

//Function Prototypes
void InitializeI2C(void);
void SendByteI2C(uint8_t);
uint8_t SendWriteCommandI2C(uint8_t, uint8_t, uint16_t);
uint16_t SendReadCommandI2C(uint8_t, uint8_t);

  
#endif /* I2C_H */
