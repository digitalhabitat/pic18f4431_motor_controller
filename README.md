# pic18f4431_motor_controller

### Schematic 
https://easyeda.com/miketm89/pic18f4431_motor_controller

## DC MOTOR CONTROLLER 

### Project Report

By

Michael Miller

Indiana University-Purdue University Indianapolis

ECE 47100 - Spring 2019

Department of Electrical and Computer Engineering

Dr. Stanley Chien

schien<span></span>@iupui.edu

April 23, 2019

### INTRODUCTION

The goal of the project is to implement a DC motor controller with the PIC18F4431 microcontroller. The PIC18F4431 was chosen specifically because of the specialized modules targeted for motor control. The Motion Feedback Module can utilize a 3-Channel Input Capture (IC) module and Quadrature encoder Interface (QEI). These modules essentially be utilized to create a closed loop control system which was to original goal of the project. The PIC18F4431 also comes with several Pulse width modulation (PWM) modules. This allows the controlling several DC motors or three phase motors as well.

### REQUIREMENT OF THE PROJECT

The requirement of the project is to utilize at least two peripherals and demonstrate the practical functionality of modules on the PIC18 microcontroller such as PWM, Timer, USART, RTI, and Input Capture.

### THE DESIGN DETIALS

This project simply controls two PWM channels on the microcontroller by sending characters through a terminal over USART. There is a single timer module implemented as in real time interrupt, for blinking LEDs and running a basic motor routine of speeding and slowing down the motor via incrementing/decrementing the PWM duty cycle. The user interface is a computer terminal connected to the microcontroller via a USB cord. The microcontroller is powered by the USB cord from the user&#39;s computer. The user has three options, selecting PWM duty cycle, request motor encoder count value, and toggle motor direction. If a character that is not assigned with an operation the program will run the demo routine mentioned earlier, which speeds up and down the motor.

#### 1.1 Schematic diagram with explanations

