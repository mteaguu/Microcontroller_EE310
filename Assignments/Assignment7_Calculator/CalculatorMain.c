/*
 * ---------------------
 * Title: Simple Calculator
 * ---------------------
 * Program Details:
 *  This program runs a simple calculator circuit
 * Inputs:
 *      PORTB
 * Outputs:
 *      PORTD
 * Setup: C- Simulator
 * Compiler: xc8, 2.46
 * Author: Mathew Teague
 * Versions:
 *      V0.1: 4/7/24 - Test version. This code outputs which keypad button 
 *                     is pressed to the output LEDs.
*/


#include <xc.h> // must have this
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"
#include "/CalculatorHeader.h"

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned char keyPress();


void main (void) {
    TRISD = 0;          //portD is used for outputs
    ANSELD = 0;         //portD is digital
    TRISB = 0xF0;       //portB 4-7 is used for inputs, 0-3 for outputs
    WPUB = 0xFF;           //enable weak pull-up resistors for portB
    ANSELB = 0;         //portB is digital
    PORTB= 0xF0;
    char xInReg = 0;    //x input register
    char yInReg = 0;    //y input register
    char opReg = 0;     //Operation Register
    char disReg = 0;    //Display Register
                        //display = x operation y
    unsigned char button = 0;
    button = keyPress();
    while  (button == 16) {
        button = keyPress();
    }
    
    //__delay_ms(2000);
    return;
    /*
    xInReg = keyPress();
    opReg = keyPress();
    yInReg = keyPress();
    if (opReg == 0xA) {
        disReg = xInReg + yInReg;
    }   
    else if (opReg == 0xB)    {
        disReg = xInReg - yInReg;
    }
    else if (opReg == 0xC)    {
        disReg = xInReg * yInReg;
    }
    else if (opReg == 0xD)    {
        disReg = xInReg / yInReg;
    }
    else    {   //invalid opReg input
        
    }
    */
}

unsigned char keyPress () {
    unsigned int keypad[4][4] = {0,1,2,3,
                                  4,5,6,7,
                                  8,9,10,11,
                                  12,13,14,15};
    unsigned char temp, COL = 0, ROW = 4;
    __delay_ms(15);    
    temp = PORTB;
    temp ^= 0xF0;
    if(!temp) return 16;
    while(temp<<= 1) COL ++;
    PORTB = 0xFE;
    if(PORTB != 0xFE)
        ROW = 0;
    else {
        PORTB = 0xFD;
        if(PORTB != 0xFD)
            ROW = 1;
        else {
            PORTB = 0xFB;
            if(PORTB != 0xFB)
                ROW = 2;
                else {
                    PORTB = 0xF7;
                    if(PORTB != 0xF7)
                        ROW = 3;
                    else {
                        return 16;
                    }
                }
            }
        }
        return keypad[ROW][COL];
    
    
}