#include <msp430fr6989.h>
#include<stdlib.h> //needed for the NULL char
#include <stdint.h> //needed for uint 16_t variable types

#define FLAGS UCA1IFG // Contains the transmit & receive flags
#define RXFLAG UCRXIFG // Receive flag
#define TXFLAG UCTXIFG // Transmit flag

#define TXBUFFER UCA1TXBUF // Transmit buffer
#define RXBUFFER UCA1RXBUF // Receive buffer

#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7 //Green LED at P9.7

//PROTOTYPES
void Initialize_UART(void); //Part 8.1
void uart_write_char(unsigned char ch); //Part 8.1
unsigned char uart_read_char(void); //Part 8.1

void uart_write_uint16(unsigned int num); //PART 8.2

void uart_write_string(char *str); //PART 8.3

void Initialize_UART_2(void); //Part 8.4
void config_ACLK_to_32KHz_crystal(); //PART 8.4 required


//PART 8.1
//MAIN
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop WDT
    PM5CTL0 &= ~LOCKLPM5; // Disable GPIO power-on default high impedance mode

    P1DIR |= redLED; // Direct pin as output
    P1OUT &= ~redLED; // Turn LED OFF

    P9DIR |= greenLED; // Direct pin as output
    P9OUT &= ~greenLED; // Turn LED OFF

    // Divert pins to backchannel UART functionality
    // (UCA1TXD same as P3.4) (UCA1RXD same as P3.5)

    // (P3SEL1=00, P3SEL0=11) (P2DIR=xx)
    //P3SEL1 &= ~(BIT4|BIT5);
    //P3SEL0 |= (BIT4|BIT5);

    Initialize_UART();

    char ch;
    volatile uint16_t i;

    for(;;) {

       for( ch ='0'; ch<='9'; ch++){

           uart_read_char();// so when the theres new data '1' the MCU "reads" it and turns on the greenLED

                        if((RXBUFFER) == '1'){
                            P9OUT |= greenLED; // Turn on the greenLED
                           }

                        if((RXBUFFER) == '2'){
                            P9OUT &= ~greenLED; // Turn on the greenLED
                          }

           for(i=0; i<25000; i++) {} //Delay loop
              P1OUT ^= redLED; // Toggle the redLED

	    uart_write_char(ch); //flag for Txbuf is ready to trasmit and then the char is written/transmitted on the terminal
	    uart_write_char(5);
	    uart_write_char('\n');
	    uart_write_char('\r');
	   }
    }

	return 0;
}

//PART 8.2 Sending unsigned 16-bit integers
/*int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop WDT
    PM5CTL0 &= ~LOCKLPM5; // Disable GPIO power-on default high impedance mode

    P1DIR |= redLED; // Direct pin as output
    P1OUT &= ~redLED; // Turn LED OFF

    P9DIR |= greenLED; // Direct pin as output
    P9OUT &= ~greenLED; // Turn LED OFF

    // Divert pins to backchannel UART functionality
    // (UCA1TXD same as P3.4) (UCA1RXD same as P3.5)

    // (P3SEL1=00, P3SEL0=11) (P2DIR=xx)
    P3SEL1 &= ~(BIT4|BIT5);
    P3SEL0 |= (BIT4|BIT5);

    Initialize_UART();

    unsigned int num = 65530;
    volatile unsigned int i;

    for(;;) {

       for(num; num<65536; num++){

           for(i=0; i<35000; i++) {} //Delay loop
           P1OUT ^= redLED; // Toggle the redLED

           uart_read_char();// so when the theres new data '1' the MCU "reads" it and turns on the greenLED

                if((RXBUFFER) == '1'){
                    P9OUT |= greenLED; // Turn on the greenLED
                   }

                if((RXBUFFER) == '2'){
                    P9OUT &= ~greenLED; // Turn on the greenLED
                  }

        uart_write_uint16(num); //flag for Txbuf is ready to trasmit and then the char is written/transmitted on the terminal
        uart_write_char('\n');
        uart_write_char('\r');
       }

    }

    return 0;
}*/

