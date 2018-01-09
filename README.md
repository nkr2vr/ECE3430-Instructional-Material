# ECE3430 Instructional Material
Written for the development of the ECE 3430 course at University of Virginia

## Boards Used and Development
Many of the projects here were written for the MSP430G2553, the board used currently (Spring 2018) in the introduction to Embedded Systems class. Professor Todd DeLong asked me and another student to migrate some of these projects to the MSP432P401R. Some projects still using the MSP430 were not migrated because they make use of proprietary "shield" boards designed for the MSP430 by Professor Harry C. Powell at the University of Virginia.

The Code Composer Studio v6 by Texas Instruments was used because of its compatibility with TI microprocessors and stability when communicating serially. When creating projects, startup and header files are automatically included for whichever processor was selected. In case one wishes to use an alternate IDE, uploaded here is "msp430.h" for the MSP430G2553 and "msp.h" used with the MSP432P401R. These files were created by Texas Instruments, and are used for educational purposes.

## Honor Code
This code is uploaded to demonstrate prior embedded software knowledge to potential employers. It is not an academic resource for students. If you are a UVA student that has not yet taken ECE3430 or are currently enrolled in it, accessing this repository will be considered a violation of the school's honor policy.
