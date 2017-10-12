#include <msp430.h> 
/*
 * Button debounce 2553
 *
 *  Created on: Oct 10, 2017
 *  Last Edited: Oct 10, 2017
 *      Cleaned up and fixed some comments
 *  Author: Tyler Brady
 */

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer


    P1DIR |= BIT0; // Set P1.0 to output direction
    P1OUT |= BIT3; // Set P1.3 as a high
    P1REN |= BIT3; // Enable pull up resistor on P1.3

    P1IE |= BIT3; // P1.3 interrupt enabled
    P1IES |= BIT3; // P1.3 Hi/lo edge
    P1IFG &= ~BIT3; // P1.3 IFG cleared

    TA0CCTL0 = CCIE; // CCR0 interrupt enabled
    TA0CCR0 = 16384; //resets timer at 10000 cycles

    _BIS_SR(CPUOFF + GIE); // Enter LPM0 w/ interrupt
    while(1)
    {
    }
}

#pragma vector = TIMER0_A0_VECTOR //Creating timer A0 as an interrupt vector
__interrupt void Timer0_A (void)//Deals with slight button bounce issue while also turning off the light after a set time.
{
    P1IE |= BIT3; //Enables P1.3 interrupt
    TA0CTL = 0x0000; //Disables and resets Timer A0
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    TA0CTL = TASSEL_1 | MC_1; // AClock, Up mode
    P1IE &= ~BIT3; //Disables interrupt on P1.3
    P1IFG &= ~BIT3; //Disables interrupt flag

    P1OUT ^= BIT0; //Enables P1.0
}

