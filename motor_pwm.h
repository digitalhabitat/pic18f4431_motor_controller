/* 
 * File:   motor_pwm.h
 * Author: mike
 *
 * Created on April 14, 2019, 4:17 PM
 */

#ifndef MOTOR_PWM_H
#define	MOTOR_PWM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "system.h"
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
	else if (freq > (_XTAL_FREQ/16384)){	//(_XTAL_FREQ/4)/4096){
		timeBasePreScale = 1;
		PTCON0 |= 0b00000000;		
	}
	else if (freq > (_XTAL_FREQ/65536)){	//(_XTAL_FREQ/(4*4))/4096){
		timeBasePreScale = 4;
		PTCON0 |= 0b00000100;
	}
	else if (freq > (_XTAL_FREQ/262144)){	//(_XTAL_FREQ/(4*16))/4096){
		timeBasePreScale = 16;
		PTCON0 |= 0b00001000;
	}
	else if (freq > (_XTAL_FREQ/1048576)){	//(_XTAL_FREQ/(4*64))/4096){
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
	
	//calculate PWM period register value from _XTAL_FREQ,
	//PWM frequency, and prescaler values
	scale = timeBasePreScale*4;
	tempLong = (_XTAL_FREQ/scale);
	tempLong = tempLong/freq;
	PWMPeriod= tempLong-1;
	PTPERH=PWMPeriod/256; //split into high and low registers
	PTPERL=PWMPeriod%256;
}


//set duty cycles, input a value from -100 (0% duty cycle) to 100 
// (100% duty cycle).  0 = 50% duty cycle.  If used with the PWM module
// in complementary mode, the duty cycles will be inverses of each other.
void setPDC0 (long dutyCyclePercent){
	int dutyCycle = PWMPeriod*(dutyCyclePercent+100)/200;
	int dutyCycleQC=dutyCycle*4;
	PDC0H=dutyCycleQC/256;
	PDC0L=dutyCycleQC%256;
    printf("dutycyclePrecent = %li \r\n", dutyCyclePercent);
    printf("dutycyle = %d \r\n", dutyCycle);
    printf("PDCOH: %d\r\n", dutyCycleQC/256);
    printf("PDCOL: %d\r\n", dutyCycleQC%256);
}

void setPDC1 (long dutyCyclePercent){
	int dutyCycle = PWMPeriod*(dutyCyclePercent+100)/200;
	int dutyCycleQC=dutyCycle*4;
	PDC1H=dutyCycleQC/256;
	PDC1L=dutyCycleQC%256;
}

void setPDC2 (long dutyCyclePercent){
	int dutyCycle = PWMPeriod*(dutyCyclePercent+100)/200;
	int dutyCycleQC=dutyCycle*4;
	PDC2H=dutyCycleQC/256;
	PDC2L=dutyCycleQC%256;
}

void setPDC3 (long dutyCyclePercent){
	int dutyCycle = PWMPeriod*(dutyCyclePercent+100)/200;
	int dutyCycleQC=dutyCycle*4;
	PDC3H=dutyCycleQC/256;
	PDC3L=dutyCycleQC%256;
}    



#ifdef	__cplusplus
}
#endif

#endif	/* MOTOR_PWM_H */

