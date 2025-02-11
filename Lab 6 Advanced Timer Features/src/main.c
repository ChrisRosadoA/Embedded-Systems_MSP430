#include <msp430fr6989.h>
#define redLED BIT0 // Red at P1.0
#define greenLED BIT7 // Green at P9.7

//PROTOTYPES
void config_ACLK_to_32KHz_crystal();

//Global Variables
static int status = 0;

// Using Timer_A with 2 channels
// Using ACLK @ 32 KHz (undivided)
// Channel 0 toggles the red LED every 0.1 seconds
// Channel 1 toggles the green LED every 0.5 seconds

//PART 6.1: Using the Timer with Two Channels
	/*void main(void) {
	WDTCTL = WDTPW | WDTHOLD; // Stop WDT
	PM5CTL0 &= ~LOCKLPM5; // Enable GPIO pins

	config_ACLK_to_32KHz_crystal();

	P1DIR |= redLED;
	P9DIR |= greenLED;
	P1OUT &= ~redLED;//Initially Off
	P9OUT &= ~greenLED;//Initially Off

	// Configure Channel 0
	TA0CCR0 = 3277-1; // @ 32 KHz --> 0.1 seconds
	TA0CCTL0 |= CCIE;
	TA0CCTL0 &= ~CCIFG;

	// Configure Channel 1 (write 3 lines similar to above)
	TA0CCR1 = 16384-1; // @ 32 KHz --> 0.5 seconds
	TA0CCTL1 |= CCIE;
	TA0CCTL1 &= ~CCIFG;

	// Configure timer (ACLK) (divide by 1) (continuous mode)
	TA0CTL = TASSEL_1 | ID_0 | MC_2 | TACLR;

	// Engage a low-power mode
    _low_power_mode_3();

	return;

	}*/

	//PART 6.2: Using Three Channels
	/*void main(void) {
	WDTCTL = WDTPW | WDTHOLD; // Stop WDT
	PM5CTL0 &= ~LOCKLPM5; // Enable GPIO pins

	config_ACLK_to_32KHz_crystal();

	P1DIR |= redLED;
    P9DIR |= greenLED;
    P1OUT &= ~redLED;//Initially Off
    P9OUT &= ~greenLED;//Initially Off

    // Configure Channel 0
    TA0CCR0 = 819 - 1; // @ 8KHz --> 0.1 seconds
    TA0CCTL0 |= CCIE;
	TA0CCTL0 &= ~CCIFG;

	// Configure Channel 1
	TA0CCR1 = 4096 - 1; // @ 8Khz --> 0.5 seconds
	TA0CCTL1 |= CCIE;
	TA0CCTL1 &= ~CCIFG;

	//Configure Channel 2
	TA0CCR2 = 32768 - 1; // @ 8Khz --> 4 seconds
	TA0CCTL2 |= CCIE;
	TA0CCTL2 &= ~CCIFG;

	// Configure timer (ACLK) (divide by 1) (continuous mode)
	TA0CTL = TASSEL_1 | ID_2 | MC_2 | TACLR;
	TA0CTL &= ~TAIFG;

	// Engage a low-power mode
	_low_power_mode_3();

	return;

	}*/

	//PART 6.3: Generating a PWM Signal with Timer A
	// Generating a PWM on P1.0 (red LED)
	// P1.0 coincides with TA0.1 (Timer0_A Channel 1)
	// Divert P1.0 pin to TA0.1 ---> P1DIR=1, P1SEL1=0, P1SEL0=1
	// PWM frequency: 1000 Hz -> 0.001 seconds
	/*#define PWM_PIN BIT0

	void main(void) {
	WDTCTL = WDTPW | WDTHOLD; // Stop WDT
	PM5CTL0 &= ~LOCKLPM5;

	// Divert pin to TA0.1 functionality (complete last two lines) //MISSING LINES COMPLETED
	P1DIR |= PWM_PIN; // P1DIR bit = 1
	P1SEL1 &= ~PWM_PIN; // P1SEL1 bit = 0
	P1SEL0 |= PWM_PIN;  // P1SEL0 bit = 1

	// Configure ACLK to the 32 KHz crystal (call function)

    config_ACLK_to_32KHz_crystal();

	// Starting the timer in the up mode; period = 0.001 seconds
	// (ACLK @ 32 KHz) (Divide by 1) (Up mode)
	TA0CCR0 = (33-1); // @ 32 KHz --> 0.001 seconds (1000 Hz)
	TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

	// Configuring Channel 1 for PWM
	TA0CCTL1 |= OUTMOD_7; // Output pattern: Reset/set
	TA0CCR1 = 1; // Modify this value between 0 and

	// 32 to adjust the brightness level
	for(;;) {}
	return;
	}*/

	//PART 6.4: Cycling through Brightness Levels
