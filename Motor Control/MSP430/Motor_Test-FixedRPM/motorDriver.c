
#include "motorDriver.h"

void InitMotor(void){
	//Motor pin direction register
	SET_STANDBY_AS_OUTPUT;
	SET_PWMA_AS_OUTPUT;
	SET_AIN_AS_OUTPUTS;

	//Initialize motor pins
	MOTOR_STANDBY;
	SET_MOTOR_MODE = MOTOR_BRAKE;
	
	//Create Active-Low Reset pin
	
}
