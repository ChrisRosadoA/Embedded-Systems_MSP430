#include <msp430fr6989.h>
#include <stdlib.h>
#include <stdint.h> //needed for uint 16_t variable types

#define FLAGS UCA1IFG // Contains the transmit & receive flags
#define RXFLAG UCRXIFG // Receive flag
#define TXFLAG UCTXIFG // Transmit flag

#define TXBUFFER UCA1TXBUF // Transmit buffer
#define RXBUFFER UCA1RXBUF // Receive buffer

//PROTOTYPES
void Initialize_I2C(void);//I2C
int i2c_read_word(unsigned char i2c_address, unsigned char i2c_reg, unsigned int *data);//I2C
int i2c_write_word(unsigned char i2c_address, unsigned char i2c_reg, unsigned int data);//12C

void Initialize_UART(void); //UART
void uart_write_char(unsigned char ch); //UART Writes ASCII chars
unsigned char uart_read_char(void); //UART

void uart_write_uint16(unsigned int num);

void config_ACLK_to_32KHz_crystal();

//PART 9.1
//MAIN
int main(void)
{
    //VARIABLES
    unsigned int data;
    unsigned int data2;
    volatile uint16_t i;
    int count;

    //SETUP AREA
    WDTCTL = WDTPW | WDTHOLD; // Stop WDT
    PM5CTL0 &= ~LOCKLPM5; // Disable GPIO power-on default high impedance mode

     // Configure ACLK to the 32 KHz crystal (function call)
    config_ACLK_to_32KHz_crystal();

    // Configure Timer_A
    // Set timer period
    TA0CCR0 = (32768-1); //1 second delay

    // Configure Timer_A
    // Use ACLK, divide by 1, MC_1 Up mode, clear TAR
    TA0CTL = TASSEL_1|MC_1|TACLR|ID_0; //ID_0 divide by 1

    // Ensure flag is cleared at the start
    TA0CTL &= ~TAIFG;

    //Initialize UART
    Initialize_UART();

    //Initialize I2C
    Initialize_I2C();

    //UART Communication to Display Manufacturer and Device ID
    for(;;){

        for(count = 0; count<65535; count++) {

         while((TA0CTL & TAIFG) == 0) {} // 1 second delay
                    TA0CTL &= ~TAIFG;// Clear the flag

    uart_write_uint16(count);
    uart_write_char('\n');
    uart_write_char('\r');

    i2c_read_word(0x44,0x7E,&data); //Reads the Manufacturer ID Value register (0x7E) at I2C register address 0x44 and stores it in data variable
    i2c_read_word(0x44,0x7F,&data2); //Reads the Device ID Value register (0x7F) at I2C register address 0x44 and stores it in data2 variable

    uart_write_uint16(data); //Display Manufacturer ID on terminal
    uart_write_char('\n');
    uart_write_char('\r');

    uart_write_uint16(data2); //Display Device ID on terminal
    uart_write_char('\n');
    uart_write_char('\r');
        }
    }
}

//PART 9.2
//MAIN
int main(void)
{
    //VARIABLES
    unsigned int data;
    volatile uint16_t i;
    int count;

    //SETUP AREA
    WDTCTL = WDTPW | WDTHOLD; // Stop WDT
    PM5CTL0 &= ~LOCKLPM5; // Disable GPIO power-on default high impedance mode

    // Configure ACLK to the 32 KHz crystal (function call)
    config_ACLK_to_32KHz_crystal();

    // Configure Timer_A
    // Set timer period
    TA0CCR0 = (32768-1); //1 second delay

    // Configure Timer_A
    // Use ACLK, divide by 1, MC_1 Up mode, clear TAR
    TA0CTL = TASSEL_1|MC_1|TACLR|ID_0; //ID_0 divide by 1

    // Ensure flag is cleared at the start
    TA0CTL &= ~TAIFG;

    //Initialize UART
    Initialize_UART(); //Comm with read input/write Terminal-Hardware

    //Initialize I2C
    Initialize_I2C(); //Comm read/write with Hardware

    //Writing Light Sensor Configuration
    //Set RN = 0111  LSB to 1.28
    // CT = 0     Result produced in 100ms at Register 0x01
    // M = 11     Continuous Readings
    // ME = 1     Mask to Hide the Exponent from the light Result to be displayed
    i2c_write_word(0x44, 0x01, 0x7604);

    //UART Communication
    for(;;){

        for(count = 0; count<65535; count++) {

            while((TA0CTL & TAIFG) == 0) {} // 1 second delay
                    TA0CTL &=~TAIFG;// Clear the flag

    uart_write_uint16(count);
    uart_write_char('\n');
    uart_write_char('\r');

    i2c_read_word(0x44,0x00,&data); //Reads Light (lux) value from Sensor's Result Register 0x00

    data = data * 1.28;

    uart_write_uint16(data); //Outputs Lux Value to Terminal
    uart_write_char('\n');
    uart_write_char('\r');
        }
    }
}

