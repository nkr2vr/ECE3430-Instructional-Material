#include "debounce.h"

extern rotState currentRotState;
extern int edgeCount;
extern int revs;

void InitializeSwitch(SwitchDefine *Switch,char *SwitchPort,unsigned char SwitchBit, unsigned int highT, unsigned int lowT)
{
	Switch->CurrentState = DbExpectHigh;
	Switch->SwitchPort = SwitchPort;
	Switch->SwitchPortBit = SwitchBit;
	Switch->highThreshold = highT;
	Switch->lowThreshold = lowT;
}

SwitchStatus GetSwitch(SwitchDefine *Switch)
{
	char val = (*(Switch->SwitchPort)) & (Switch->SwitchPortBit);
	_nop();
	if(val){
		return Low;
	}
	else{
		return High;
	}
}

SwitchStatus Debouncer(SwitchDefine *Switch)
{
	extern unsigned int g1msTimer; // proper timer
	extern unsigned int g1mSTimeout;  //queue of time to be added to timer
	extern unsigned int tEnd; //keeps track of when state is allowed to move on in cases where time matters

	extern unsigned char output;

	SwitchStatus CurrentSwitchReading = GetSwitch(Switch);
	DbState NextState = DbExpectHigh;
	
	// First, determine the current inputs.

	/*
	 * The below chart shows the reactions of the state machine in varius states. High and Low are self
	 * explenatory, but t < v or t > v means that the elapsed time since entering that state is greater
	 * or less than the threshold value specified by the switch.
	 *
	 * States are:
	 * 				EH - Expect High
	 * 				VH - Validate High
	 * 				EL - Expect Low
	 * 				VL - Validate Low
	 *
	 * 		 						 Possible Inputs
	 * STATE | High & t < v | High & t >= v | Low & t < v | Low & t >= v
	 * -----------------------------------------------------------------
	 *   EH  |      VH      |       VH      |      EH     |      EH
	 * -----------------------------------------------------------------
	 *   VH  |      VH      |       EL      |      EH     |      EH
	 * -----------------------------------------------------------------
	 *   EL  |      EL      |       EL      |      VL     |	     VL
	 * -----------------------------------------------------------------
	 *   VL  |      EL      |       EL      |      VL     |      EH
	 *
	 */
	// Next, based on the input values and the current state, determine the next state.
	switch (Switch->CurrentState) {
		case DbExpectHigh:

			if(CurrentSwitchReading == High){
				NextState = DbValidateHigh;
				tEnd = g1msTimer + (Switch->highThreshold);
				if(tEnd >= 500){
					tEnd = tEnd - 500;
				}
			}
			else{
				NextState = DbExpectHigh;
			}
		break;
		case DbValidateHigh:
			if(CurrentSwitchReading == Low){
				NextState = DbExpectHigh;
			}
			else{
				if(g1msTimer > tEnd){
					NextState = DbExpectLow;
				}
				else{
					NextState = DbValidateHigh;
				}
			}
		break;
		case DbExpectLow:
			if(CurrentSwitchReading == Low){
							NextState = DbValidateLow;
							tEnd = g1msTimer + (Switch->lowThreshold);
							if(tEnd >= 500){
								tEnd = tEnd - 500;
							}
						}
						else{
							NextState = DbExpectLow;
						}
		break;
		case DbValidateLow:
			if(CurrentSwitchReading == High){
				NextState = DbExpectLow;
			}
			else{
				if(g1msTimer > tEnd){
					NextState = DbExpectHigh;
				}
				else{
					NextState = DbValidateLow;
				}
			}
		break;
		default: NextState = DbExpectHigh;
	}

	// Perform the output function(s) based on the inputs and current state.

	// Finally, update the current state with the next state.
	Switch->CurrentState = NextState;
	
	return CurrentSwitchReading;
}

