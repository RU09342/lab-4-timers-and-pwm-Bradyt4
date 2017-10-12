#include <msp430.h> 

/*
 * Software PWM 5994
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
    P1OUT &= ~BIT1;

    P5OUT |= BIT6; // Set P5.6 as a high
    P5REN |= BIT6; // Enable pull up resistor on P5.6
    P5IE |= BIT6; // P5.6 interrupt enabled
    P5IES |= BIT6; // P5.6 Hi/lo edge
    P5IFG &= ~BIT6; // P5.6 IFG cleared

    TA0CCTL0 = CCIE; // CCR0 interrupt enabled
    TA0CCR0 = 16384; //Sets timer to about half a second to be safe

    TA1CCTL1 = CCIE; //Enable interrupt in compare mode
    TA1CCTL0 = CCIE; //Enable interrupt in compare mode
    TA1CTL = TBSSEL_2 + MC_1; //SMCLK, up mode
    TA1CCR0 = 1000; //Full Duty cycle
    TA1CCR1 = 500; //

    _BIS_SR(CPUOFF + GIE); // Enter LPM0 w/ interrupt
    while(1)
    {
    }
}

#pragma vector = TIMER0_A0_VECTOR //Creating timer A0 as an interrupt vector
__interrupt void Timer0_A (void) //Re-enables the button interrupt and clears the clock for next input
{
    P5IE |= BIT6; //Enables P5.6 interrupt
    TA0CTL = 0x0000; //Disables and resets Timer A0
    P5IFG &= ~BIT6; //Disables interrupt flag
    P1OUT &= ~BIT1; //Enables P1.1
}

#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)
{
    P1OUT |= BIT1; //Enables P1.1
    if(TA1CCR1 >= 1000)//Logic to increase the duty cycle by 10%
    {
        TA1CCR1 = 0;
    }
    else
    {
        TA1CCR1 += 100;
    }
    TA0CTL = TASSEL__ACLK  | MC__UP; // AClock, Up mode
    P5IE &= ~BIT6; //Disables interrupt on P5.6
    P5IFG &= ~BIT6; //Disables interrupt flag

}

//Timer A1 interrupt vectors
#pragma vector=TIMER1_A1_VECTOR //Disables the LED once the time has reached the duty cycle %
__interrupt void Timer1_A1 (void)
{
    if(TA1CCR1 != 1000)//Ensures that the duty cycle isnt 100% and disables the Red led for the remaining stage time
    {
       P1OUT &= ~(BIT0); //turns off red led
    }
    TA1CCTL1 &= ~BIT0; //clears flag
}

#pragma vector=TIMER1_A0_VECTOR //Re-enables the LED at the end of the cycle
__interrupt void Timer1_A0 (void)
{
    P1OUT |= (BIT0); //turns on red led
    TA1CCTL0 &= ~BIT0;  //clears flag
}

