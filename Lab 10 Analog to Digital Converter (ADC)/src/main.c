#include <msp430fr6989.h>

//FOR UART
#define FLAGS UCA1IFG // Contains the transmit & receive flags
#define RXFLAG UCRXIFG // Receive flag
#define TXFLAG UCTXIFG // Transmit flag

#define TXBUFFER UCA1TXBUF // Transmit buffer
#define RXBUFFER UCA1RXBUF // Receive buffer

#define redLED BIT0 // Red at P1.0

//PROTOTYPES
void Initialize_ADC();
void Initialize_UART(void);
void uart_write_uint16(unsigned int num);
void uart_write_char(unsigned char ch);
void config_ACLK_to_32KHz_crystal();

//MAIN
//PART 10.1: Using The ADC SAR Type
int main(void)
{
    //SETUP AREA
        WDTCTL = WDTPW | WDTHOLD; // Stop WDT
        PM5CTL0 &= ~LOCKLPM5; // Disable GPIO power-on default high impedance mode

        //Configuring Red LED
        P1DIR |= redLED; // Direct pin as output
        P1OUT &= ~redLED; // Turn LED Off

        config_ACLK_to_32KHz_crystal();

        // Configure Timer_A
        TA0CCR0 = (16384-1); //32768 = 0.5 sec delay
        // Use ACLK, divide by 1, up mode, clear TAR
        TA0CTL = TASSEL_1|ID_0|MC_1|TACLR;
        // Ensure flag is cleared at the start
        TA0CTL &= ~TAIFG;

        Initialize_UART();
        Initialize_ADC();

	 //UART Communication to Display JoyStick
	    for(;;){

	    while((TA0CTL & TAIFG) == 0) {}

	    ADC12CTL0 |= ADC12SC; //Setting the ADC12SC bit starts the conversion
	    while ((ADC12CTL1 &= ADC12BUSY) == 0) {} //Wait for ADC12BUSY bit to Clear meaning Result is Ready

	    uart_write_uint16(ADC12MEM0); //Output Horizontal JoyStick Result
	    uart_write_char('\n');
	    uart_write_char('\r');

	    P1OUT ^=redLED; //Toggling Red Led to Indicate ongoing activity

	    TA0CTL &= ~TAIFG; // Clear Timer Flag

	    }
}

//PART 10.2: Reading the X- and Y- Coordinates of the JoyStick
int main(void)
{
    //SETUP AREA
        WDTCTL = WDTPW | WDTHOLD; // Stop WDT
        PM5CTL0 &= ~LOCKLPM5; // Disable GPIO power-on default high impedance mode

        //Configuring Red LED
        P1DIR |= redLED; // Direct pin as output
        P1OUT &= ~redLED; // Turn LED Off

        config_ACLK_to_32KHz_crystal();

        // Configure Timer_A
        TA0CCR0 = (16384-1); //32768 = 0.5 sec delay
        // Use ACLK, divide by 1, up mode, clear TAR
        TA0CTL = TASSEL_1|ID_0|MC_1|TACLR;
        // Ensure flag is cleared at the start
        TA0CTL &= ~TAIFG;

        Initialize_UART();
        Initialize_ADC();

       unsigned int X;
       unsigned int Y;

     //UART Communication to Display JoyStick
        for(;;){

        while((TA0CTL & TAIFG) == 0) {}

        ADC12CTL0 |= ADC12SC; //Setting the ADC12SC bit starts the conversion

        while ((ADC12CTL1 & ADC12BUSY) != 0) {} //Wait for ADC12BUSY bit to Clear meaning Results are Ready

        X = ADC12MEM0;
        Y = ADC12MEM1;


        uart_write_uint16(X); //Output Horizontal Axis Result
        uart_write_char('\t');
        uart_write_uint16(Y); //Output Vertical Axis Result
        uart_write_char('\n');
        uart_write_char('\r');

        P1OUT ^=redLED; //Toggling Red Led to Indicate ongoing activity

        TA0CTL &= ~TAIFG; // Clear Timer Flag

        }
}

