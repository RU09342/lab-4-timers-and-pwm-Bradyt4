#include <msp430.h> 
/*
 * Software PWM 5529.
 *
 *  Created on: Oct 10, 2017
 *  Last Edited: Oct 10, 2017
 *  Author: Tyler Brady
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR |= BIT0; // Set P1.0 to output direction
    P4DIR |= BIT7; // Set P4.7 to output

    P1OUT |= BIT1; // Set P1.1 as a high
    P1REN |= BIT1; // Enable pull up resistor on P1.1
    P1IE |= BIT1; // P1.1 interrupt enabled
    P1IES |= BIT1; // P1.1 Hi/lo edge
    P1IFG &= ~BIT1; // P1.1 IFG cleared

    TA0CCTL0 = CCIE; // CCR0 interrupt enabled
    TA0CCR0 = 16384; //resets timer at 16384 cycles

    TA1CCTL1 = CCIE; //Enable interrupt in compare mode
    TA1CCTL0 = CCIE; //Enable interrupt in compare mode
    TA1CTL = TBSSEL_2 + MC_1; //SMCLK, up mode
    TA1CCR0 = 1000; //Full Cycle
    TA1CCR1 = 500; //Sets the duty cycle to 50%

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

//Timer A1 interrupt vectors: control PWM LED
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
