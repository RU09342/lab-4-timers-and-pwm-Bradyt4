#include <msp430.h>
/*
 * Hardware PWM 2311
 *
 *  Created on: Oct 10, 2017
 *  Last Edited: Oct 11, 2017
 *      Author: Tyler Brady
 */

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= BIT0; // Set P1.0 to output direction
    P2DIR |= BIT0; // Set P2.0 to output
    P2SEL0 |= BIT0;
    P2SEL1 &= ~BIT0;

    P1DIR &= ~(BIT1);
    P1OUT |= BIT1; // Set P1.1 as a high
    P1REN |= BIT1; // Enable pull up resistor on P1.1
    P1IE |= BIT1;  // P1.1 interrupt enabled
    P1IES |= BIT1; // P1.1 Hi/lo edge
    P1IFG &= ~(BIT1);// P1.1 IFG cleared

    TB0CCTL0 = CCIE; // CCR0 interrupt enabled
    TB0CCR0 = 25000 - 1; //resets timer 32768/25000

    TB1CCTL1 = OUTMOD_7; //SET/RESET MODE
    TB1CTL = TBSSEL_2 + MC_1; //SMCLK, up mode
    TB1CCR0 = 1000;
    TB1CCR1 = 500;

    _BIS_SR(CPUOFF + GIE); // Enter LPM0 w/ interrupt
    while(1)
    {
    }
}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1OUT |= BIT0;
    if(TB1CCR1 >= 1000)//Logic to increase the duty cycle by 10%
    {
        TB1CCR1 = 0;
    }
    else
    {
        TB1CCR1 += 100;
    }

    TB0CTL = TBSSEL__ACLK  | MC__UP; // AClock, Up mode
    P1IE &= ~BIT1; //Disables interrupt on P1.1
    P1IFG &= ~BIT1; //Disables interrupt flag
}

#pragma vector = TIMER0_B0_VECTOR //Creating timer A0 as an interrupt vector
__interrupt void Timer0_B (void)//Deals with slight button bounce issue while also turning off the light after a set time.
{
    P1OUT &= ~BIT0; //disables P1.0
    P1IFG &= ~BIT1; //Disables interrupt flag
    P1IE |= BIT1; //Enables P1.1 interrupt
    TB0CTL = 0x0000; //Disables and resets Timer A0
}

