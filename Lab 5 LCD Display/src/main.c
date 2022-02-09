// Sample code that prints 430 on the LCD monitor
#include <msp430fr6989.h>
#define redLED BIT0 // Red at P1.0
#define greenLED BIT7 // Green at P9.7

#define BUT1 BIT1 // Button S1 at Port 1.1
#define BUT2 BIT2 // Button S2 at Port 1.2

//PROTOTYPES
void display_num_lcd(unsigned int n);
void Initialize_LCD();
void config_ACLK_to_32KHz_crystal();


// The array has the shapes of the digits (0 to 9)
unsigned char LCD_Num[10] = {0xFC,0x60,0xDB,0xF3,0x67,0xB7,0xBF,0xE0,0xFF,0xF7};

static unsigned int i = 0;
unsigned int state = 0;


//PART 5.1
/*int main(void) {
volatile unsigned int n;

WDTCTL = WDTPW | WDTHOLD; // Stop WDT
PM5CTL0 &= ~LOCKLPM5; // Enable GPIO pins

P1DIR |= redLED; // Pins as output
P9DIR |= greenLED;
P1OUT |= redLED; // Red on
P9OUT &= ~greenLED; // Green off

// Initializes the LCD_C module
Initialize_LCD();

display_num_lcd(65535);

// The line below can be used to clear all the segments
//LCDCMEMCTL = LCDCLRM; // Clears all the segments

// Display 430 on the rightmost three digits
//LCDM19 = LCD_Num[4];
//LCDM15 = LCD_Num[3];
//LCDM8 = LCD_Num[0];

// Flash the red and green LEDs
for(;;) {
for(n=0; n<=50000; n++) {} // Delay loop
P1OUT ^= redLED;
P9OUT ^= greenLED;
}
return 0;
}*/

//PART 5.2
//Creating a StopWatch with a delay of one second
/*void main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

volatile unsigned int n;

//Configuring LEDs
P1DIR |= redLED; // Direct pin as output
P9DIR |= greenLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off
P9OUT |= greenLED; // Turn LED On (alternate flashing)

// Configure ACLK to the 32 KHz crystal
config_ACLK_to_32KHz_crystal();

// Initializes the LCD_C module
Initialize_LCD();

// Configure Channel 0 for up mode with interrupt
TA0CCR0 = (32768 - 1); // Fill to get 1 second @ 32 KHz
TA0CCTL0 |= CCIE; // Enable Channel 0 CCIE bit
TA0CCTL0 &= ~CCIFG; // Clear Channel 0 CCIFG bit

// Timer_A: ACLK, div by 1, up mode, clear TAR (leaves TAIE=0)
TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

// Enable the global interrupt bit (call an intrinsic function)
_enable_interrupts();

for(;;) {
for(n=0; n<=50000; n++) {} // Delay loop
P1OUT ^= redLED;
P9OUT ^= greenLED;
}

}*/

//PART 5.3
void main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins


//Configuring LEDs
P1DIR |= redLED; // Direct pin as output
P9DIR |= greenLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off
P9OUT |= greenLED; // Turn LED On (alternate flashing)

// Configuring buttons with interrupt
P1DIR &= ~(BUT1| BUT2); // 0: input
P1REN |= (BUT1 | BUT2); // 1: enable built-in resistors
P1OUT |= (BUT1 | BUT2); // 1: built-in resistor is pulled up to Vcc

P1IE  |= (BUT1 | BUT2); // 1: enable interrupts
P1IES |= (BUT1 | BUT2); // 1: interrupt on falling edge
P1IFG &= ~(BUT1| BUT2); // 0: clear the interrupt flags

// Configure ACLK to the 32 KHz crystal
config_ACLK_to_32KHz_crystal();

// Initializes the LCD_C module
Initialize_LCD();

// Configure Channel 0 for up mode with interrupt
TA0CCR0 = (32768 - 1); // Fill to get 1 second @ 32 KHz
TA0CCTL0 |= CCIE; // Enable Channel 0 CCIE bit
TA0CCTL0 &= ~CCIFG; // Clear Channel 0 CCIFG bit

// Timer_A: ACLK, div by 1, up mode, clear TAR (leaves TAIE=0)
TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

_low_power_mode_3();//ACLK will remain on

}

