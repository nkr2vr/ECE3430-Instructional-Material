#include <msp430.h>
#include "debounce.h"
#include "timerA.h"
#include "motorDriver.h"
#include "LED.h"

#define UINT_MAX        65535
#define TARGET_RPM      60
#define MS_TOLERANCE    2

// Global Variables
unsigned int g1mSTimeout;		// This variable is incremented by the interrupt handler and								// decremented by a software call in the main loop.
unsigned int g1msTimer;			// This variable is incremented in ManageSoftwareTimers
unsigned int tEnd; 				// Will be used to keep track of when states were entered
unsigned int eventTimer;

SwitchDefine resetPin;		    // Active low reset pin P2.0
SwitchDefine rotA; 				// Rotary encoder switch A
SwitchDefine rotB; 				// Rotary encoder switch B
rotState currentRotState;		// Current rotary state

int edgeCount; 						// Used to for rotery encoder state machine
int revs;
int revTime;
int targetRevTime;

// Function Prototypes
void ConfigureClockModule();
void InitializeGlobalVariables(void);
void ManageSoftwareTimers(void);	// This function manages software timers.

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	ConfigureClockModule();
	InitializeGlobalVariables();
	InitMotor();
	ConfigureTimerA();
	InitializeLEDPortPins();
	
	_enable_interrupt();
	
	MOTOR_ACTIVE;               //Remove standby mode
	SET_MOTOR_MODE = MOTOR_CCW;
	PWM_DUTY = PWM_FULL;        //Start at max speed, wind down
	
	unsigned char i = 0;
	for(;;){
        ManageSoftwareTimers();
        
        if (edgeCount == EDGES_PER_REV){
    	    revs += 1;          //track revolutions for debugging purposes
    		edgeCount = 0;
    		TOGGLE_LED1;
    		if(i < TABLE_LENGTH){   //Add or subtract next increment from lookup table
        		revTime = g1msTimer - eventTimer;
        		if(revTime < 0)     //Account for integer overflow in g1msTimer
        		    revTime = UINT_MAX - eventTimer + g1msTimer;
        		    
        		/*
        		PWM value changes like binary search indeces, using lookup table
        		(instead of recursion and variables) to save memory and clock
        		cycles.
        		*/
        		if((targetRevTime - revTime) > MS_TOLERANCE)
        		    //Slow down, revTime is too short
        		    PWM_DUTY -= timerIncrements[i++];
        		else if((revTime - targetRevTime) > MS_TOLERANCE)
        		    //Speed up, revTime is too long
        		    PWM_DUTY += timerIncrements[i++];
        		//Otherwise, speed is within tolerance, no more adjustment needed
        		
        		// Set eventTimer for next revolution
        		eventTimer = g1msTimer;
    	    }
        }
	}
	
	return 0;
}

void ConfigureClockModule()
{
	// Configure Digitally Controlled Oscillator (DCO) using factory calibrations
	DCOCTL  = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
}

void InitializeGlobalVariables(void)
{
    //Timing variables
	g1mSTimeout = 0;
	g1msTimer = 0;
	eventTimer = 0;
    revTime = 0;
    targetRevTime = (int) ((float)1000.0 * 60 / TARGET_RPM);  //time in ms for one revolution

    //Switches and rotary encoder information
	InitializeSwitch(&resetPin,(char *) &P2IN,(unsigned char) BIT0, HIGH_THRESHOLD, LOW_THRESHOLD);
	//Pin is being used as active low, needs additional steps
	P2REN |= BIT0;
	P2OUT |= BIT0;
	InitializeSwitch(&rotA, (char *) &P2IN, (unsigned char) BIT4, 5,5);
	InitializeSwitch(&rotB, (char* ) &P2IN, (unsigned char) BIT3, 5,5);
	
	//Encoder state machine variables
	edgeCount = 0;
	currentRotState = INIT;
	revs = 0;
}

void ManageSoftwareTimers(void)
{
    
    if(g1mSTimeout){
        g1mSTimeout = 0;
        g1msTimer += 1;
    }
    
	Debouncer(&resetPin);
	Debouncer(&rotA);
	Debouncer(&rotB);
	updateState(&resetPin, &rotA, &rotB);
	
}

