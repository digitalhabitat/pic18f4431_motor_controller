/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "user.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */

    /* Setup analog functionality and port direction */
    TRISBbits.TRISB0 = 0; // enable port B bit 0 as output
    
    // Setup UART
    TRISCbits.TRISC6 = 0;   // TX as output
    TRISCbits.TRISC7 = 1;   // RX as input
    
    TXSTA1bits.SYNC = 0;    // Async operation
    TXSTA1bits.TX9 = 0;     // No tx of 9th bit
    TXSTA1bits.TXEN = 1;    // Enable transmitter
    
    RCSTA1bits.RX9 = 0;     // No rx of 9th bit
    RCSTA1bits.CREN = 1;    // Enable receiver
    RCSTA1bits.SPEN = 1;    // Enable serial port
    
    // Setting for 9600 BPS
    BAUDCONbits.BRG16 = 0;  // Divisor at 8 bit
    TXSTA1bits.BRGH = 0;    // No high-speed baudrate
    SPBRG1 = 51;            // divisor for 9600
    
    // enable USART Interrupt
    // PIE1bits.TXIE = 1;
    // INTCONbits.GIE = 1;
    // INTCONbits.PEIE = 1;

    /* Initialize peripherals */
    

    /* Configure the IPEN bit (1=on) in RCON to turn on/off int priorities */

    /* Enable interrupts */
}



