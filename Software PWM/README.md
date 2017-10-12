# Software PWM

## Tyler Brady

## Background
* Pulse width modulation is the method of sending a signal with a specific frequency, however with a slight difference, as
the time at which the signal is high is different. This percentage of time high vs low is call the duty cycle.
Using this method its possible to make an LED look to be dimmer at lower duty cycles and higher frequencies.
This may sound complicated, but the actual implementation is quite simple. Since in the MSP430 timers, each timer has access to more than one 
capture/compare register, it is possible to set up a timer to trigger more than once with two different interrupts.
Using this method one can set up two timer interrupts, one which has a changing trigger, and one which always triggers a 1k clock cycles.
The first interrupt will toggle the LED off, while the second will toggle the LED back on.

* Then by adding in a debounced button interrupt it is possible to change the point at which the first interrupt fires by 100 cycles, changing the
duty cycle by 10%. This has the offhanded affect of making the brightness of the LED increase as the button is pressed.

## Usage
* The code for this function has been coded for all five MSP430 boards and once loaded onto the board can be demonstrated by simply pressing the 
designated button.

## Devices
* MSP430G2553
* MSP430FR2311
* MSP430FR5994
* MSP430FR6989
* MSP430F5529

## Differences
* The differences between each board is mostly just differences between pins on boards and timer setup/macros. Otherwise the only other big difference
is the 2311's lack of a timer A.

## Extra work
* NONE 