#define PWM_PIN BIT0

    void main(void) {
    WDTCTL = WDTPW | WDTHOLD; // Stop WDT
    PM5CTL0 &= ~LOCKLPM5;

    // Divert pin to TA0.1 functionality (complete last two lines)
    P1DIR |= PWM_PIN; // P1DIR bit = 1
    P1SEL1 &= ~PWM_PIN; // P1SEL1 bit = 0
    P1SEL0 |= PWM_PIN;  // P1SEL0 bit = 1

    // Configure ACLK to the 32 KHz crystal (call function)

    config_ACLK_to_32KHz_crystal();

    // Starting the timer in the up mode; period = 0.001 seconds
    // (ACLK @ 32 KHz) (Divide by 1) (Up mode)
    TA0CCR0 = (33-1); // @ 32 KHz --> 0.001 seconds (1000 Hz)
    TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

    // Configuring Channel 1 for PWM
    TA0CCTL1 |= OUTMOD_7; // Output pattern: Reset/set
    TA0CCR1 = 1; // Modify this value between 0 and 32 to adjust the brightness


    TA1CCR0 = 32768 - 1;
    TA1CCTL0 |= CCIE;
    TA1CCTL0 &= ~CCIFG;

    TA1CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;
    TA1CTL &= ~TAIFG;

    _low_power_mode_3();

    return;
    }


	//ISRs
	// Part 6.1 ISR of Channel 0 (A0 vector)
	/*#pragma vector = TIMER0_A0_VECTOR
	__interrupt void T0A0_ISR() {

	P1OUT ^= redLED; // Toggle the red LED
	TA0CCR0 += 3277; // Schedule the next interrupt
	// Hardware clears Channel 0 flag (CCIFG in TA0CCTL0)
	}*/


	// Part 6.1 ISR of Channel 1 (A1 vector)
	/*#pragma vector = TIMER0_A1_VECTOR
	__interrupt void T0A1_ISR() {
	P9OUT ^= greenLED;// Toggle the green LED
    TA0CCR1 += 16384;// Schedule the next interrupt
    TA0CCTL1 &=  ~CCIFG;// Clear Channel 1 interrupt flag
	}*/


    // Part 6.2 ISR of Channels 1 and 2 (A1 vector)
/*#pragma vector = TIMER0_A0_VECTOR
__interrupt void T0A0_ISR(){

    if(status == 1){
    P1OUT ^= redLED;
       }
    TA0CCR0 += 819;

    }

    #pragma vector = TIMER0_A1_VECTOR
    __interrupt void T0A1_ISR(){

    if((TA0CCTL1 & CCIFG) != 0){
    if(status == 1)
    P9OUT ^= greenLED;
    }

    TA0CCR1 += 4096;
    TA0CCTL1 &= ~CCIFG;

    if((TA0CCTL2 & CCIFG) != 0){
    status ^= 1;

    TA0CCR2 += 32768;
    TA0CCTL2 &= ~CCIFG;
      }
    }*/

//Part 6.4
     #pragma vector = TIMER1_A0_VECTOR
    __interrupt void T1A0_ISR()
       {
         P9OUT ^= greenLED;

         if (TA0CCR1 >= 30)
             TA0CCR1 = 0;
         else
             TA0CCR1 += 5;

             TA1CCR0 += 32768;
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

