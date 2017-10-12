#include <msp430.h> 
/*
 * Button debounce 6989
 *
 *  Created on: Oct 10, 2017
 *  Last Edited: Oct 10, 2017
 *  Author: Tyler Brady
 */


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= BIT0; // Set P1.0 to output direction
    P1OUT |= BIT1; // Set P1.1 as a high
    P1REN |= BIT1; // Enable pull up resistor on P1.1

    P1IE |= BIT1; // P1.1 interrupt enabled
    P1IES |= BIT1; // P1.1 Hi/lo edge
    P1IFG &= ~BIT1; // P1.1 IFG cleared

    TA0CCTL0 = CCIE; // CCR0 interrupt enabled
    TA0CCR0 = 16384; //resets timer at 16834 cycles

    _BIS_SR(CPUOFF + GIE); // Enter LPM0 w/ interrupt
    while(1)
    {
    }
}

#pragma vector = TIMER0_A0_VECTOR //Creating timer A0 as an interrupt vector
__interrupt void Timer0_A (void) //Deals with slight button bounce issue while also turning off the light after a set time.
{
    P1IE |= BIT1; //Enables P1.1 interrupt
    TA0CTL = 0x0000; //Disables and resets Timer A0
    P1IFG &= ~BIT1; //Disables interrupt flag
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    TA0CTL = TASSEL__ACLK  | MC__UP; // AClock, Up mode
    P1IE &= ~BIT1; //Disables interrupt on P1.1
    P1IFG &= ~BIT1; //Disables interrupt flag

    P1OUT ^= BIT0; //Enables P1.0
}