//FUNCTIONS
//FUNCTION 1: Configure eUSCI in I2C master mode
void Initialize_I2C(void) {
// Enter reset state before the configuration starts...
UCB1CTLW0 |= UCSWRST;

// Divert pins to I2C functionality
P4SEL1 |= (BIT1|BIT0);
P4SEL0 &= ~(BIT1|BIT0);

// Keep all the default values except the fields below...
// (UCMode 3:I2C) (Master Mode) (UCSSEL 1:ACLK, 2,3:SMCLK)
UCB1CTLW0 |= UCMODE_3 | UCMST | UCSSEL_3;

// Clock divider = 8 (SMCLK @ 1.048 MHz / 8 = 131 KHz)
UCB1BRW = 8;

// Exit the reset mode
UCB1CTLW0 &= ~UCSWRST;
}

//FUNCTION 2: Read a word (2 bytes) from I2C (address, register)
int i2c_read_word(unsigned char i2c_address, unsigned char i2c_reg,
unsigned int * data) {
unsigned char byte1, byte2;

// Initialize the bytes to make sure data is received every time
byte1 = 111;
byte2 = 111;

//********** Write Frame #1 ***************************
UCB1I2CSA = i2c_address; // Set I2C address

UCB1IFG &= ~UCTXIFG0;
UCB1CTLW0 |= UCTR; // Master writes (R/W bit = Write)
UCB1CTLW0 |= UCTXSTT; // Initiate the Start Signal

while ((UCB1IFG & UCTXIFG0) ==0) {}
UCB1TXBUF = i2c_reg; // Byte = register address

while((UCB1CTLW0 & UCTXSTT)!=0) {}

if(( UCB1IFG & UCNACKIFG )!=0) return -1;
UCB1CTLW0 &= ~UCTR; // Master reads (R/W bit = Read)
UCB1CTLW0 |= UCTXSTT; // Initiate a repeated Start Signal
//****************************************************

//********** Read Frame #1 ***************************
while ( (UCB1IFG & UCRXIFG0) == 0) {}
byte1 = UCB1RXBUF;
//****************************************************
//********** Read Frame #2 ***************************
while((UCB1CTLW0 & UCTXSTT)!=0) {}
UCB1CTLW0 |= UCTXSTP; // Setup the Stop Signal

while ( (UCB1IFG & UCRXIFG0) == 0) {}
byte2 = UCB1RXBUF;

while ( (UCB1CTLW0 & UCTXSTP) != 0) {}
//****************************************************
// Merge the two received bytes
*data = ( (byte1 << 8) | (byte2 & 0xFF) );
return 0;
}

// Write a word (2 bytes) to I2C (address, register)
int i2c_write_word(unsigned char i2c_address, unsigned char i2c_reg, unsigned int data) {

unsigned char byte1, byte2;
byte1 = (data >> 8) & 0xFF; // MSByte
byte2 = data & 0xFF; // LSByte

UCB1I2CSA = i2c_address; // Set I2C address
UCB1CTLW0 |= UCTR; // Master writes (R/W bit = Write)
UCB1CTLW0 |= UCTXSTT; // Initiate the Start Signal

while ((UCB1IFG & UCTXIFG0) ==0) {}
UCB1TXBUF = i2c_reg; // Byte = register address

while((UCB1CTLW0 & UCTXSTT)!=0) {}

//********** Write Byte #1 ***************************
UCB1TXBUF = byte1;
while ( (UCB1IFG & UCTXIFG0) == 0) {}

//********** Write Byte #2 ***************************
UCB1TXBUF = byte2;
while ( (UCB1IFG & UCTXIFG0) == 0) {}

UCB1CTLW0 |= UCTXSTP;

while ( (UCB1CTLW0 & UCTXSTP) != 0) {}

return 0;

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
