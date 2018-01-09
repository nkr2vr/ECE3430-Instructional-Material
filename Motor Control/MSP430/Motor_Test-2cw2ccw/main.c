#include <msp430.h>
#include "debounce.h"
#include "LED.h"
#include "timerA.h"
#include "motorDriver.h"

// Global Variables
unsigned int g1mSTimeout;		// This variable is incremented by the interrupt handler and								// decremented by a software call in the main loop.
unsigned int g1msTimer;			// This variable is incremented in ManageSoftwareTimers
unsigned int tEnd; 				// Will be used to keep track of when states were entered

SwitchDefine resetPin;		    // Active low reset pin P2.0
SwitchDefine rotA; 				// Rotary encoder switch A
SwitchDefine rotB; 				// Rotary encoder switch B
rotState currentRotState;		// Current rotary state
pwmMode spin;

int edgeCount; 						// Used to for rotery encoder state machine
int revs;

// Function Prototypes
void ConfigureClockModule();
void InitializeGlobalVariables(void);
void ManageSoftwareTimers(void);	// This function manages software timers.

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
    ConfigureClockModule();

    // Initialization of hardware.
	InitializeGlobalVariables();

    // Initialize port pins associated with the LEDs and turn off LEDs.
    // SetLEDState(LED1,OFF);
    // SetLEDState(LED2,OFF);
    // InitializeLEDPortPins();

    // Init motor port pins
    InitMotor();

    // Configure timer A to generate the required interrupt.
	ConfigureTimerA();

	_enable_interrupt();		// interrupts enabled
	MOTOR_ACTIVE;
	while(1){
            ManageSoftwareTimers();
            if((spin == up) && (revs < 1)){
                SET_MOTOR_MODE = MOTOR_CCW;
                PWM_DUTY = PWM_PRESET;
            } else if((spin == up) && (revs == 1)) {
                SET_MOTOR_MODE = MOTOR_BRAKE;
                _delay_cycles(8000000); //Should pause for 500ms
                spin = down;
                PWM_DUTY = 0;
                SET_MOTOR_MODE = MOTOR_CW;
            } else if((spin == down) && (revs > -1)){
                SET_MOTOR_MODE = MOTOR_CW;
                PWM_DUTY = PWM_PRESET;
            } else if((spin == down) && (revs == -1)){
                SET_MOTOR_MODE = MOTOR_BRAKE;
                _delay_cycles(8000000);
                spin = up;
                PWM_DUTY = 0;
                SET_MOTOR_MODE = MOTOR_CCW;
            }
	}
}

void ConfigureClockModule()
{
	// Configure Digitally Controlled Oscillator (DCO) using factory calibrations
	DCOCTL  = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
}

void InitializeGlobalVariables(void)
{
	g1mSTimeout = 0;
	g1msTimer = 0;

	InitializeSwitch(&resetPin,(char *) &P2IN,(unsigned char) BIT0, HIGH_THRESHOLD, LOW_THRESHOLD);
	//Pin is being used as active low, needs additional steps
	P2REN |= BIT0;
	P2OUT |= BIT0;
	
	InitializeSwitch(&rotA, (char *) &P2IN, (unsigned char) BIT4, 5,5);
	InitializeSwitch(&rotB, (char* ) &P2IN, (unsigned char) BIT3, 5,5);
	edgeCount = 0;
	currentRotState = INIT;
	spin = up;
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
	
	if (edgeCount == 480){
	    revs += 1;
		edgeCount = 0;
	}
	if (edgeCount == -480){
		revs -= 1;
		edgeCount = 0;
	}
}
