#include "DAC.h"

void InitializeDACPins(void)
{
	DISABLE_DAC_CS;
	SET_DAC_CS_AS_AN_OUTPUT;

	DISABLE_DAC_CLEAR;
	SET_DAC_CLEAR_AS_AN_OUTPUT;

}

void InitDAC(DAC *Dac, int DACNum)
{
	Dac->DACAddress = DACNum;
	Dac->DACValue = 0;
}

