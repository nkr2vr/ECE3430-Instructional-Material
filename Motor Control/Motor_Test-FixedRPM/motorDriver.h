
#include <msp430.h>

#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

#define STBY_BIT		BIT1
#define STBY_DIR		P1DIR
#define STBY_PORT		P1OUT
#define SET_STANDBY_AS_OUTPUT	STBY_DIR |= STBY_BIT
//Standby bit is L for standby
#define MOTOR_STANDBY	        STBY_PORT &= ~STBY_BIT
#define MOTOR_ACTIVE	        STBY_PORT |= STBY_BIT
#define TOGGLE_STBY		        STBY_PORT ^= STBY_BIT
#define MOTOR_STATUS            !(STBY_PORT & STBY_BIT)      //H = Active, L = standby

#define	PWMA_BIT		BIT2
#define PWMA_DIR		P1DIR
#define PWMA_PORT		P1OUT
#define SET_PWMA_AS_OUTPUT		PWMA_DIR |= PWMA_BIT
#define PWMA_H			        PWMA_PORT |= PWMA_BIT
#define PWMA_L			        PWMA_PORT &= ~PWMA_BIT

#define AIN1_BIT		BIT3
#define AIN2_BIT		BIT4
#define AIN_DIR			P1DIR
#define AIN_PORT		P1OUT
#define SET_AIN_AS_OUTPUTS		AIN_DIR |= (AIN1_BIT + AIN2_BIT)
#define SET_MOTOR_MODE          AIN_PORT
#define MOTOR_BRAKE             (AIN_PORT & ~(AIN1_BIT | AIN2_BIT))
#define MOTOR_CW		        ((AIN_PORT | AIN1_BIT) & ~AIN2_BIT)
#define MOTOR_CCW		        ((AIN_PORT | AIN2_BIT) & ~AIN1_BIT)

#define EDGES_PER_REV           465

//Function prototypes
void InitMotor(void);

#endif /* MOTORDRIVER_H_ */
