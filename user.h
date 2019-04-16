/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */
void init_usart(void);      /* USART Initialization */
void init_timer(void);      /* Timer Initialization */
void init_pwm(void);        /* PWM Initialization */
void enablePWM(char);
void configPWMFreq(unsigned long freq);
void setPDC0(long dutyCyclePercent);
void initQEI(int mode);
unsigned int getQEIPos(void);
void setQEIPos(unsigned int pos);

