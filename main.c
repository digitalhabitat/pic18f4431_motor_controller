/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
//#include "motor_pwm.h"
/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void test(void);
char * dec2hex(int value);


void     main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();
    //init_pwm();
    //test();
    enablePWM(0b00101111 & 0b01111110);
    configPWMFreq(10e3);
            
    /* TODO <INSERT USER APPLICATION CODE HERE> */
    
    char x;
    short int val;
    val = 96;
    
    while(1)
    {

       // PORTBbits.RB0 = 1;s
        __delaywdt_ms(100);
        __delaywdt_ms(100);
        __delaywdt_ms(100);
        __delaywdt_ms(100);
        __delaywdt_ms(100);
       /// PORTBbits.RB0 = 0;
        __delaywdt_ms(100);
        __delaywdt_ms(100);
        __delaywdt_ms(100);
        __delaywdt_ms(100);
        __delaywdt_ms(100);
       //T0CONbits.TMR0ON = !T0CONbits.TMR0ON;
       // */
    }

}

void test(void){
    PORTA=0;        
    PORTB=0;   
    PORTD=0;       
    PORTE=0;    
    TRISB=0b00000000;     
    TRISD=0b00000000;   
    LATA=0b00110111; 
    LATB=0b00000000; 
    LATD=0b00000000; 
    LATE=0b00000011; 
    
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
    PTPERH=0b0000;        // 4 bits wide    
    PTPERL=0xFF;        // 8 bits wide
    
    // PWM Duty Cycle 14-bit resolution 
    PDC0H=0b000000;   // 6 bits wide
    PDC0L=0b0000000;   // 8 bits wide 
}
