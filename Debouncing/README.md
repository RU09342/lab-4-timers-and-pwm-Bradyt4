# Software Debouncing

## Tyler Brady

## Background
* Most mechanial devices typically come with some form of a downside. This is no different in phyiscal switches like buttons. The manner in which the buttons located on the MSP430's
work is by acting a short to ground when pressed, meaning theyre high when open and low when pressed. When a switch is released the spring used to push the button away oscillates slightly
and while this may not be a problem for most uses of a button, in a extreamly fast processor this small oscillation can become a massive issue. Each time the button osscilates
to ground the circuit the processor recognizes a change. When using interrupts, that detect this state change and trigger a function, this can be an issue as the interrupt could be trigger
more than intended. In order to solve this a method called "debouncing" must be used. This essentially boils down to disabling interrupts from being called by turning them off for a period of time
after the initial trigger. Typically this period of off time is so short it has no effect on the use of the button to the user. In this project the button was debounced by using a timer
to turn on the interrupt after approximatly half a second. One other method used to ensure that no interrupts managed to get through is to clear both before leaving the interrupt and after
the timer's interrupt has fired.
## Usage
* Once the board specific code has been loaded onto one of the MSP430s, the designated button can be pressed to toggle the onboard LED.
## Devices
* MSP430G2553
* MSP430FR2311
* MSP430FR5994
* MSP430FR6989
* MSP430F5529

## Differences
* For the most part the code between projects is highly similar, only really changing in regards to the differnt ports, macros, and required setup such as the 2311 only having a timer B.

## Extra work
* NONE 