//FUNCTIONS PROTOTYPES
void config_ACLK_to_32KHz_crystal();

//MAIN
//PART 3.1
// Flashing the LED with Timer_A, continuous mode, via polling
#include <msp430fr6989.h>
#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7 // Green LED at P9.7

void main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

P1DIR |= redLED; // Direct pin as output
P9DIR |= greenLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off
P9OUT &= ~greenLED; // Turn LED Off

// Configure ACLK to the 32 KHz crystal (function call)
config_ACLK_to_32KHz_crystal();

// Configure Timer_A
// Use ACLK, divide by 1, continuous mode, clear TAR
TA0CTL = TASSEL_1|MC_2|TACLR|ID_0; //ID_0 divide by 1

// Ensure flag is cleared at the start
TA0CTL &= ~TAIFG;

// Infinite loop
for(;;) {
// Empty while loop; waits here until TAIFG is raised
while((TA0CTL & TAIFG) == 0) {}
P1OUT^=redLED;// Toggle the red LED
TA0CTL &=~TAIFG;// Clear the flag
    }
}

//PART 3.2
#include <msp430fr6989.h>
#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7 // Green LED at P9.7
//MAIN
// Flashing the LED with Timer_A, Up mode, via polling
void main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

P1DIR |= redLED; // Direct pin as output
P9DIR |= greenLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off
P9OUT &= ~greenLED; // Turn LED Off

// Configure ACLK to the 32 KHz crystal (function call)
config_ACLK_to_32KHz_crystal();

// Configure Timer_A
// Set timer period
TA0CCR0 = (32768-1); //32768 = 1 sec delay

// Use ACLK, divide by 1, up mode, clear TAR
TA0CTL = TASSEL_1|ID_0|MC_1|TACLR;

// Ensure flag is cleared at the start
TA0CTL &= ~TAIFG;

// Infinite loop
for(;;) {
// Empty while loop; waits here until TAIFG is raised
while((TA0CTL & TAIFG) == 0) {}
P1OUT^=redLED;// Toggle the red LED
TA0CTL &=~TAIFG;// Clear the flag
    }
}

//Part 3.3 Continuous Mode
//Using continuous Mode when you pressed a button the red led will blink with a delay of 2 second
/*#include <msp430fr6989.h>
#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7 // Green LED at P9.7
#define BUT1 BIT1 // Button S1 at P1.1
#define BUT2 BIT2 //Button S2 at P1.2

void main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

//Configure LEDs
P1DIR |= redLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off

//Configure buttons
P1DIR &= ~BUT1; // Direct pin as input   //pushing button is input == 0    //set for P1 Bit 1
P1REN |= BUT1; // Enable built-in resistor
P1OUT |= BUT1; // Set resistor as pull-up

// Configure ACLK to the 32 KHz crystal (function call)
config_ACLK_to_32KHz_crystal();

// Configure Timer_A
// Use ACLK, divide by 1, continuous mode, clear TAR
TA0CTL = TASSEL_1|MC_2|TACLR|ID_0; //divide frequency by 2

// Ensure flag is cleared at the start
TA0CTL &= ~TAIFG;

// Infinite loop
for(;;) {
    if ((P1IN &=BUT1)== 0){
// Empty while loop; waits here until TAIFG is raised
while((TA0CTL & TAIFG) == 0) {}       // There will be a period of 2 seconds
P1OUT^=redLED;// Toggle the red LED
TA0CTL &=~TAIFG;// Clear the flag
       }
    else P1OUT &= ~redLED; // Turn red LED off
    }
}*/

#include <msp430fr6989.h>
#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7 // Green LED at P9.7
#define BUT1 BIT1 // Button S1 at P1.1
#define BUT2 BIT2 //Button S2 at P1.2
//Part 3.3 Up Mode Experiment
//Using Up Mode when a button is pressed red and green led will flash with a delay of 1 second
void main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

P1DIR |= redLED; // Direct pin as output
P9DIR |= greenLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off
P9OUT &= ~greenLED; // Turn LED Off

//Configure buttons
P1DIR &= ~BUT1; // Direct pin as input   //pushing button is input == 0    //set for P1 Bit 1
P1REN |= BUT1; // Enable built-in resistor
P1OUT |= BUT1; // Set resistor as pull-up

//CONFIG BUTTON 2
P1DIR &= ~BUT2; // Direct pin as input   //pushing button is input == 0    //set for P1 Bit 1
P1REN |= BUT2; // Enable built-in resistor
P1OUT |= BUT2; // Set resistor as pull-up

// Configure ACLK to the 32 KHz crystal (function call)
config_ACLK_to_32KHz_crystal();

// Configure Timer_A
// Set timer period
TA0CCR0 = (65536-1); //Timer period is 2 second and flashing period is 4 seconds

// Use ACLK, divide by 1, up mode, clear TAR
TA0CTL = TASSEL_1|ID_0|MC_1|TACLR;

// Ensure flag is cleared at the start
TA0CTL &= ~TAIFG;

// Infinite loop
for(;;) {

    if ((P1IN &=BUT2)== 0){
// Empty while loop; waits here until TAIFG is raised
while((TA0CTL & TAIFG) == 0) {}
P1OUT^=redLED;// Toggle the red LED
P9OUT^=greenLED;// Toggle the green LED
TA0CTL &=~TAIFG;// Clear the flag
      }
   else{
   P1OUT &= ~redLED; // Turn red LED off
   P9OUT &= ~greenLED; // Turn green LED off
   }
    }
}

//FUNCTIONS
//**********************************
// Configures ACLK to 32 KHz crystal
void config_ACLK_to_32KHz_crystal() {
// By default, ACLK runs on LFMODCLK at 5MHz/128 = 39 KHz
// Reroute pins to LFXIN/LFXOUT functionality

PJSEL1 &= ~BIT4;
PJSEL0 |= BIT4;

// Wait until the oscillator fault flags remain cleared
CSCTL0 = CSKEY; // Unlock CS registers

do {
CSCTL5 &= ~LFXTOFFG; // Local fault flag
SFRIFG1 &= ~OFIFG; // Global fault flag
} while((CSCTL5 & LFXTOFFG) != 0);

CSCTL0_H = 0; // Lock CS registers

return;
}
