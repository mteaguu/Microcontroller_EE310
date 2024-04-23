//This header contains all the init values for the SecretBox project

#include <xc.h> // include processor files - each processor file is guarded.  
//#include <stdio.h>
////#include <string.h>
//#include <stdlib.h>
//PORTD is for 7 seg (OUTPUT)
//PORTE.1 is for sys_led (OUTPUT)
//PORTB.2 is for active buzzer (OUTPUT)
//PORTB.1 is for relay input (OUTPUT)
//PORTB.0 is for emergency switch, 0 V when pressed, ENABLE WPU (INPUT)
//PORTA.0 and 1 are for the photo resistors (INPUT)


void initFunc ( void ){
    TRISD = 0;          //RD0 - RD7 are output for 7 seg display
    ANSELD = 0;         //portD is digital
    PORTD = 0;
    
    TRISE = 0;      //RE1 is output for sys_led
    ANSELE = 0;
    PORTE = 0;
    
    TRISBbits.TRISB0 = 1;    //RB0 is input for emergency switch
    TRISBbits.TRISB1 = 0;    //RB1 is output for relay input
    TRISBbits.TRISB2 = 0;   //RB2 is output for buzzer
    //TRISB = 0x01
    WPUBbits.WPUB0 = 1;     //enable WPU for emergency switch
    //WPUB = 0x01;
    ANSELB = 0;
    PORTBbits.RB0 = 1;
    PORTBbits.RB1 = 0;
    PORTBbits.RB2 = 0;
    //PORTB = 0x01;
    
    TRISA = 0xFF;   //used for photo resistors
    ANSELA = 0;
    PORTA = 0;
    WPUA = 0;
    
    INTCON0bits.IPEN = 1;    //enables interrupt priority
    INTCON0bits.GIEH = 1;   //enables high priority interrupts
    INTCON0bits.GIEL = 1;   //enables low priority interrupts
    INTCON0bits.INT0EDG = 1;    //interrupt on rising edge
    IPR1bits.INT0IP = 1;        // enable high priority interrupts for INT0
   // IPR0bits.IOCIP = 1;
    PIE1bits.INT0IE = 1;        //enable external interrupt 0
    //PIE0bits.IOCIE = 1;
    
    //Clear interrupt flag for INT0
    PIR1bits.INT0IF = 0;

    IVTBASEU = 0x00;    //These regs hold the address of the interrupt vector
    IVTBASEH = 0x40;
    IVTBASEL = 0x08;
}