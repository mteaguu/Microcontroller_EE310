/*
 * ---------------------
 * Title: Interfacing with Sensors
 * ---------------------
 * Program Details:
 *  This program is an exercise in interfacing with peripherals
 * Inputs:
 *  PORTB.0 is for emergency switch
 *  PORTA.0 and 1 are for the photo resistors
 * Outputs:
 *  PORTD is for 7 segment display
 *  PORTE.1 is for sys_led 
 *  PORTB.2 is for active buzzer 
 *  PORTB.1 is for relay input 
 * Setup: C- Simulator
 * Compiler: xc8 v2.46
 * Author: Mathew Teague
*/

#include <xc.h> // must have this
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"
#include <stdio.h>
//#include <string.h>
#include <stdlib.h>
//#include <math.h>
#include "./SecretConfigHeader.h"
#include "./SecretInitHeader.h"
#include "./SecretFunctionHeader.h"

#define _XTAL_FREQ 4000000       // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4


//void activateBuzzer(char buzzer);   //activate buzzer noise depending on buzzer
                                    //value given to the function
//void updateDisplay (char disNum);   //put disNum on display
char buzzer = 0;
char disNum = 0;
char secret_code = 0x12;    //secret code
char sim = 1;   //this will remove delays if sim = 0


void main(void) {
    initFunc();
    //PORTE = 0x2; //enable SYS_LED
    char entered_code = 0x00;   //code entered from photo resistors
    char entered_code_digit = 0;    //for upper nibble of entered_code
    char delayTimer = 0;    //tracks length of delay
    
    updateDisplay(0);       //put 0 on 7 seg display until first input
    PORTE = 0x2;        //SYS_LED turned on
    while (PORTAbits.RA2 == 0);    //delay until photo resistor 1 is covered
    
    //activateBuzzer(9); //for testing
    while (delayTimer < 21) {//this loop tracks how many times the  first photo
        if (PORTAbits.RA2 == 1) {   //resistor has been covered
            ++entered_code; 
            updateDisplay(entered_code); //put current entry on display;
            if (sim) __delay_ms(1000);    //delay 0.5 seconds to avoid double input
            delayTimer = 0;     //reset input delay
        }
        if (sim) __delay_ms(100);
        ++delayTimer;   //increment delayTimer
    }                   //after 2 seconds of no input, the loop ends
    
    delayTimer = 0;     //restart timer variable
    updateDisplay(0);   //put 0 on display to represent new input
    
    while (PORTAbits.RA3 == 0);    //delay until photo resistor 2 is covered
    
    while (delayTimer < 21) { //same as previous loop but for photo resistor 2
        if (PORTAbits.RA3 == 1) {
            entered_code += 16; //increment upper nibble of entered_code
            ++entered_code_digit;
            updateDisplay(entered_code_digit); //put current entry on display;
            if (sim) __delay_ms(1000);
            delayTimer = 0;
        }
        if (sim) __delay_ms(100);
        ++delayTimer;
    }
    updateDisplay(0);
    if (secret_code != entered_code) {
        activateBuzzer(0);  //buzzer fail noise
        return; //restart
    }
    
    //activateBuzzer(1);  //buzzer success noise
    PORTEbits.RE0 = 1;
    PORTBbits.RB1 = 1;    //activate relay to turn on motor
    if (sim) __delay_ms(5000);
    PORTBbits.RB1 = 0;
    PORTEbits.RE0 = 0;
    PORTE = 0;      //turn off SYS_LED
    return; //restart
    
}