![images](https://github.com/digitalhabitat/pic18f4431_motor_controller/blob/master/images/figure_1.png)

*Figure 1: Schematic Diagram*

The Schematic in Figure 1 illustrates how the PICkit 4 debugger, USB-to-TTL Serial Convertor, DC Motor with Encoders, motor driver, and other elements are connected to the PIC18 microcontroller. The microcontroller during program operation is power through the USB-to-TTL Serial Converter for the User PC. This device is used to send motor commands and read encoder values. The DC motor is connected directly a motor driver that is supplied with a separate 12 volt source. This moto driver receives PWM signals from the microcontroller over the PB0 port. The encoders on the DC motor are connected to pin 5 or QEA and pin 6 or QEB. The USB-to-TTL Serial Convertor TX and RX pins are connect to pins RX and TX respectively. The yellow LED with blink at each timer0 overflow interrupt and the white light connected the same PWM pin that controls the DC motor. The pink LED uses timer0 toggle about every 5 seconds. The diode across the DC motor help with the performance and response. The 20 MHz crystal was not necessary but will improve performance of USART.

#### 1.2 Data structure definitions

    voidConfigureOscillator();
Configures the internal oscillator (not used)

    voidInitApp(void)
Initialize LED ports, calls init\_usart(); and init\_timer();

    voidinit_usart(void)
Initializes USART

    voidinit_timer(void)
Initializes Timer0 with 8-bit register

    voidinit_pwm(void)

Initializes PWM

    voidputch(unsignedchar byte)
Used by printf() to transmit and display character on terminal

    union UINT\_UCHAR
Used by getQEIPos() to hold encoder counter value

    voidinitQEI(int mode)
Initializes QEI interface module

    unsignedintgetQEIPos(void)
Returns the current encoder count value

    voidsetQEIPos(unsignedint pos)
Manually set encoder count value (not used)

    voidprintmsg
Print a short manual on terminal for user to know how key presses control the motor

#### 1.3 Block diagram shows the components and interactions

![images](https://github.com/digitalhabitat/pic18f4431_motor_controller/blob/master/images/figure_2.png)

*Figure 2: Abstract Block Diagram*

#### 1. 4 Flowchart shows the operation sequence

![images](https://github.com/digitalhabitat/pic18f4431_motor_controller/blob/master/images/figure_3.png)

*Figure 3: Main Program and USART Interrupt*

![images](https://github.com/digitalhabitat/pic18f4431_motor_controller/blob/master/images/figure_4.png)

*Figure 4: Timer0 Interrupt*


### IMPLEMENTATION DETIALS

#### 2.1 List of parts and tools (name, model, source of purchasing, price)

![images](https://github.com/digitalhabitat/pic18f4431_motor_controller/blob/master/images/table_1.png)

*Table 1: Bill of Materials*

#### 2.2 Wiring diagram

![images](https://github.com/digitalhabitat/pic18f4431_motor_controller/blob/master/images/figure_1.png)
*Figure 1: Schematic Diagram*

#### 2.3 Layout diagram

![images](https://github.com/digitalhabitat/pic18f4431_motor_controller/blob/master/images/figure_6.png)

*Figure 6: Layout Diagram*

#### 2.4 Software tools (name, version, configuration details)

Hardware Platform: PIC18F4431-I/P

Debuggers Used: MPLAB PICkit 4

Programmers Used: MPLAB PICkit 4

MPLAB Version: v5.15

C Compiler Version: XC8 v2.05

Serial Terminal: Putty

### EXPERIMENT RESULTS

The DC motor controller behaves closely to as expected. When everything is connected and powered on the yellow LED blinks very fast and the Pink LED toggles about every 4 seconds. When the putty terminal is connected a blank screen is shown. A single keypress is necessary to invoke the manual. The first key press is usually not assigned with a motor function so the program will go into demo mode as mentioned before. The program will accept characters &#39;0&#39; to &#39;9&#39; to control the PWM, &#39;:&#39; (colon) to stop the motor and &#39;a&#39; to print encoder count value. Due to the nature of the motor driver the PWM control is opposite of to what is expected. When the duty cycle is 0% the motor is at full speed when the duty cycle is at 100% the motor is stopped.

### DISCUSSION

Configuring the PWM modules was the most difficult part of this project. The original implantation did not perform accordingly to the design. The PWM channels are heavily dependent on the system oscillator. Getting the desired duty cycle and PWM frequency require an exact understanding the complicated math documented in the data sheet. Luckily a header file was already available from similar project source online. This header file took care of the PWM and QEI initialization and configuration.

### CONCULSION

Although the project did not get to implement all project features it did successfully implemented all main functionality. All hardware implementation is complete. Additional feature that could be to implement is a position control using the QEI module.

### REFERENCES

[http://hades.mech.northwestern.edu/index.php/PIC\_Motor\_Control\_and\_Serial\_Port\_Example#Project\_Files](http://hades.mech.northwestern.edu/index.php/PIC_Motor_Control_and_Serial_Port_Example#Project_Files)

http://www.dmi.unict.it/~santoro/teaching/lap1/slides/UART\_MCU.pdf

### APPENDICES

#### 3.1 The specification of all parts

**USB-to-TTL Serial Converter Specification:**

- Standard USB power supply,
-  the lead pin includes: 3.3V, GND, TXD, RXD, 5V, GND

**Qunqi L297N Motor Driver Specification:**

- Chip: L298N
- Logic voltage: 5V
- Logic current 0mA-36mA
- Storage Temperature: -20 ℃ to ℃ to +135
- Operating mode: H-bridge driver (dual)
- Drive voltage: 5V-35V
- Drive current: 2A (MAX single bridge)
- Maximum power: 25W
- Dimensions: 43x43x27mm

**uxcell Gear Motor with Encoder DC 12V Specification:**

- Material: Metal
- Color: Silver
- Rated Voltage: 12V
- Voltage Range: 6-24V
- No-load Speed: 201RPM
- Load speed: 168RPM
- No-load Current: 46mA
- Load Current: 300mA
- Stall Current: 1A
- Torque: 0.53kg.cm
- Reduction Ratio: 21.3:1
- Gearbox Size(D\*L): 25x19mm/0.98x0.75inch
- Motor Size(D\*L): 24.5x30mm/0.96x1.18inch
- Shaft Size(D\*L): 4x10mm/0.16x0.39inch
- Encoder Cable Length: 20cm/7.87inch
- Gross Weight: Approx. 100g

**3.2 Complete software code.**

[https://github.com/digitalhabitat/pic18f4431\_motor\_controller](https://github.com/digitalhabitat/pic18f4431_motor_controller)

**3.3 A video of the product in operation.**

![images](https://github.com/digitalhabitat/pic18f4431_motor_controller/blob/master/images/operation.gif)
