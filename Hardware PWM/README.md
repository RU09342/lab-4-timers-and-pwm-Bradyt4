# Hardware PWM

## Tyler Brady

## Background
* While a PWM signal can be created using software methods, as explained within the Software PWM README, the 
MSP430 boards do have other, simplier ways at creating PWM signals.

* All the MSP430 boards have the ability to directly output peripherals to their respective pins. Each pin has a
suite of different peripherals attached, and can toggle between general I/O and peripheral output using P(X)SEL or P(X)SEL0
depending on the board being used, where X is replaced by the pin register number, and then toggling which pin would be used
similar to the P(X)DIR.

* All of the MSP430 boards can then output a variety of different functions using the timer register TA0CCTL1, or corresponding register to each
specific board as this register name changes depending on the processor. By setting this register to OUTMOD_7, or what essentially can be called
set/reset function, this make the timer into what essentially is a square wave. This mode essentally tells the processor to output a high until it
hits the specified capture/compare, which typically is CCR1, and then switch to outputing a low until it hits the main capture/compare CCR0.
Since both of these registers are changeable the duty cycle can be modified by changing CCR1. This creates a simple and easy PWM signal. However, this is not to say
that this method is without fault. Since each pin has set peripherals, that means that certain peripherals can only be used on certain pins.
Meaning in some cases, such as the 5529, the output of the peripherals cannot always be set to the onboard LEDs and must be routed off the board, or a jumper to the LED pin.

## Usage
* Once the program is successfully written to the processor, each button press should increase the PWM duty cycle by 10%.
Once the duty cycle has reached 100% the next press will cycle back to 0%

## Devices
* MSP430G2553
* MSP430FR2311
* MSP430FR5994
* MSP430FR6989
* MSP430F5529

## Differences
* For the most part the main differences fall into the different timer setups, and the difference in which pin has the PWM signal outputed.
* The G2553 uses P1.2 and Timer 0
* The F5529 uses P2.0 and Timer 0 and requires a jumper to P1.0 to light the onboard LED
* The FR2311 uses P2.0 and Timer 1
* The FR5994 uses P1.0 and Timer 0
* The FR6989 uses P1.0 and Timer 0


## Extra work
* NONE 