void Initialize_ADC(){
// Divert the pins to analog functionality
// X-axis: A10/P9.2, for A10 (P9DIR=x, P9SEL1=1, P9SEL0=1)
P9SEL1 |= BIT2;
P9SEL0 |= BIT2;

// Y-axis: A4/P8.7, for A4 (P8DIR=x, P8SEL1=1, P8SEL0=1)
P8SEL1 |= BIT7;
P8SEL0 |= BIT7;

// Turn on the ADC module
ADC12CTL0 |= ADC12ON;

// Turn off ENC (Enable Conversion) bit while modifying the configuration
ADC12CTL0 &= ~ADC12ENC;

//*************** ADC12CTL0 ***************
// Set ADC12SHT0 (select the number of cycles that you determined)
ADC12CTL0 |= ADC12SHT0_3; //Sample Hold Time 32 cycles since my result was 16.2 cycles + 12(fixed) = 28.2

// Set the bit ADC12MSC (Multiple Sample and Conversion)
ADC12CTL0 |= ADC12MSC;

//*************** ADC12CTL1 ***************
// Set ADC12SHS (select ADC12SC bit as the trigger)
ADC12CTL1 |= ADC12SHS_0;

// Set ADC12CONSEQ (select sequence-of-channels)
ADC12CTL1 |= ADC12CONSEQ_1;

// Set ADC12SHP bit
ADC12CTL1 |= ADC12SHP;

// Set ADC12DIV (select the divider you determined)
ADC12CTL1 |= ADC12DIV_0; // MODOSC divider /1 for fastest conversion

// Set ADC12SSEL (select MODOSC)
ADC12CTL1 |= ADC12SSEL_0;

//*************** ADC12CTL2 ***************
// Set ADC12RES (select 12-bit resolution)
ADC12CTL2 |= ADC12RES_2;

// Set ADC12DF (select unsigned binary format)
ADC12CTL2 &= ~ADC12DF;

//*************** ADC12CTL3 ***************
// Leave all fields at default values
// Set ADC12CSTARTADD to 0 (first conversion in ADC12MEM0) //Set to Zero to Indicate Conversion MEM Starting Point
ADC12CTL3 |= ADC12CSTARTADD_0;


//*************** ADC12MCTL0 ***************
// Set ADC12VRSEL (select VR+ = AVCC, VR- = AVSS)
ADC12MCTL0 |= ADC12VRSEL_0;

// Set ADC12INCH (select channel A10)
ADC12MCTL0 |= ADC12INCH_10;


//*************** ADC12MCTL1 ***************
// Set ADC12VRSEL (select VR+=AVCC, VR-=AVSS)
ADC12MCTL1 |= ADC12VRSEL_0;

// Set ADC12INCH (select the analog channel that you found) (A4)
ADC12MCTL1 |= ADC12INCH_4;

//Set the Conversion MEM Stop Point
ADC12MCTL1 |= ADC12EOS;


// Turn on ENC (Enable Conversion) bit at the end of the configuration
ADC12CTL0 |= ADC12ENC;
return;
}


//FUNCTIONS
void uart_write_uint16(unsigned int num){

    int digit;

    if (num>=10000){
        digit = (num/10000)%10;
        uart_write_char('0' + digit);
    }

    if(num>=1000){
            digit = (num/1000)%10;
            uart_write_char('0' + digit);
        }

    if(num>=100){
            digit = (num/100)%10;
            uart_write_char('0' + digit);
        }

    if(num>=10){
            digit = (num/10)%10;
            uart_write_char('0' + digit);
        }

    digit = num%10;
    uart_write_char('0' + digit);

    return;
}


// Configure UART to the popular configuration
// 9600 baud, 8-bit data, LSB first, no parity bits, 1 stop bit
// no flow control
// Initial clock: SMCLK @ 1.048 MHz with oversampling
void Initialize_UART(void){

// Divert pins to UART functionality
P3SEL1 &= ~(BIT4|BIT5);
P3SEL0 |= (BIT4|BIT5);

// Use SMCLK clock; leave other settings default
UCA1CTLW0 |= UCSSEL_2;

// Configure the clock dividers and modulators
// UCBR=6, UCBRF=8, UCBRS=0x20, UCOS16=1 (oversampling)
UCA1BRW = 6;
UCA1MCTLW = UCBRS5|UCBRF3|UCOS16;

// Exit the reset state (so transmission/reception can begin)
UCA1CTLW0 &= ~UCSWRST;
}

void uart_write_char(unsigned char ch){
// Wait for any ongoing transmission to complete
while ( (FLAGS & TXFLAG)==0 ) {}
// Write the byte to the transmit buffer
TXBUFFER = ch;
}

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
