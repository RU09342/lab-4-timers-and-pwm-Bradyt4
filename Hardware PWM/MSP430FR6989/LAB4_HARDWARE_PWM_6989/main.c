#include <msp430.h> 
/*
 * Hardware PWM 6989
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
    P9DIR |= BIT7; // Set P9.7 to output
    P1SEL0 |= BIT0; //Select Primary module function
    P1SEL1 &= ~BIT0;

    P1OUT |= BIT1; // Set P1.1 as a high
    P1REN |= BIT1; // Enable pull up resistor on P1.1
    P1IE |= BIT1; // P1.1 interrupt enabled
    P1IES |= BIT1; // P1.1 Hi/lo edge
    P1IFG &= ~BIT1; // P1.1 IFG cleared

    TA1CCTL0 = CCIE; // CCR0 interrupt enabled
    TA1CCR0 = 16384; //resets timer at 16384 cycles

    TA0CCTL1 = OUTMOD_7; //SET/RESET MODE
    TA0CTL = TBSSEL_2 + MC_1; //SMCLK, up mode
    TA0CCR0 = 1000; //Full Cycle
    TA0CCR1 = 500; //Sets the duty cycle to 50%

    _BIS_SR(CPUOFF + GIE); // Enter LPM0 w/ interrupt
    while(1)
    {
    }
}

#pragma vector = TIMER1_A0_VECTOR //Creating timer A0 as an interrupt vector
__interrupt void Timer1_A (void) //Deals with slight button bounce issue while also turning off the light after a set time.
{
    P1IE |= BIT1; //Enables P1.1 interrupt
    TA1CTL = 0x0000; //Disables and resets Timer A0
    P9OUT &= ~BIT7; //Enables P9.7
    P1IFG &= ~BIT1; //Disables interrupt flag
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P9OUT |= BIT7; //Enables P9.7
    if(TA0CCR1 >= 1000)//Logic to increase the duty cycle by 10%
    {
        TA0CCR1 = 0;
    }
    else
    {
        TA0CCR1 += 100;
    }

    TA1CTL = TASSEL__ACLK  | MC__UP; // AClock, Up mode
    P1IE &= ~BIT1; //Disables interrupt on P1.1
    P1IFG &= ~BIT1; //Disables interrupt flag
}

