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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>        /* For true/false definition */


#endif

#include "user.h"
#include "system.h"




/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */

    /* Setup analog functionality and port direction */
    TRISDbits.RD7 = 0; // enable port B bit 0 as output
    TRISDbits.RD6 = 0;
    TRISDbits.RD5 = 0;
    TRISBbits.RB5 = 0;
    
    /* Initialize peripherals */
    init_usart();
    init_timer();

    /* Configure the IPEN bit (1=on) in RCON to turn on/off int priorities */

    /* Enable interrupts */
}

void init_usart(void)
{
      // Setup UART
    TRISCbits.RC6 = 1;   // TX as output
    TRISCbits.RC7 = 1;   // RX as input
    
    TXSTA1bits.SYNC = 0;    // Async operation
    TXSTA1bits.TX9 = 0;     // 8 bit reception
    TXSTA1bits.TXEN = 1;    // Enable transmitter
    
    RCSTA1bits.RX9 = 0;     // 8 bit transmission
    RCSTA1bits.CREN = 1;    // Enable receiver
    RCSTA1bits.SPEN = 1;    // Enable serial port
    
    // Setting for 9600 BPS
    BAUDCONbits.BRG16 = 0;  // Divisor at 8 bit
    TXSTA1bits.BRGH = 0;    // No high-speed baudrate
    SPBRG1 = 129;           // divisor for 9600 baud with 20MHz FOSC (TABLE 20-3:)
        
    SPBRG = 129;          //calculated
    SPBRGH = 0; 

    // enable USART Interrupt
    // PIE1bits.TXIE = 1;
    // INTCONbits.GIE = 1;
    // INTCONbits.PEIE = 1;  
}

void init_timer(void)
{
    
    T0CONbits.TMR0ON = 0; // stop the timer
    
    #if 0
    T0CONbits.T016BIT = 0; // timer configured as 16-bit
    #else
    T0CONbits.T016BIT = 1; // timer configured as 8-bit
    #endif
    
    #if 1
    T0CONbits.T0CS = 0; // Internal clock (FOSC/4)
    #else 
    T0CONbits.T0CS = 1; // Transition on T0CKI pin input edge
    #endif
    
    #if 1
    T0CONbits.PSA = 0; // use prescaler if defined
    #else
    T0CONbits.PSA = 1; // don't use prescaler
    #endif

    T0CONbits.T0PS = 0b111; // 1:256 prescale value
    
    TMR0 = 0; // setup initial timer value 
    INTCONbits.T0IF = 0; // reset timer interrupt flag
    INTCONbits.T0IE = 1; // enable timer interrupts
    RCONbits.IPEN = 0; // do not use priorities
    INTCONbits.PEIE = 1; // enable peripheral interrupts
    INTCONbits.GIE = 1; // enable interrupts globally
    T0CONbits.TMR0ON = 1;
    
    

}

void init_pwm(void)
{
    PTCON0bits.PTOPS = 0b000;   // 1:1 PWM Time Base Output Postscale
    PTCON0bits.PTCKPS = 0b11;   // PWM time Fosc/256 (1:64 presale)
    PTCON0bits.PTMOD = 0b00;    // PWM time base operates in a Free-Running mode    
    
    PTCON1bits.PTEN = 1;        // PWM time base Timer is on
    PTCON1bits.PTDIR = 0;       // PWM time base counts up    
    
    PWMCON0bits.PWMEN = 0b010;  // PWM0 and PWM1 pins are enabled for PWM output
    PWMCON0bits.PMOD  = 0b1110; // (PWM0, PWM1) is in Complementary mode
    
    //PWMCON1bits.UDIS = 1;       //
    PWMCON1=0b00000000;      
    
    //  PWM Period Register pair Tpwm = (PTPER+1)*PTMRPS/(0.25*Fosc)
    //  12 bit-resolution
    PTPERH=0x01;        // 4 bits wide    
    PTPERL=0x38;        // 8 bits wide
    
    // PWM Duty Cycle 14-bit resolution 
    PDC0H=0b00000011;   // 6 bits wide
    PDC0L=0b00110100;   // 8 bits wide
    
    PORTDbits.RD6 = 1;
    PORTDbits.RD5 = 0;
}


void putch(unsigned char byte) {
    TXSTA = 0x26;
    RCSTA1bits.SPEN = 1;
    while (!TXIF)continue;
    TXREG = byte; /* transmit a character to Serial IO */
}

union UINT_UCHAR{
		unsigned int intCount;
		unsigned char charCount[2];
};    

void initQEI(int mode){//input 2x or 4x, default 2x
	ANSEL0bits.ANS3 = 0;
	ANSEL0bits.ANS4 = 0;
	TRISA |= 0b00011000;
	if (mode == 4){
		QEICON = 0b10111000;
	}
	else{
		QEICON = 0b10101000;
	}
	POSCNTH = 0;
	POSCNTL = 0;
	MAXCNTH = 0xFF;
	MAXCNTL = 0xFF;
}

unsigned int getQEIPos(void){
	union UINT_UCHAR qeiPos;
	
	qeiPos.charCount[0] = POSCNTL;
	qeiPos.charCount[1] = POSCNTH;
	
	return qeiPos.intCount;
}

void setQEIPos(unsigned int pos){
	union UINT_UCHAR qeiPos;
	qeiPos.intCount = pos;
	POSCNTH = qeiPos.charCount[1];
	POSCNTL = qeiPos.charCount[0];
}

void printmsg(void)
{
    printf("\r\n");
    printf("                        ******************************\r\n");
    printf("                        **-------MOTOR CONTROL------**\r\n");
    printf("                        ** keys 0-9 adjust PWM      **\r\n");
    printf("                        ** key a    print encoder   **\r\n");
    printf("                        ** key <colon>  stop motor  **\r\n");
    printf("                        ******************************\r\n");
    printf("\033[10A"); // move up 10 lines
}


