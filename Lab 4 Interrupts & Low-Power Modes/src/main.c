//PROTOTYPES
void config_ACLK_to_32KHz_crystal();

// Timer_A continuous mode, with interrupt, flashes LEDs ///ROLLBACK TO 0
#include <msp430fr6989.h>
#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7 // Green LED at P9.7

#define BUT1 BIT1 // Button S1 at Port 1.1
#define BUT2 BIT2 // Button S2 at Port 1.2

//MAIN
void main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

//Configures LEDs
P1DIR |= redLED; // Direct pin as output
P9DIR |= greenLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off
P9OUT &= ~greenLED; // Turn LED Off

// Configure ACLK to the 32 KHz crystal
config_ACLK_to_32KHz_crystal();

// Timer_A configuration (fill the line below)
// Use ACLK, divide by 1, continuous mode, TAR cleared, enable interrupt for rollback-to-zero event
TA0CTL = TASSEL_1|ID_0|MC_2|TACLR|TAIE; //The interrupt was enabled for Timer A 0

// Ensure the flag is cleared at the start
TA0CTL &= ~TAIFG;

// Enable the global interrupt bit (call an intrinsic function)
_enable_interrupts();

// Infinite loop... the code waits here between interrupts
for(;;) {}
}

//ISR
#pragma vector = TIMER0_A1_VECTOR // Link the ISR to the vector
__interrupt void T0A1_ISR() {  //can it also be TA0_A1_ISR()????
// Toggle both LEDs
    P1OUT ^= redLED;
    P9OUT ^= greenLED;
// Clear the TAIFG flag
    TA0CTL &= ~TAIFG;
}

//Part 4.2 Interrupt UP MODE
// Timer_A up mode, with interrupt, flashes LEDs

void main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

//Configuring LEDs
P1DIR |= redLED; // Direct pin as output
P9DIR |= greenLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off
P9OUT |= greenLED; // Turn LED On (alternate flashing)

// Configure ACLK to the 32 KHz crystal
config_ACLK_to_32KHz_crystal();

// Configure Channel 0 for up mode with interrupt
TA0CCR0 = (32768 - 1); // Fill to get 1 second @ 32 KHz
TA0CCTL0 |= CCIE; // Enable Channel 0 CCIE bit
TA0CCTL0 &= ~CCIFG; // Clear Channel 0 CCIFG bit

// Timer_A: ACLK, div by 1, up mode, clear TAR (leaves TAIE=0)
TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

// Enable the global interrupt bit (call an intrinsic function)
_enable_interrupts();

for(;;) {}
}

//ISR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void T0A0_ISR() {
// Toggle the LEDs
    P1OUT ^=redLED;   //toggle led
    P9OUT ^=greenLED;

// Hardware clears the flag (CCIFG in TA0CCTL0)
}


//4.3 Push Button with Interrupt
//MAIN
void main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

//Configures LEDs
P1DIR |= redLED; // Direct pin as output
P9DIR |= greenLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off
P9OUT &= ~greenLED; // Turn LED Off

// Configuring buttons with interrupt
P1DIR &= ~(BUT1| BUT2); // 0: input
P1REN |= (BUT1 | BUT2); // 1: enable built-in resistors
P1OUT |= (BUT1 | BUT2); // 1: built-in resistor is pulled up to Vcc
P1IE  |= (BUT1 | BUT2); // 1: enable interrupts
P1IES |= (BUT1 | BUT2); // 1: interrupt on falling edge
P1IFG &= ~(BUT1| BUT2); // 0: clear the interrupt flags

// Enable the global interrupt bit (call an intrinsic function)
_enable_interrupts();

for(;;){} //Holds CPU
}

#pragma vector = PORT1_VECTOR // Write the vector name
__interrupt void Port1_ISR() {
// Detect button 1 (BUT1 in P1IFG is 1)
if ((P1IFG &BUT1) != 0 ) {
// Toggle the red LED
P1OUT ^= redLED;

// Clear BUT1 in P1IFG
P1IFG &= ~BUT1;

}
// Detect button 2 (BUT2 in P1IFG is 1)
if ((P1IFG &BUT2) != 0 ) {
// Toggle the green LED
P9OUT ^= greenLED;

// Clear BUT2 in P1IFG
P1IFG &= ~BUT2;
    }
}

