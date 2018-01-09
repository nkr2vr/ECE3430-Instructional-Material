#ifndef FSM_DEBOUNCE_H_
#define FSM_DEBOUNCE_H_

#include <msp430.h>

// Definitions for debounce Times for the P1.3 button (ms)
#define HIGH_THRESHOLD 10
#define LOW_THRESHOLD 20

// Type Definitions
typedef enum {
	DbExpectHigh, DbValidateHigh, DbExpectLow, DbValidateLow
} DbState;

typedef enum {
	INIT, AHBH, AHBL, ALBH, ALBL
} rotState;

typedef enum {
	Low, High
} SwitchStatus;

// This structure should encapsulate all of the information needed for
// a switch.  As such, You will NEED TO insert more definitions in this struct.
// For example, you should include values for your High/Low threshold delay times.
typedef struct {
	DbState CurrentState;			// Current state of the FSM
	char * SwitchPort;				// Input port associated with switch
	unsigned char SwitchPortBit;	// Port pin associated with switch
	unsigned int highThreshold;		// Threshold for switch going to high
	unsigned int lowThreshold; 		// Threshold for switch going back to low
} SwitchDefine;

// Function Prototypes
void InitializeSwitch(SwitchDefine *Switch,char *SwitchPort,unsigned char SwitchBit, unsigned int hightT, unsigned int lowT);

//This function returns the instantaneous value of the selected switch
SwitchStatus GetSwitch(SwitchDefine *Switch);

//This function debounces a switch input
SwitchStatus Debouncer(SwitchDefine *Switch);

rotState updateState(SwitchDefine *Switch,SwitchDefine *aRot,SwitchDefine *bRot);

#endif
