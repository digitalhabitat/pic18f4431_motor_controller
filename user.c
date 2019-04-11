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
    
    /* Initialize peripherals */
    init_usart();
    init_timer();

    /* Configure the IPEN bit (1=on) in RCON to turn on/off int priorities */

    /* Enable interrupts */
}

void init_usart(void)
{
      // Setup UART
    TRISCbits.RC6 = 0;   // TX as output
    TRISCbits.RC7 = 1;   // RX as input
    
    TXSTA1bits.SYNC = 0;    // Async operation
    TXSTA1bits.TX9 = 0;     // No tx of 9th bit
    TXSTA1bits.TXEN = 1;    // Enable transmitter
    
    RCSTA1bits.RX9 = 0;     // No rx of 9th bit
    RCSTA1bits.CREN = 1;    // Enable receiver
    RCSTA1bits.SPEN = 1;    // Enable serial port
    
    // Setting for 9600 BPS
    BAUDCONbits.BRG16 = 0;  // Divisor at 8 bit
    TXSTA1bits.BRGH = 0;    // No high-speed baudrate
    SPBRG1 = 129;           // divisor for 9600 baud with 20MHz FOSC (TABLE 20-3:)
    
    // enable USART Interrupt
    // PIE1bits.TXIE = 1;
    // INTCONbits.GIE = 1;
    // INTCONbits.PEIE = 1;  
}

void init_timer(void)
{
    
    T0CONbits.TMR0ON = 0; // stop the timer
    T0CONbits.T016BIT = 0; // timer configured as 16-bit
    T0CONbits.T0CS = 0; // use internal clock
    T0CONbits.PSA = 0; // use prescaler
    T0CONbits.T0PS = 0b011; // 1:256 prescale value
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
    PDC0H = 0x00;               // PWM Duty Cycle    
    PDC0L = 0xFF;
    
    PTCON0bits.PTOPS = 0b000;   // 1:1 Output postscale
    PTCON0bits.PTCKPS = 0b00;   // 1:1 Input prescale (Fosc/4)
    PTCON0bits.PTMOD = 0b00;    // Free-Running mode
    
    PTPERL = 0xFF;
    PTPERH = 0x0F;              // PWM period = (PTPER+1)*PTPMRPS/(0.25*Fosc)
    
    PWMCON0bits.PWMEN = 0b010;  // PWM0 and PWM1 pins are enabled for PWM output
    PWMCON0bits.PMOD0 = 0;      // pins PWM0 PWM1 is in the Complementary mode
}

static int PWMPeriod; //PTMR will count until this value


static void PWMError(void){
	//PWM encountered an error, enter trap state and flash RB5 LED
	unsigned int i;
	TRISB = 0;
	PWMCON0 = 0; //disable PWM module
	printf("PWM Error");
	while(1){
		//Flash the LED to indicate an error
		PORTBbits.RB5 = 1;
		__delaywdt_ms(_XTAL_FREQ/5000);
		PORTBbits.RB5 = 0;
		__delaywdt_ms(_XTAL_FREQ/5000);
	}
}

void enablePWM(char PWM_BITMASK){
/*
Use & (bitwise AND) in the function call to overlay the bit masks.
Example:
	enablePWM(	PWM_EN_ALL &
				PMOD0_COMPLEMENTARY &
				PMOD3_COMPLEMENTARY);
*/

//select PWM pins to enable (only use one bitmask from this group):
#define PWM_EN_ODD 			0b01111111
#define PWM_EN_1_3			0b01101111
#define PWM_EN_ALL			0b01011111
#define PWM_EN_0_1_2_3_4_5	0b01001111
#define PWM_EN_0_1_2_3		0b00111111
#define PWM_EN_0_1			0b00101111
#define PWM_EN_1			0b00011111
#define PWM_EN_NONE			0b00001111

//select PWM modules to use complementary mode.  Otherwise,
//PWM modules will be in independent mode by default.
//select as many as needed:
#define PWM_01_COMPLEMENTARY	 	0b01111110 //PMOD0 (Pins PWM0, PWM1)
#define PWM_23_COMPLEMENTARY		0b01111101 //PMOD1 (Pins PWM2, PWM3)
#define PWM_45_COMPLEMENTARY		0b01111011 //PMOD2 (Pins PWM4, PWM5)
#define PWM_67_COMPLEMENTARY		0b01110111 //PMOD3 (Pins PWM6, PWM7)

	PWMCON0=0b11111111 & PWM_BITMASK; //0b00110000 enables PWM 0,1,2,3 in complementary mode
}


void configPWMFreq(unsigned long freq){
	
	int timeBasePreScale; //set time base prescale
	long tempLong;
	int scale;
	
	if (freq > (_XTAL_FREQ/4/2)){
		PWMError(); //frequency is too high, period has less than 1 bit resolution!
					//enter error state.
	}
	else if (freq > (_XTAL_FREQ/16384)){	//(FOSC/4)/4096){
		timeBasePreScale = 1;
		PTCON0 |= 0b00000000;		
	}
	else if (freq > (_XTAL_FREQ/65536)){	//(FOSC/(4*4))/4096){
		timeBasePreScale = 4;
		PTCON0 |= 0b00000100;
	}
	else if (freq > (_XTAL_FREQ/262144)){	//(FOSC/(4*16))/4096){
		timeBasePreScale = 16;
		PTCON0 |= 0b00001000;
	}
	else if (freq > (_XTAL_FREQ/1048576)){	//(FOSC/(4*64))/4096){
		timeBasePreScale = 64;
		PTCON0 |= 0b00001100;
	}
	else {
		//frequency is too low, trap state here and flash LED
		PWMError();
	}
	
	//PTCON0 set to 1:1 postscale, prescale per above, and
	//free running mode.
	PTCON1 = 0b10000000;  //time base enabled, counts upwards
	
	//calculate PWM period register value from FOSC,
	//PWM frequency, and prescaler values
	scale = timeBasePreScale*4;
	tempLong = (_XTAL_FREQ/scale);
	tempLong = tempLong/freq;
	PWMPeriod= tempLong-1;
	PTPERH=PWMPeriod/256; //split into high and low registers
	PTPERL=PWMPeriod%256;
}

void setPDC0(long dutyCyclePercent)
{
    int dutyCycle = PWMPeriod*(50+100)/200;
    int dutyCycleQC = dutyCycle*4;
    PDC0H = dutyCycleQC/256;
    PDC0L = dutyCycleQC%256;
    
}


void putch(unsigned char byte) {
    TXSTA = 0x26;
    RCSTA1bits.SPEN = 1;
    while (!TXIF)continue;
    TXREG = byte; /* transmit a character to Serial IO */
}



