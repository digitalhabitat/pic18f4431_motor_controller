/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

int timer0_var=0;
int mode=9;
int temp;

unsigned int qei_var, qei_temp;

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

//#include "qei.h"

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include <stdio.h>
#endif

#include "motor_pwm.h"
#include "user.h"
//

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* High-priority service */

#if defined(__XC) || defined(HI_TECH_C)
void __interrupt(high_priority) high_isr(void)
#elif defined (__18CXX)
#pragma code high_isr=0x08
#pragma interrupt high_isr
void high_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif

{

      /* This code stub shows general interrupt handling.  Note that these
      conditional statements are not handled within 3 seperate if blocks.
      Do not use a seperate if block for each interrupt flag to avoid run
      time errors. */

#if 1
    
      /* TODO Add High Priority interrupt routine code here. */

      /* Determine which flag generated the interrupt */
      
      /*if(<Interrupt Flag 1>)
      {
          <Interrupt Flag 1=0>; Clear Interrupt Flag 1
      }
      else if (<Interrupt Flag 2>)
      {
          <Interrupt Flag 2=0>; Clear Interrupt Flag 2 
      }
      else
      {
           Unhandled interrupts
      }*/
    
    //////////////////////////////////////////////////////////////
    /* USART RECIEVE INTERRUPT */    
    if( PIR1bits.RCIF == 1)
    {
        long DutyCycle;
        char data_rcv;
        PORTBbits.RB5 = ~PORTBbits.RB5;
        while(!TXSTAbits.TRMT );// Wait until empty
        data_rcv = RCREG; // echo back character
        
        /// PWM CONTROL INPUT    
        if (data_rcv >= 48 && data_rcv <= 58 )
        {
            mode = 1;
            printf("\033[8A"); // move up 5 lines
            printf("\033[2J"); // Clear screen
            printf("character received: %c\r\n",data_rcv);
            DutyCycle = (data_rcv - '0')*10;
            printf("PWM0 duty cycle set to: %d\r\n",DutyCycle);
            setPDC0(DutyCycle);
            printmsg();
        }
        /// ENCORDER DATA REQUEST
        else if (data_rcv == 'a')
        {
            
            printf("\033[8A"); // move up 5 lines
            printf("\033[2J"); // Clear screen
            qei_var = getQEIPos(); 
            printf("Encoder Count %x\r\n", qei_var);
            printmsg();
        }
        /// TOGGlE REQUEST
        else if (data_rcv == 'b')
        {
            printf("\033[8A"); // move up 5 lines
            printf("\033[2J"); // Clear screen
            PORTDbits.RD6 = ~PORTDbits.RD6;
            PORTDbits.RD5 = ~PORTDbits.RD5;
            printf("Toggled Motor Direction\r\n");
            printmsg();
            
        }  
        else
        {
            printf("\033[8A"); // move up 8 lines
            printf("\033[2J"); // Clear screen
            printf("character received: %c\r\n",data_rcv);
            printf("No action assigned for is character \r\n");
            printf("Going into demo mode\r\n");
            printmsg();
            mode = 9;
        }
         
        PIR1bits.RCIF == 0;       
    }
    //////////////////////////////////////////////////////////////
    /* TIMER0 INTERRUPT */
    else if(INTCONbits.T0IF == 1)
    {
        // Toggle LED on interrupt
        PORTDbits.RD6 = ~PORTDbits.RD6;       
        timer0_var++;
        
        // Pulse up-down mode 0-50% duty cycle
        if (mode == 9)
        {
            if (timer0_var <= 100)
            {
            setPDC0(timer0_var);
            }
            else 
            {
            setPDC0(200-timer0_var);
            }
        }
        
        
        // Toggle LED on pulse period
        if(timer0_var == 200)
        {
            timer0_var = 0;
            PORTDbits.RD7 = ~PORTDbits.RD7;         
        }
        
        // Reload Timer 
        TMR0 = 0;
         
        // Reset Interrupt Flag
        INTCONbits.T0IF = 0;
    }
    
    

    
#endif

}

/* Low-priority interrupt routine */
#if defined(__XC) || defined(HI_TECH_C)
void __interrupt(low_priority) low_isr(void)
#elif defined (__18CXX)
#pragma code low_isr=0x18
#pragma interruptlow low_isr
void low_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
{

      /* This code stub shows general interrupt handling.  Note that these
      conditional statements are not handled within 3 seperate if blocks.
      Do not use a seperate if block for each interrupt flag to avoid run
      time errors. */

#if 0

      /* TODO Add Low Priority interrupt routine code here. 

      // Determine which flag generated the interrupt 
      if(<Interrupt Flag 1>)
      {
          <Interrupt Flag 1=0>; // Clear Interrupt Flag 1 
      }
      else if (<Interrupt Flag 2>)
      {
          <Interrupt Flag 2=0>; // Clear Interrupt Flag 2 
      }
      else
      {
          / Unhandled interrupts 
      }
      */
              
      /* TIMER0 INTERRUPT */
    if(INTCONbits.T0IF == 1)
    {
        //PDC0L = timer0_var;
        //setPDC0((long)timer0_var);
        
        timer0_var++;
        TMR0 = 0; // reload timer value
        qei_var = getQEIPos();
        if (qei_var != qei_temp) // if change in qei_var
        {
            qei_temp = qei_var;
            printf("Encoder Count %x\r\n", qei_var);
        }    
        // invert the LED
        if (timer0_var == 0)
        {
            //PORTB = 0;
            //PORTBbits.RB0 = 1;
        } 
        else if(timer0_var == 1)
        {
            //PORTB = 0;
            //PORTBbits.RB1 = 1;
        } 
        else if(timer0_var == 10)
        {
            timer0_var = 0;
            PORTDbits.RD7 = ~PORTDbits.RD7;
            
        }
        
        // Reset Interrupt Flag
    
        INTCONbits.T0IF = 0;
    }
#endif

}