//Timer ISR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void T0A0_ISR() {

    display_num_lcd(i);
    i++;
// Hardware clears the flag (CCIFG in TA0CCTL0)
}

//Buttons ISR Pause/Resume/Reset
#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(){

//Pushing Button 1
if ((P1IFG &BUT1) != 0 ){

   state = 1 - state; //inverts the previous state when the interrupt occurs

  if(state == 1){
      P1OUT |=redLED;
      P9OUT &=~greenLED;
      TA0CTL &= ~MC_3; //the inverse of 11 == 00 so it pauses the clock/timer
  }

  if(state == 0){
      P1OUT &=~redLED;
      P9OUT |=greenLED;
      TA0CTL |= MC_1; //resume operation
  }

// Clear BUT1 Flag
  P1IFG &= ~BUT1;
  }

//Pushing Button 2
if ((P1IFG &BUT2) != 0 ){

 i = i * 0;
 display_num_lcd(i);
 i++;
 // Clear BUT2 Flag
 P1IFG &= ~BUT2;

  }

}


//FUNCTIONS
void display_num_lcd(unsigned int n){

unsigned int digit; // Since its an int we can use modulo and just take the integer we want to extract. An int type variable will ignore the decimal digits

//Prints 0 if n == 0
if(n == 0){
 LCDM8 = LCD_Num[n]; // Just print 0 on pos 6

 LCDM10 = 0;//pos 1   turns off leading zeros
 LCDM6 = 0; //pos2
 LCDM4 = 0;//pos 3
 LCDM19 = 0;//pos 4
 LCDM15 = 0;//pos 5
}

//Else prints the n number > 0
else if(n>0){

digit = n % 10;
LCDM8 = LCD_Num[digit]; //pos 6
n = n/10;


if(n!=0){
digit = n % 10;
LCDM15 = LCD_Num[digit]; //pos 5
n = n/10;
}

else{
   LCDM15 = 0; //pos 5
   LCDM19 = 0; //pos4
   LCDM4 = 0; //pos 3
   LCDM6 = 0; //pos 2
   LCDM10 = 0; //pos1
}

if(n!=0){
digit = n % 10;
LCDM19 = LCD_Num[digit]; //pos 4
n = n/10;
}

else{
    LCDM19 = 0; //pos4
    LCDM4 = 0; //pos 3
    LCDM6 = 0; //pos 2
    LCDM10 = 0; //pos1;
}

if(n!=0){
digit = n % 10;
LCDM4 = LCD_Num[digit]; //pos 3
n = n/10;
}

else{
      LCDM4 = 0; //pos 3
      LCDM6 = 0; //pos 2
      LCDM10 = 0; //pos1
}

if(n!=0){
digit = n % 10;
LCDM6 = LCD_Num[digit]; //pos 2
n = n/10;
}

else{
      LCDM6 = 0; //pos 2
      LCDM10 = 0; //pos1
   }

  }
return;
}

//**********************************************************
// Initializes the LCD_C module
// *** Source: Function obtained from MSP430FR6989’s Sample Code ***
void Initialize_LCD() {
PJSEL0 = BIT4 | BIT5; // For LFXT

// Initialize LCD segments 0 - 21; 26 - 43
LCDCPCTL0 = 0xFFFF;
LCDCPCTL1 = 0xFC3F;
LCDCPCTL2 = 0x0FFF;

// Configure LFXT 32kHz crystal
CSCTL0_H = CSKEY >> 8; // Unlock CS registers
CSCTL4 &= ~LFXTOFF; // Enable LFXT

do {
CSCTL5 &= ~LFXTOFFG; // Clear LFXT fault flag
SFRIFG1 &= ~OFIFG;
}while (SFRIFG1 & OFIFG); // Test oscillator fault flag

CSCTL0_H = 0; // Lock CS registers

// Initialize LCD_C
// ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;

// VLCD generated internally,
// V2-V4 generated internally, v5 to ground
// Set VLCD voltage to 2.60v

// Enable charge pump and select internal reference for it
LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN;
LCDCCPCTL = LCDCPCLKSYNC; // Clock synchronization enabled
LCDCMEMCTL = LCDCLRM; // Clear LCD memory

//Turn LCD on
LCDCCTL0 |= LCDON;
return;
}

//CRYSTAL FUNCTION
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
