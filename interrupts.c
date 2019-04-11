/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/
int timer0_var=0;

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
#include <stdio.h>
#endif

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

    if(INTCONbits.T0IF == 1)
    {
        timer0_var++;
        printf("timer!\r\n");
        TMR0 = 0; // reload timer value
        
        
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
        else if(timer0_var == 2)
        {
            timer0_var = 0;
            PORTDbits.RD7 = ~PORTDbits.RD7;
        }
        
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

      /* TODO Add Low Priority interrupt routine code here. */

      /* Determine which flag generated the interrupt */
      if(<Interrupt Flag 1>)
      {
          <Interrupt Flag 1=0>; /* Clear Interrupt Flag 1 */
      }
      else if (<Interrupt Flag 2>)
      {
          <Interrupt Flag 2=0>; /* Clear Interrupt Flag 2 */
      }
      else
      {
          /* Unhandled interrupts */
      }

#endif

}
