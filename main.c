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
//#include "user.c"
/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void test(void);


void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();
    //init_pwm();
    test();
    /* TODO <INSERT USER APPLICATION CODE HERE> */
    
    char x;
    int val;
    val = 96;
    
    while(1)
    {

        printf("timer: %x\r\n", TMR0);
       /* PORTBbits.RB0 = 1;s
        __delaywdt_ms(100);
        __delaywdt_ms(100);
        __delaywdt_ms(100);
        __delaywdt_ms(100);
        __delaywdt_ms(100);
        PORTBbits.RB0 = 0;
        __delaywdt_ms(100);
        __delaywdt_ms(100);
        __delaywdt_ms(100);
        __delaywdt_ms(100);
        __delaywdt_ms(100);
       T0CONbits.TMR0ON = !T0CONbits.TMR0ON;
        */
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
    PTCON0=0b00001100; //1:1 postcalw,  1:64 prescale  ,free-running mode     
    PTCON1=0b10000000; //PWM time base timer      
    PWMCON0=0b00110111;// ,PWM0 and PWM1 in complementary mode  
    PWMCON1=0b00000000;      
    PTPERH=0x01;       
    PTPERL=0x38; 
    PDC0L=0b00110100;     
    PDC0H=0b00000011;
}