//PART 8.3 Sending an ASCII String over UART
/*int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop WDT
    PM5CTL0 &= ~LOCKLPM5; // Disable GPIO power-on default high impedance mode

    // Divert pins to backchannel UART functionality
    // (UCA1TXD same as P3.4) (UCA1RXD same as P3.5)

    // (P3SEL1=00, P3SEL0=11) (P2DIR=xx)
    P3SEL1 &= ~(BIT4|BIT5);
    P3SEL0 |= (BIT4|BIT5);

    Initialize_UART();

    //char mystring[] = "UART Transmission Begins...";
    char mystring[] = "Final Fantasy VII: Remake!... ";

    uart_write_string(mystring);

    return 0;
}*/

//PART 8.4 Changing the Configuration
/*int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop WDT
    PM5CTL0 &= ~LOCKLPM5; // Disable GPIO power-on default high impedance mode

    P1DIR |= redLED; // Direct pin as output
    P1OUT &= ~redLED; // Turn LED OFF

    P9DIR |= greenLED; // Direct pin as output
    P9OUT &= ~greenLED; // Turn LED OFF

    config_ACLK_to_32KHz_crystal();

    // Divert pins to backchannel UART functionality
    // (UCA1TXD same as P3.4) (UCA1RXD same as P3.5)

    // (P3SEL1=00, P3SEL0=11) (P2DIR=xx)
    P3SEL1 &= ~(BIT4|BIT5);
    P3SEL0 |= (BIT4|BIT5);

    Initialize_UART_2();

    unsigned int num;
    volatile uint16_t i;

    for(;;) {

       for(num = 0; num<65536; num++){

           for(i=0; i<30000; i++) {} //Delay loop
           P1OUT ^= redLED; // Toggle the redLED

           uart_read_char();// so when the theres new data '1' the MCU "reads" it and turns on the greenLED //or send it back to a variable and compare it

                if((RXBUFFER) == '1'){
                    P9OUT |= greenLED; // Turn on the greenLED
                   }

                if((RXBUFFER) == '2'){
                    P9OUT &= ~greenLED; // Turn on the greenLED
                  }

        uart_write_uint16(num); //flag for Txbuf is ready to trasmit and then the char is written/transmitted on the terminal
        uart_write_char('\n');
        uart_write_char('\r');
       }

    }

    return 0;
}*/



//FUNCTIONS
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

// The function returns the byte; if none received, returns NULL
unsigned char uart_read_char(void){
unsigned char temp;

// Return NULL if no byte received
if( (FLAGS & RXFLAG) == 0)
return NULL;

// Otherwise, copy the received byte (clears the flag) and return it
temp = RXBUFFER;
return temp;
}

//FUNCTION PART 8.2 function
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

//FUNCTION PART 8.3
void uart_write_string(char *str){

int i = 0;

while((str[i])!= '\0'){

    uart_write_char(str[i]);
    i++;
}

return;
}

//FUNCTION PART 8.4
void Initialize_UART_2(void){
    // Changing UART Configuration
    // 4800 baud, 8-bit data, LSB first, no parity bits, 1 stop bit
    // no flow control
    // Initial clock: ACLK @ 32 kHz with no oversampling

    // Divert pins to UART functionality
    P3SEL1 &= ~(BIT4|BIT5);
    P3SEL0 |= (BIT4|BIT5);

    // Use ACLK clock; leave other settings default
    UCA1CTLW0 |= UCSSEL_1;

    // Configure the clock dividers and modulators
    // UCBR=6, UCBRF= Dont Care, UCBRS=0x20, UCOS16=0 (no oversampling)

    UCA1BRW = 6;
    UCA1MCTLW = UCBRS7|UCBRS6|UCBRS5|UCBRS3|UCBRS2|UCBRS1; //REVIEW!!!
    UCA1MCTLW &= ~UCOS16;


    // Exit the reset state (so transmission/reception can begin)
    UCA1CTLW0 &= ~UCSWRST;

}

// FUNCTION Configures ACLK to 32 KHz crystal
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


