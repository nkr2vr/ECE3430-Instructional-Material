#include "timerA.h"

void TimeDelay(unsigned int delay, unsigned int resolution)
{
	unsigned int i;

	/* Timer A1 Control Register
	 * TASSEL1 = 0x0200 Timer A clock source select 0
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

	// Stop the timer and clear register
	TA1CTL = (MC_0 | TACLR);

	// Update the value of Compare-Capture Register 0.
	TA1CCR0 = resolution;

	// Restart the timer.
	TA1CTL = (TASSEL_2 + ID_0 + MC_1);

	for (i = 0; i < delay; i++) {

		// Wait for the timer to overflow
		while (!(TA1CTL & TAIFG));

		// Reset the TAIFG.
		TA1CTL &= ~TAIFG;
	}

	// Stop the timer and clear register
	TA1CTL = (MC_0 | TACLR);
}
