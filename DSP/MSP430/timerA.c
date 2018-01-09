#include "timerA.h"

extern int samples[NUMBER_OF_SAMPLES];
extern unsigned int CurrentSample;
extern unsigned int CurrentSampleIndex;
extern int CumulativeSum;
extern int Average;
extern int CurrentOutput;

void ConfigureTimerA(void)
{
	// Timer0_A3 Control Register
    TA0CTL = (MC_0 | TACLR);     // Stop the timer and clear it.

	TA0CTL |= TASSEL_2 | ID_0 | MC_1;
	/* TASSEL1 = 0x0200 Timer A clock source select 0
	 * TASSEL0 = 0x0100 Timer A clock source select 1
	 *  00 = TACLK
	 *  01 = ACLK
	 *  10 = SMCLK
	 *  11 = INCLK
	 * ID1 = 0x0080 Timer A clock input divider 1
	 * ID0 = 0x0040 Timer A clock input divider 0
	 *  00 => Input divider = 1
	 *  01 => Input divider = 2
	 *  10 => Input divider = 4
	 *  11 => Input divider = 8
	 * MC1 = 0x0020 Timer A mode control 1
	 * MC0 = 0x0010 Timer A mode control 0
	 *  00 = Stop
	 *  01 = Up
	 *  10 = Continuous
	 *  11 = Up/Down
	 * TACLR = 0x0004 Timer A counter clear
	 * TAIE = 0x0002 Timer A counter interrupt enable
	 * TAIFG = 0x0001 Timer A counter interrupt flag
	 */

	TACCR0 = TACCR0_VALUE;
	TACCR1 = TACCR0_VALUE >> 1;

	// TACCR0 interrupt enabled (see Section 12.2.6 of User's Guide)
	TACCTL0 |= CCIE;
	TACCTL1 |= CCIE;	// Enable TACCR1 Compare/Capture Interrupt Flag (CCIFG1)
}

void filter()
{
	// Moving average filter for ADC samples

	//TOGGLE_DEBUG;

	// To compute the average, add the current sample value to the cumulative sum,
	// and remove the oldest sample (located at samples[CurrentSampleIndex]).
	CumulativeSum += (CurrentSample - samples[CurrentSampleIndex]);

	// Calculate the new average using a bit shift
	Average = CumulativeSum >> NUMBER_OF_BIT_SHIFTS;

	// Update the samples buffer with the current sample.
	samples[CurrentSampleIndex] = CurrentSample;

	/*
	 * Update the index variables.  Essentially, perfoms:
	 *
	 * CurrentSampleIndex = (CurrentSampleIndex + 1) % NUMBER_OF_SAMPLES
	 *
	 * If NUMBER_OF_SAMPLES (see timerA.h) is a power of 2, then use AND operation
	 * (which should be faster) to perform the modulo arithmetic on the sample array index.
	 * Otherwise, use the if statement.
	 */
	CurrentSampleIndex++;
	CurrentSampleIndex &= NUMBER_OF_SAMPLES-1;
	//if (CurrentSampleIndex > (NUMBER_OF_SAMPLES - 1)) CurrentSampleIndex = 0;

	//TOGGLE_DEBUG;
}

// Timer0 A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0(void)
{
	//TOGGLE_DEBUG;

	// The scaling factor is used to adjust any discrepancies between the number of bits produced
	// by the ADC and the number of bits supported by the DAC.
	CurrentOutput = (Average << SCALING_FACTOR);

	ENABLE_DAC_CS;
	SPISendByte((unsigned char) (WRITE_TO_AND_UPDATE_N | DAC_A));
	SPISendByte((unsigned char) (CurrentOutput >> 8)); // Most Significant Byte
	SPISendByte((unsigned char) (CurrentOutput)); // Least Significant Byte
	DISABLE_DAC_CS;

	//TOGGLE_DEBUG;
}

#pragma vector = TIMER0_A1_VECTOR
// Interrupt service routine for CCIFG1 and TAIFG
__interrupt void TimerA1_routine(void)
{
	switch (__even_in_range(TAIV,10)){
	case TA0IV_NONE:
		break;
	case TA0IV_TACCR1: // CCIFG1 interrupt

		//TOGGLE_DEBUG;

		// Read the current sample value from the ADC.
		ADC10CTL0 |= ADC10SC; // Start sampling and conversion
		while (ADC10CTL1 & ADC10BUSY);
		CurrentSample = ADC10MEM;

		filter(); // Function to filer input.

		//TOGGLE_DEBUG;
		break;
	case TA0IV_TAIFG: // TAIFG interrupt
		break;
	default: for (;;); // Should not be possible
	}
}
