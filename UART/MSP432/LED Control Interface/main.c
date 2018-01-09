//
// This program presents a simple User Interface (UI) for controlling the LEDs.
//
#include "msp.h"
#include <stdio.h>

#include "uart.h"
#include "LED.h"
#include "timerA.h"

#define NUMBER_OF_BLINKS 10
#define BUFFER_SIZE 80
#define TRUE 1
#define FALSE 0
#define UART 0

// Global variable
char char_has_not_been_received;
char receive_char;

//For a delay of 250ms with 8MHz clock,
int delay = 250;
int resolution = 1000;
int i;

// Function prototypes
void ConfigureClockModule(void);
void PrintString(char *, unsigned char);

void main(void)
{
    unsigned char PrintDevice;

    // Stop the watchdog timer, and configure the clock module.
    WDTCTL = WDTPW + WDTHOLD;
    ConfigureClockModule();

    // Initialize LEDs
    InitializeLEDPortPins();

    // Initialize USCI A0 to use UART.
    InitializeUART();
    PrintDevice = UART;

    // Initialize receive_char, and continue with the UI until the user chooses to exit.
    receive_char = (char) NULL;
    while ((receive_char != 'x') && (receive_char != 'X'))
    {

        // "Clear" the screen with a carriage return ('\r') and line feed ('\n'),
        // followed by the prompt string.
        PrintString("\r\nEnter a command - (1) Turn on LED (2) Turn off LED (3) Blink LED (x) Exit: ",PrintDevice);

        PrintString("\r\nInput Character: ", PrintDevice);
        // Wait for a character to be entered.
        char_has_not_been_received = TRUE;
        while (char_has_not_been_received);

        // Now, process the command
        switch (receive_char)
        {
        case '1':
            PrintString("\r\nTurn on which LED? (1) Red (2) Green: ",PrintDevice);

            // Wait for a character to be entered.
            char_has_not_been_received = TRUE;
            while (char_has_not_been_received);

            switch (receive_char)
            {
            case '1':
                PrintString("\r\nRed LED is on now.",PrintDevice);
                TURN_ON_LED1;
                break;
            case '2':
                PrintString("\r\nGreen LED is on now.",PrintDevice);
                TURN_ON_LED2;
                break;
            default:
                PrintString("\r\nError! Incorrect choice. ",PrintDevice);
                break;
            }
            break;
        case '2':
            PrintString("\r\nTurning off LEDs...",PrintDevice);
            TURN_OFF_LED1;
            TURN_OFF_LED2;
            break;
        case '3':
            PrintString("\r\nBlink which LED? (1) Red (2) Green: ",PrintDevice);

            // Wait for a character to be entered.
            char_has_not_been_received = TRUE;
            while (char_has_not_been_received);

            switch (receive_char)
            {
            case '1':
                PrintString("\r\nRed LED is blinking now.",PrintDevice);
                for (i = 0; i < NUMBER_OF_BLINKS; i++)
                {
                    TOGGLE_LED1;
                    TimeDelay(delay, resolution);
                }
                break;
            case '2':
                PrintString("\r\nGreen LED is blinking now.",PrintDevice);
                for (i = 0; i < NUMBER_OF_BLINKS; i++)
                {
                    TOGGLE_LED2;
                    TimeDelay(delay, resolution);
                }
                break;
            default:
                PrintString("\r\nError! Incorrect choice. ", PrintDevice);
                break;
            }
            break;
        case 'x':
        case 'X':
            PrintString("\r\nExiting...",PrintDevice);
            TURN_OFF_LED1;
            TURN_OFF_LED2;
            break;
        default:
            PrintString("\r\nError! Incorrect choice. ",PrintDevice);
            break;
        }
    }
}

void PrintString(char * StringToPrint, unsigned char PrintDeviceType)
{
    unsigned int i;                     // Used for indexing
    char StringBuffer[BUFFER_SIZE]; // Character buffer to hold each string to be printed

    for (i = 0; i < BUFFER_SIZE; i++)
        StringBuffer[i] = (char) NULL;
    sprintf(StringBuffer, StringToPrint);
    switch (PrintDeviceType)
    {
    case UART:
        WriteStringUART(StringBuffer);
        break;
    default:
        break;
    }
}

void ConfigureClockModule(void)
{
    CS->KEY = CS_KEY_VAL;
    //Reset parameters
    CS->CTL0 = 0;
    CS->CTL1 = 0;
    CS->CTL2 = 0;
    CS->CTL3 = 0;

    /**
     * Configure DCO for ~8MHz
     * CS_CTL0_DCOEN         Set high to enable DCO
     * CS_CTL0_DCORSEL_2     nominal 6MHz, range 4 to 8Mhz
     * CS_CTL0_DCOTUNE_MASK  10-bit, 2's complement frequency offset
     * See formula in section 5.2.8.3 for DCOTUNE value
     */
    signed int N = 140;
    CS->CTL0 = CS_CTL0_DCOEN | CS_CTL0_DCORSEL_2 | (CS_CTL0_DCOTUNE_MASK & N);

    //Set DCO as source for SMCLK
    CS->CTL1 = CS_CTL1_SELS__DCOCLK;

    //Enable SMCLK
    CS->CLKEN = CS_CLKEN_SMCLK_EN;

    //Lock to prevent further changes.
    CS->KEY = 0;
}

