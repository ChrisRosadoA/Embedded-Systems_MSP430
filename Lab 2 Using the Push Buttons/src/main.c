// Turning on the red LED while button S1 is pushed
#include <msp430fr6989.h>
#include <stdint.h>

#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7 // Green LED at P9.7
#define BUT1 BIT1 // Button S1 at P1.1
#define BUT2 BIT2 //Button S2 at P1.2


//PART 2.1
void main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

// Configure and initialize LEDs
P1DIR |= redLED; // Direct pin as output
P9DIR |= greenLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off
P9OUT &= ~greenLED; // Turn LED Off

// Configure buttons
P1DIR &= ~BUT1; // Direct pin as input   //pushing button is input == 0    //set for P1 Bit 1
P1REN |= BUT1; // Enable built-in resistor
P1OUT |= BUT1; // Set resistor as pull-up

// Polling the button in an infinite loop
for(;;) {

// Fill the if-statement below...
if ((P1IN &=BUT1)== 0)  //... button S1 is pushed on 0 because its on Active Low configuration
P1OUT |= redLED; // Turn red LED on
else P1OUT &= ~redLED; // Turn red LED off

    }
}


//PART 2.2
/*void main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

// Configure and initialize LEDs
P1DIR |= redLED; // Direct pin as output
P9DIR |= greenLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off
P9OUT &= ~greenLED; // Turn LED Off

// Configure buttons
P1DIR &= ~BUT1; // Direct pin as input   //pushing button is input == 0    //set for P1 Bit 1
P1REN |= BUT1; // Enable built-in resistor
P1OUT |= BUT1; // Set resistor as pull-up

//CONFIG BUTTON 2
P1DIR &= ~BUT2; // Direct pin as input   //pushing button is input == 0    //set for P1 Bit 1
P1REN |= BUT2; // Enable built-in resistor
P1OUT |= BUT2; // Set resistor as pull-up

// Polling the button in an infinite loop
for(;;) {

// Fill the if-statement below...
if ((P1IN &=BUT1)== 0)  //... button S2 is pushed on 0 because its on Active Low configuration
P1OUT |= redLED; // Turn red LED on
else P1OUT &= ~redLED; // Turn red LED off

//FOR GREEN LED PUSH
if ((P1IN &=BUT2)== 0)  //... button S2 is pushed on 0 because its on Active Low configuration
P9OUT |= greenLED; // Turn green LED on
else P9OUT &= ~greenLED; // Turn green LED off
    }
}*/


//PART 2.3 Turn ON both LEDS but NOT simultaneously
/*void main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

// Configure and initialize LEDs
P1DIR |= redLED; // Direct pin as output
P9DIR |= greenLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off
P9OUT &= ~greenLED; // Turn LED Off

// Configure buttons
P1DIR &= ~BUT1; // Direct pin as input   //pushing button is input == 0    //set for P1 Bit 1
P1REN |= BUT1; // Enable built-in resistor
P1OUT |= BUT1; // Set resistor as pull-up

//CONFIG BUTTON 2
P1DIR &= ~BUT2; // Direct pin as input   //pushing button is input == 0    //set for P1 Bit 1
P1REN |= BUT2; // Enable built-in resistor
P1OUT |= BUT2; // Set resistor as pull-up

// Polling the button in an infinite loop
for(;;) {

//FOR RED LED PUSH   //Port 1 input FOR Bit 1 which is button 1 on the board
while ((P1IN &= BUT1) == 0){  //... button S1 is pushed on 0 because its on Active Low configuration
    P1OUT |= redLED;// Turn red LED on
}

    P1OUT &= ~redLED; // Turn red LED off

//FOR GREEN LED PUSH
while ((P1IN &=BUT2) == 0){  //... button S2 is pushed on 0 because its on Active Low configuration
    P9OUT |= greenLED;                            // Turn green LED on //Port 9 Bit 7 is set to 1
}
    P9OUT &= ~greenLED;                           // Turn green LED off
    }
}*/

//PART 2.4 MY Own Design
//Pushing button 1 flashes redLED and pushing button 2 flashes both LEDs out of sync
/*void main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

volatile uint32_t i; // unsigned int 32-bit type

// Configure and initialize LEDs
P1DIR |= redLED; // Direct pin as output
P9DIR |= greenLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off
P9OUT &= ~greenLED; // Turn LED Off

// Configure buttons
P1DIR &= ~BUT1; // Direct pin as input   //pushing button is input == 0    //set for P1 Bit 1
P1REN |= BUT1; // Enable built-in resistor
P1OUT |= BUT1; // Set resistor as pull-up

//CONFIG BUTTON 2
P1DIR &= ~BUT2; // Direct pin as input   //pushing button is input == 0    //set for P1 Bit 1
P1REN |= BUT2; // Enable built-in resistor
P1OUT |= BUT2; // Set resistor as pull-up

// Polling the button in an infinite loop
for(;;) {

//FOR RED LED PUSH   //Port 1 input FOR Bit 1 which is button 1 on the board
while((P1IN &= BUT1) == 0){  //... button S1 is pushed on 0 because its on Active Low configuration

    for(i=0; i<15000; i++){}
    P1OUT ^= redLED;// Toggle the redLED
}

P1OUT &= ~redLED;  //Make sure redLED turns off when not pushing the button


if((P1IN &= BUT2)== 0)
    P1OUT |=redLED;

while((P1IN &= BUT2)==0){
    for(i=0; i<15000; i++){}
    P1OUT ^=redLED;   //toggle led
    P9OUT ^=greenLED;
}


   P1OUT &= (~redLED); //makes sure LEDs stays off when you stop pushing the button
   P9OUT &= (~greenLED);//makes sure LEDs stays off when you stop pushing the button

   }
}*/




