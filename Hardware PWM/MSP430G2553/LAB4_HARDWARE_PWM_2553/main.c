#include <msp430.h>
/*
 * Hardware PWM 2553
 *
 *  Created on: Oct 10, 2017
 *  Last Edited: Oct 10, 2017
 *  Author: Tyler Brady
 */
 int main(void) {
    WDTCTL = WDTPW + WDTHOLD; //Disable the Watchdog timer.

    P1DIR |= BIT6 + BIT0; //Set pin 1.6  and 1.0 to the output direction.
    P1SEL |= BIT6; //Select pin 1.6 as PWM output.

    //Debounced Button
    P1OUT |= BIT3; //Set pin 1.3 to a 1
    P1REN |= BIT3; //Enable a pull up resistor on 1.3

    P1IE |= BIT3; // P1.1 interrupt enabled
    P1IES |= BIT3; // P1.1 Hi/lo edge
    P1IFG &= ~BIT3; // P1.1 IFG cleared

    //TIMER 1: Debounce control
    TA1CCTL0 = CCIE; // CCR0 interrupt enabled
    TA1CCR0 = 16384; //resets timer at 16384 cycles

    TA0CCR0 = 1000; //Set the period in the Timer A0 Capture/Compare 0 register to 1000 Microseconds.
    TA0CCTL1 = OUTMOD_7;//Sets the ouput modification to Set/Reset where output is set high until TA0CCR1 and reset to low.
    TA0CCR1 = 100; //The period in microseconds that the power is ON. It's half the time, which translates to a 10% duty cycle.
    TA0CTL = TASSEL_2 + MC_1; //TASSEL_2 selects SMCLK as the clock source, and MC_1 tells it to count up to the value in TA0CCR0.

    __bis_SR_register(CPUOFF + GIE); //Switch to low power mode 0.
}


#pragma vector = TIMER1_A0_VECTOR //Creating timer A0 as an interrupt vector
__interrupt void Timer1_A (void)//Deals with slight button bounce issue while also turning off the light after a set time.
{
    P1IE |= BIT3; //Enables P1.3 interrupt
    P1OUT &= ~BIT0; //Disables P1.0
    TA1CTL = 0x0000; //Disables and resets Timer A0
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    TA1CTL = TASSEL_1 | MC_1; // AClock, Up mode
    P1IE &= ~BIT3; //Disables interrupt on P1.3
    P1IFG &= ~BIT3; //Disables interrupt flag
    P1OUT |= BIT0;
    if(TA0CCR1 <= 1000)
    {
        TA0CCR1 += 100;
    }
    else
    {
        TA0CCR1 = 0;
    }
}
