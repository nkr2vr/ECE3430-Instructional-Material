#include <msp430.h>
#include "i2c.h"
#include "timerA.h"

void ConfigureClockModule(void);
typedef enum { up, down } pwmMode;      // This is a list, don't change it during runtime
pwmMode direction = up;                 // This is a variable, change it during runtime

void main(void){
    WDTCTL = WDTPW + WDTHOLD;
    ConfigureClockModule();
    InitializeI2C();
    
    char pulseWidth = 1;
    
    _enable_interrupts();
    
    DISABLE_USCI;
    MASTER_MODE;
    I2C_TRANSMIT;
    ENABLE_USCI;
    
    while(1){
        
        I2C_START;
        if(NACK_RECEIVED){
            I2C_STOP;
            while(I2C_STOP_PENDING);
            continue;
        }
        SendByteI2C(pulseWidth);
        while(!TXBUF_EMPTY);
        I2C_STOP;
        while(I2C_STOP_PENDING);
        
        // Wait before incrementing
        // 125ms delay = 125 * 1ms = 125 * 16e6/1e3
        TimeDelay(4, 16000);
        if(direction == up){
            pulseWidth += 1;
            if(!pulseWidth){        // Pulsewidth has overflown
                direction = down;
                pulseWidth = 0xFF;
            }
            
        } else {
            pulseWidth -= 1;
            if(!pulseWidth){        // PulseWidth has overflown
                direction = up;
                pulseWidth = 0x01;
            }
        }
    }
}

void ConfigureClockModule(void){
    DCOCTL  = CALDCO_16MHZ;
    BCSCTL1 = CALBC1_16MHZ;

}
