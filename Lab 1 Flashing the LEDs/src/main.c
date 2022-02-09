// Code that flashes Two LEDs at Different Rates
#include <msp430fr6989.h>
#include <stdint.h>

#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7 //Green LED at P9.7

void main(void) {

//volatile unsigned int i;
volatile uint32_t i; // unsigned int 32-bit type

WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Disable GPIO power-on default high impedance mode


P1DIR |= redLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED OFF

P9DIR |= greenLED; // Direct pin as output
P9OUT &= ~greenLED; // Turn LED OFF


for(;;) {

    for(i=0; i<20000; i++) {} //Delay loop
        P1OUT ^= redLED; // Toggle the redLED (ON initially)

        for(i=0; i<20000; i++) {} //Delay loop
                P1OUT ^= redLED; // Toggle the redLED (OFF initially)
                P9OUT ^= greenLED; // Toggle the greenLED (ON initially)

   }
}



// Code that flashes the red LED
//PART 1.2
/*#include <msp430fr6989.h>
#define redLED BIT0 // Red LED at P1.0

void main(void) {

volatile unsigned int i;

WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Disable GPIO power-on default high impedance mode

P1DIR |= redLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off

for(;;) {
// Delay loop
for(i=0; i<20000; i++) {}

P1OUT ^= redLED; // Toggle the LED

   }
}*/


//PART 1.3
// Code that flashes the red LED with large delay
/*#include <stdint.h>
#include <msp430fr6989.h>
#define redLED BIT0 // Red LED at P1.0

void main(void) {

volatile uint32_t i; //unsigned int of 32 bit type

WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Disable GPIO power-on default high impedance mode

P1DIR |= redLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off

for(;;) {
// Delay loop
for(i=0; i<120000; i++) {}

P1OUT ^= redLED; // Toggle the LED

   }
}*/

/*
//PART 1.4 Flashing two LEDs out of Sync
#include <stdint.h>
#include <msp430fr6989.h>
#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7

void main(void) {

volatile uint32_t i; //unsigned int of 32 bit type

WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Disable GPIO power-on default high impedance mode

P1DIR |= redLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off

P9DIR |= greenLED; //Direct pin as output
//P9OUT |= greenLED;//Turn LED ON with OR to set 1

P9OUT&= ~greenLED;

for(;;) {
// Delay loop
for(i=0; i<80000; i++) {}

P1OUT ^= redLED; // Toggle the LED
P9OUT ^= greenLED; //Toggle the LED

   }
}*/