rotState updateState(SwitchDefine *Switch,SwitchDefine *aRot,SwitchDefine *bRot)
{

	/*
	 * Below are two charts detailing the state and data transitions of the state machine:
	 *
	 *  Transitions | Input in order Reset,A,B
	 * 	  State     | HHH | HHL | HLH | HLL | LHH | LHL | LLH | LLL
	 * 	  ----------------------------------------------------------
	 * 	  INIT      | INIT| INIT| INIT| INIT| AHBH| AHBL| ALBH| ALBL
	 * 	  ----------------------------------------------------------
	 *    ALBL      | INIT| INIT| INIT| INIT| ALBL| AHBL| ALBH| ALBL
	 * 	  ----------------------------------------------------------
	 *    AHBL      | INIT| INIT| INIT| INIT| AHBH| AHBL| AHBL| ALBL
	 * 	  ----------------------------------------------------------
	 *    ALBH      | INIT| INIT| INIT| INIT| AHBH| AHBL| ALBH| AHBH
	 * 	  ----------------------------------------------------------
	 *    ALBL      | INIT| INIT| INIT| INIT| AHBH| ALBH| ALBH| ALBL
	 *
	 *    edgeCount     | Input in order Reset,A,B
	 * 	  State     | HHH | HHL | HLH | HLL |  LHH  |  LHL  |  LLH  |  LLL
	 * 	  -------------------------------------------------------------------
	 * 	  INIT      |  0  |  0  |  0  |  0  |   0   |   0   |   0   |   0
	 * 	  -------------------------------------------------------------------
	 *    ALBL      |  0  |  0  |  0  |  0  | edgeCount |edgeCount++|edgeCount--| edgeCount
	 * 	  -------------------------------------------------------------------
	 *    AHBL      |  0  |  0  |  0  |  0  |edgeCount++| edgeCount | edgeCount | edgeCount--
	 * 	  -------------------------------------------------------------------
	 *    ALBH      |  0  |  0  |  0  |  0  | edgeCount |edgeCount--|edgeCount++| edgeCount
	 * 	  -------------------------------------------------------------------
	 *    ALBL      |  0  |  0  |  0  |  0  |edgeCount--| edgeCount | edgeCount | edgeCount++
	 *
	 */

	DbState resetState = Switch -> CurrentState;
	DbState AState = aRot -> CurrentState;
	DbState BState = bRot -> CurrentState;

	rotState nextState = INIT;

	switch (currentRotState){
	case INIT:
		if (resetState != DbExpectLow && resetState != DbValidateHigh) { //Reset Low
			if(AState != DbExpectLow && AState != DbValidateHigh){ //A Low
				if(BState != DbExpectLow && BState != DbValidateHigh){ //A Low, B Low
					nextState = ALBL;
					edgeCount = 0;
				}
				else{ //A Low, B High
					nextState = ALBH;
					edgeCount = 0;
				}
			}
			else{ //A High
				if(BState != DbExpectLow && BState != DbValidateHigh){ //A High, B Low
					nextState = AHBL;
					edgeCount = 0;
				}
				else{ //A High, B High
					nextState = AHBH;
					edgeCount = 0;
				}
			}
		}
		else{ //Reset High
			nextState = INIT;
			edgeCount = 0;
		}
		break;
	case ALBL:
		if (resetState != DbExpectLow && resetState != DbValidateHigh) { //Reset Low
			if(AState != DbExpectLow && AState != DbValidateHigh){ //A Low
				if(BState != DbExpectLow && BState != DbValidateHigh){ //A Low, B Low
					nextState = ALBL;
					edgeCount = edgeCount;
				}
				else{ //A Low, B High
					nextState = ALBH;
					edgeCount = edgeCount - 1;
				}
			}
			else{ //A High
				if(BState != DbExpectLow && BState != DbValidateHigh){ //A High, B Low
					nextState = AHBL;
					edgeCount = edgeCount + 1;
				}
				else{ //A High, B High
					nextState = ALBL;
					edgeCount = edgeCount;
				}
			}
		}
		else{ //Reset High
			nextState = INIT;
			edgeCount = 0;
		}
		break;
	case AHBL:
		if (resetState != DbExpectLow && resetState != DbValidateHigh) { //Reset Low
			if(AState != DbExpectLow && AState != DbValidateHigh){ //A Low
				if(BState != DbExpectLow && BState != DbValidateHigh){ //A Low, B Low
					nextState = ALBL;
					edgeCount = edgeCount -1;
				}
				else{ //A Low, B High
					nextState = AHBL;
					edgeCount = edgeCount;
				}
			}
			else{ //A High
				if(BState != DbExpectLow && BState != DbValidateHigh){ //A High, B Low
					nextState = AHBL;
					edgeCount = edgeCount;
				}
				else{ //A High, B High
					nextState = AHBH;
					edgeCount = edgeCount + 1;
				}
			}
		}
		else{ //Reset High
			nextState = INIT;
		}
		break;
	case AHBH:
		if (resetState != DbExpectLow && resetState != DbValidateHigh) { //Reset Low
			if(AState != DbExpectLow && AState != DbValidateHigh){ //A Low
				if(BState != DbExpectLow && BState != DbValidateHigh){ //A Low, B Low
					nextState = AHBH;
					edgeCount = edgeCount;
				}
				else{ //A Low, B High
					nextState = ALBH;
					edgeCount = edgeCount + 1;
				}
			}
			else{ //A High
				if(BState != DbExpectLow && BState != DbValidateHigh){ //A High, B Low
					nextState = AHBL;
					edgeCount = edgeCount - 1;
				}
				else{ //A High, B High
					nextState = AHBH;
					edgeCount = edgeCount;
				}
			}
		}
		else{ //Reset High
			nextState = INIT;
		}
		break;
	case ALBH:
		if (resetState != DbExpectLow && resetState != DbValidateHigh) { //Reset Low
			if(AState != DbExpectLow && AState != DbValidateHigh){ //A Low
				if(BState != DbExpectLow && BState != DbValidateHigh){ //A Low, B Low
					nextState = ALBL;
					edgeCount = edgeCount + 1;
				}
				else{ //A Low, B High
					nextState = ALBH;
					edgeCount = edgeCount;
				}
			}
			else{ //A High
				if(BState != DbExpectLow && BState != DbValidateHigh){ //A High, B Low
					nextState = ALBH;
					edgeCount = edgeCount;
				}
				else{ //A High, B High
					nextState = AHBH;
					edgeCount = edgeCount - 1;
				}
			}
		}
		else{ //Reset High
			nextState = INIT;
		}
		break;
	}

	currentRotState = nextState;

    if (nextState == INIT){
        edgeCount = 0;
        revs = 0;
    }

	return nextState;
}

