#include <msp430.h>
/*
 * Software PWM 2553
 *
 *  Created on: Oct 10, 2017
 *  Last Edited: Oct 10, 2017
 *  Author: Tyler Brady
 */
 int main(void) {
    WDTCTL = WDTPW + WDTHOLD; //Disable the Watchdog timer.

    P1DIR |= BIT6 + BIT0; //Set pin 1.6  and 1.0 to the output direction.

    //Debounced Button
    P1OUT |= BIT3; //Set pin 1.3 to a 1
    P1REN |= BIT3; //Enable a pull up resistor on 1.3

    P1IE |= BIT3; // P1.1 interrupt enabled
    P1IES |= BIT3; // P1.1 Hi/lo edge
    P1IFG &= ~BIT3; // P1.1 IFG cleared

    //TIMER 1: Debounce control
    TA1CCTL0 = CCIE; // CCR0 interrupt enabled
    TA1CCR0 = 16384; //resets timer at 10000 cycles

    TA0CCTL1 = CCIE; //Enable interrupt in compare mode
    TA0CCTL0 = CCIE; //Enable interrupt in compare mode
    TA0CCR0 = 1000; //Set the period in the Timer A0 Capture/Compare 0 register to 1000 Microseconds.
    TA0CCR1 = 500; //The period in microseconds that the power is ON. It's half the time, which translates to a 50% duty cycle.
    TA0CTL = TASSEL_2 + MC_1; //TASSEL_2 selects SMCLK as the clock source, and MC_1 tells it to count up to the value in TA0CCR0.

    __bis_SR_register(CPUOFF + GIE); //Switch to low power mode 0.
}


#pragma vector = TIMER1_A0_VECTOR //Creating timer A0 as an interrupt vector
__interrupt void Timer1_A (void)//Deals with slight button bounce issue while also turning off the light after a set time.
{
    P1IE |= BIT3; //Enables P1.3 interrupt
    P1OUT &= ~BIT0;
    TA1CTL = 0x0000; //Disables and resets Timer A0
    P1IFG &= ~BIT3; //Disables interrupt flag
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

#pragma vector=TIMER0_A1_VECTOR //Disables the LED once the time has reached the duty cycle %
__interrupt void Timer0_A1 (void)
{
    if(TA0CCR1 != 1000)//Ensures that the duty cycle isnt 100% and disables the Green led for the remaining stage time
    {
       P1OUT &= ~(BIT6); //turns off Green led
    }
    TA0CCTL1 &= ~BIT0; //clears flag
}

#pragma vector=TIMER0_A0_VECTOR //Re-enables the LED at the end of the cycle
__interrupt void Timer0_A0 (void)
{
    P1OUT |= (BIT6); //turns on Green led
    TA0CCTL0 &= ~BIT0;  //clears flag
}
