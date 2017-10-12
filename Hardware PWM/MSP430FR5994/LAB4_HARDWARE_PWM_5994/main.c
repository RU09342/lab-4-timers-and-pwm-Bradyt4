#include <msp430.h> 

/*
 * HARDWARE PWM 5994
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

    P1DIR |= BIT0 + BIT1; // Set P1.0 and 1.1 to output direction
    P1SEL0 |= BIT0; //Selects primary peripheral module function
    P1SEL1 &= ~BIT0;

    P5OUT |= BIT6; // Set P5.6 as a high
    P5REN |= BIT6; // Enable pull up resistor on P5.6
    P5IE |= BIT6; // P5.6 interrupt enabled
    P5IES |= BIT6; // P5.6 Hi/lo edge
    P5IFG &= ~BIT6; // P5.6 IFG cleared

    TA1CCTL0 = CCIE; // CCR0 interrupt enabled
    TA1CCR0 = 16384; //Sets timer to about half a second to be safe

    TA0CCTL1 = OUTMOD_7; //SET/RESET MODE
    TA0CTL = TBSSEL_2 + MC_1; //SMCLK, up mode
    TA0CCR0 = 1000; //Full Duty cycle
    TA0CCR1 = 500; //

    _BIS_SR(CPUOFF + GIE); // Enter LPM0 w/ interrupt
    while(1)
    {
    }
}

#pragma vector = TIMER1_A0_VECTOR //Creating timer A0 as an interrupt vector
__interrupt void Timer1_A (void) //Re-enables the button interrupt and clears the clock for next input
{
    P5IE |= BIT6; //Enables P5.6 interrupt
    TA1CTL = 0x0000; //Disables and resets Timer A0
    P5IFG &= ~BIT6; //Disables interrupt flag
    P1OUT &= ~BIT1; //Enables P1.1
}

#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)
{
    P1OUT |= BIT1; //Enables P1.1
    if(TA0CCR1 >= 1000)//Logic to increase the duty cycle by 10%
    {
        TA0CCR1 = 0;
    }
    else
    {
        TA0CCR1 += 100;
    }
    TA1CTL = TASSEL__ACLK  | MC__UP; // AClock, Up mode
    P5IE &= ~BIT6; //Disables interrupt on P5.6
    P5IFG &= ~BIT6; //Disables interrupt flag

}

