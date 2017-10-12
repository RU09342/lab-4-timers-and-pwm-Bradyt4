#include <msp430.h> 
/*
 * HARDWARE PWM 5529.
 *
 *  Created on: Oct 10, 2017
 *  Last Edited: Oct 10, 2017
 *  Author: Tyler Brady
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P2DIR |= BIT0; // Set P2.0 to output direction
    P4DIR |= BIT7; // Set P4.7 to output
    P2SEL |= BIT0; // Outputs PWM to P2.0,  must then connect to 1.0.

    P1OUT |= BIT1; // Set P1.1 as a high
    P1REN |= BIT1; // Enable pull up resistor on P1.1
    P1IE |= BIT1; // P1.1 interrupt enabled
    P1IES |= BIT1; // P1.1 Hi/lo edge
    P1IFG &= ~BIT1; // P1.1 IFG cleared

    TA1CCTL0 = CCIE; // CCR0 interrupt enabled
    TA1CCR0 = 16384; //resets timer at 16384 cycles

    TA0CCTL1 = OUTMOD_7;//SET/RESET MODE
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
    TA0CTL = 0x0000; //Disables and resets Timer A0
    P1IFG &= ~BIT1; //Disables interrupt flag
    P4OUT &= ~BIT7; //Disables P4.7
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P4OUT |= BIT7; //Enables P4.7
    if(TA1CCR1 >= 1000)//Logic to increase the duty cycle by 10%
    {
        TA1CCR1 = 0;
    }
    else
    {
        TA1CCR1 += 100;
    }
    TA0CTL = TASSEL_1 | MC__UP; // AClock, Up mode
    P1IE &= ~BIT1; //Disables interrupt on P5.6
    P1IFG &= ~BIT1; //Disables interrupt flag

    P1OUT |= BIT0; //Enables P1.0
}