//4.4 Low-Power Modes
//Code #1
void main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

//Configures LEDs
P1DIR |= redLED; // Direct pin as output
P9DIR |= greenLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off
P9OUT &= ~greenLED; // Turn LED Off

// Configure ACLK to the 32 KHz crystal
config_ACLK_to_32KHz_crystal();

// Timer_A configuration (fill the line below)
// Use ACLK, divide by 1, continuous mode, TAR cleared, enable interrupt for rollback-to-zero event
TA0CTL = TASSEL_1|ID_0|MC_2|TACLR|TAIE; //The interrupt was enabled for Timer A 0

// Ensure the flag is cleared at the start
TA0CTL &= ~TAIFG;

//because this also enables GIE theres no need to use _enable_interrupts(); intrinsic function
_low_power_mode_3();//ACLK will remain on so that Timer peripheral isnt shut down
}

//ISR
#pragma vector = TIMER0_A1_VECTOR // Link the ISR to the vector
__interrupt void T0A1_ISR() {  //can it also be TA0_A1_ISR()????
// Toggle both LEDs
    P1OUT ^= redLED;
    P9OUT ^= greenLED;
// Clear the TAIFG flag
    TA0CTL &= ~TAIFG;
}

//Code #2
//Part 4.2 Interrupt UP MODE
// Timer_A up mode, with interrupt, flashes LEDs

/*void main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

//Configuring LEDs
P1DIR |= redLED; // Direct pin as output
P9DIR |= greenLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off
P9OUT |= greenLED; // Turn LED On (alternate flashing)

// Configure ACLK to the 32 KHz crystal
config_ACLK_to_32KHz_crystal();

// Configure Channel 0 for up mode with interrupt
TA0CCR0 = (32768 - 1); // Fill to get 1 second @ 32 KHz
TA0CCTL0 |= CCIE; // Enable Channel 0 CCIE bit
TA0CCTL0 &= ~CCIFG; // Clear Channel 0 CCIFG bit

// Timer_A: ACLK, div by 1, up mode, clear TAR (leaves TAIE=0)
TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

_low_power_mode_3();//ACLK will remain on so that Timer peripheral isnt shut down

}

//ISR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void T0A0_ISR() {
// Toggle the LEDs
    P1OUT ^=redLED;   //toggle led
    P9OUT ^=greenLED;

// Hardware clears the flag (CCIFG in TA0CCTL0)
}*/

//CODE #3
//4.3 Push Button with Interrupt
//MAIN
void main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

//Configures LEDs
P1DIR |= redLED; // Direct pin as output
P9DIR |= greenLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off
P9OUT &= ~greenLED; // Turn LED Off

// Configuring buttons with interrupt
P1DIR &= ~(BUT1| BUT2); // 0: input
P1REN |= (BUT1 | BUT2); // 1: enable built-in resistors
P1OUT |= (BUT1 | BUT2); // 1: built-in resistor is pulled up to Vcc
P1IE  |= (BUT1 | BUT2); // 1: enable interrupts
P1IES |= (BUT1 | BUT2); // 1: interrupt on falling edge
P1IFG &= ~(BUT1| BUT2); // 0: clear the interrupt flags

_low_power_mode_4();//All CLKs are turned off since we arent using any peripherals

}

#pragma vector = PORT1_VECTOR // Write the vector name
__interrupt void Port1_ISR() {
// Detect button 1 (BUT1 in P1IFG is 1)
if ((P1IFG &BUT1) != 0 ) {
// Toggle the red LED
P1OUT ^= redLED;

// Clear BUT1 in P1IFG
P1IFG &= ~BUT1;

}
// Detect button 2 (BUT2 in P1IFG is 1)
if ((P1IFG &BUT2) != 0 ) {
// Toggle the green LED
P9OUT ^= greenLED;

// Clear BUT2 in P1IFG
P1IFG &= ~BUT2;
    }
}


//FUNCTIONS
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





