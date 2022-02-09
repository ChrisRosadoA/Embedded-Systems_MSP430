// Code to print to the LCD pixel display on the Educational BoosterPack

#include "msp430fr6989.h"
#include "Grlib/grlib/grlib.h"          // Graphics library (grlib)
#include "LcdDriver/lcd_driver.h"       // LCD driver
#include <stdio.h>

#define redLED BIT0
#define greenLED BIT7
#define button BIT1

unsigned static int state = 0;
unsigned static int clearCheck = 1;

//PART 11.1
void main(void)
{
    volatile unsigned int counter=0;
    char mystring[20];
    unsigned int n;

    WDTCTL = WDTPW | WDTHOLD;     // Stop the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5;         // Disable GPIO power-on default high-impedance mode

    P1DIR |= redLED;    P1OUT &= ~redLED;
    P9DIR |= greenLED;  P9OUT &= ~greenLED;
    P1DIR &= ~button; P1REN|=button; P1OUT|=button; // button, resistor, pullup

    // Configure SMCLK to 8 MHz (used as SPI clock)
    CSCTL0 = CSKEY;                 // Unlock CS registers
    CSCTL3 &= ~(BIT4|BIT5|BIT6);    // DIVS=0
    CSCTL0_H = 0;                   // Relock the CS registers

    ////////////////////////////////////////////////////////////////////////////////////////////
    Graphics_Context g_sContext;        // Declare a graphic library context
    Crystalfontz128x128_Init();         // Initialize the display

    // Set the screen orientation
    Crystalfontz128x128_SetOrientation(0);

    // Initialize the context
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);

    // Set background and foreground colors
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);

    // Set the default font for strings
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

    // Clear the screen
    Graphics_clearDisplay(&g_sContext);
    ////////////////////////////////////////////////////////////////////////////////////////////


    Graphics_drawStringCentered(&g_sContext, "Welcome to", AUTO_STRING_LENGTH, 64, 30, OPAQUE_TEXT);

    sprintf(mystring, "EEL 4742 Lab!");
    Graphics_drawStringCentered(&g_sContext, mystring, AUTO_STRING_LENGTH, 64, 55, OPAQUE_TEXT);

    n = 1234;
    sprintf(mystring, "%d", n);
    Graphics_drawStringCentered(&g_sContext, mystring, AUTO_STRING_LENGTH, 64, 80, OPAQUE_TEXT);

    while(1){}


}

//PART 11.2
void main(void)
{
    volatile unsigned int counter=0;
    char mystring[20];
    unsigned int n;
    extern tImage logo4BPP_UNCOMP;

    struct Rectangle{

    int x1, y1, x2, y2;

    };

    int i;

    WDTCTL = WDTPW | WDTHOLD;     // Stop the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5;         // Disable GPIO power-on default high-impedance mode

    P1DIR |= redLED;    P1OUT &= ~redLED;
    P9DIR |= greenLED;  P9OUT &= ~greenLED;
    P1DIR &= ~button; P1REN|=button; P1OUT|=button; // button, resistor, pullup

    //Button Interrupt Configuration
    P1IE  |= (button); // 1: enable interrupts
    P1IES |= (button); // 1: interrupt on falling edge
    P1IFG &= ~(button); // 0: clear the interrupt flags

    // Enable the global interrupt bit (call an intrinsic function)
    _enable_interrupts();

    // Configure SMCLK to 8 MHz (used as SPI clock)
    CSCTL0 = CSKEY;                 // Unlock CS registers
    CSCTL3 &= ~(BIT4|BIT5|BIT6);    // DIVS=0
    CSCTL0_H = 0;                   // Relock the CS registers

    Graphics_Context g_sContext;        // Declare a graphic library context //Defined Graphics Context
    Crystalfontz128x128_Init();         // Initialize the display

    // Set the screen orientation
    Crystalfontz128x128_SetOrientation(0);

    // Initialize the context
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);

    // Brightness Set to Max
    P2OUT |= BIT6;

    // Set background and foreground colors
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_CYAN);

    // Set the default font for strings
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

    // Clear the screen
    Graphics_clearDisplay(&g_sContext);

//********************************************************************************************
    for(;;){

        // Set the default font for strings
        GrContextFontSet(&g_sContext, &g_sFontFixed6x8);


        if(state == 0){
    Graphics_drawImage(&g_sContext, &logo4BPP_UNCOMP,0,0);
    clearCheck = 1;
        }

        else{

    if(clearCheck == 1){
    Graphics_clearDisplay(&g_sContext);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    clearCheck = 0;
    }

    Graphics_drawStringCentered(&g_sContext, "Welcome to", AUTO_STRING_LENGTH, 64, 30, OPAQUE_TEXT);

    sprintf(mystring, "EEL 4742 Lab!");
    Graphics_drawStringCentered(&g_sContext, mystring, AUTO_STRING_LENGTH, 64, 55, OPAQUE_TEXT);

    //Drawing Outline Rectangle
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_CYAN);
    struct Rectangle R1 = {0,100,127,127};
    Graphics_drawRectangle(&g_sContext, &R1);

    //Drawing 2 Outline Circles
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_PURPLE);
    Graphics_drawCircle(&g_sContext, 41, 113, 5);
    Graphics_drawCircle(&g_sContext, 82, 113, 5);

    //Drawing A Filled Circle
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_GREEN);
    Graphics_fillCircle(&g_sContext, 62, 113, 5);

    //Drawing A Filled Rectangle
    struct Rectangle R2 = {41,0,84,10};
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
    Graphics_fillRectangle(&g_sContext, &R2);

    //Drawing An Horizontal Line
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_CYAN);
    Graphics_drawLineH(&g_sContext,48, 78, 88);

       for( i= 0; i<256; i++){
        // Set the Numbers font
        GrContextFontSet(&g_sContext, &g_sFontCm12b);

        if(i<10)  sprintf(mystring, "  %d", i);
        else if (i<100)  sprintf(mystring, " %d", i);
        else  sprintf(mystring, "%d", i);

     Graphics_drawStringCentered(&g_sContext, mystring, AUTO_STRING_LENGTH, 64, 80, OPAQUE_TEXT);

        if((button & P1IN) == 0)
       break;

            }
       }
   }

    while(1){}
}
//FUNCTIONS & ISRs
//PUSH BUTTON ISR
#pragma vector = PORT1_VECTOR // Write the vector name
__interrupt void Port1_ISR() {

// Detect button 1 (BUT1 in P1IFG is 1)
if ((P1IFG &button) != 0 ) {

    state = 1 - state; //inverts the previous state when the interrupt occurs
    clearCheck = 1 - clearCheck;

// Clear BUT1 in P1IFG
P1IFG &= ~button;
    }
}


