//This file includes all the added functions for the SecretBox project
#define _XTAL_FREQ 4000000       // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdio.h>
void activateBuzzer(char buzzer);   //activate buzzer noise depending on buzzer
                                    //value given to the function
void updateDisplay (char disNum);   //put disNum on display

void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void)
{
        // Check if interrupt flag for INT0 is set to 1
    if (PIR1bits.INT0IF == 1) {
        if (PORTEbits.RE0 == 0) {
            PORTE = 0x0; //disable SYS_LED
            updateDisplay(50); //disable 7 seg display
            activateBuzzer(2); //emergency buzzer noise
            asm("RESET");
        }
    }
    
    PIR1bits.INT0IF = 0;
    
}

void activateBuzzer(char buzzer) {
    
    if (buzzer == 0) {  //fail noise
        PORTBbits.RB2 = 1;
        __delay_ms(500);
        PORTBbits.RB2 = 0;
        __delay_ms(500);
        PORTBbits.RB2 = 1;
        __delay_ms(500);
        PORTBbits.RB2 = 0;
    }
    else if (buzzer == 1) { //success noise
        PORTBbits.RB2 = 1;
        __delay_ms(250);
        PORTBbits.RB2 = 0;
        __delay_ms(250);
        PORTBbits.RB2 = 1;
        __delay_ms(250);
        PORTBbits.RB2 = 0;
    }
    else if (buzzer == 2) { //emergency noise
        for (int i = 0; i < 6; ++i) {
            PORTBbits.RB2 = 1;
            for (int j = 0; j < i; ++j)
            __delay_ms(50);
            PORTBbits.RB2 = 0;
            for (int k = 0; k < i; ++k)
            __delay_ms(50);
        }
        
    }
    else if (buzzer == 9) { //testing noise
        PORTBbits.RB2 = 1;
        __delay_ms(50);
        PORTBbits.RB2 = 0;
    }
    return;
}

void updateDisplay(char disNum) {
    /*
    DB	    0x3F    ;0
    DB	    0x06    ;1
    DB	    0x5B    ;2
    DB	    0x4F    ;3
    DB	    0x66    ;4
    DB	    0x6D    ;5
 */
    if (disNum == 0) {
        PORTD = 0x3F;
    }
    else if (disNum == 1) {
        PORTD = 0x06;
    }
    else if (disNum == 2) {
        PORTD = 0x5B;
    }
    else if (disNum == 3) {
        PORTD = 0x4F;
    }
    else if (disNum == 4) {
        PORTD = 0x66;
    }
    else if (disNum == 5) {
        PORTD = 0x6D;
    }
    else if (disNum == 50) { //disable the 7 seg display
        PORTD = 0;
    }
    return;
}