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
 *      V1.0: 4/7/24 - First working version. The program is nearly ready to be
 *                     shipped, just need to add comments.
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
    PORTD = 0;
    TRISB = 0xF0;       //portB 4-7 is used for inputs, 0-3 for outputs
    WPUB = 0xFF;           //enable weak pull-up resistors for portB
    ANSELB = 0;         //portB is digital
    PORTB= 0xF0;        //assume portB inputs start high
    char xInReg = 0;    //x input register
    char yInReg = 0;    //y input register
    char opReg = 0;     //Operation Register
    char disReg = 0;    //Display Register
                        //display = x operation y
    unsigned char button = 0;
    
    button = keyPress();    //first digit
    while  (button == 16) {
        button = keyPress();
    }
    if (button > 9) 
        return;
    xInReg = button;
    PORTD = 1;             //input 1 LED on
    __delay_ms(200);
    
    button = keyPress();    //operator
    while  (button == 16) {
        button = keyPress();
    }
    if (button <= 9)
        return;
    if (button >= 14)
        return;
    opReg = button;
    //PORTD = button;             //for testing (FIX)
    __delay_ms(200);
    
    button = keyPress();    //digit 2
    while  (button == 16) {
        button = keyPress();
    }
    if (button > 9) 
        return;
    yInReg = button;
    PORTD = 2;             // input 2 LED on (FIX)
    __delay_ms(200);
    
    while (button != 22) {
        button = keyPress();
    }
    
    if (opReg == 10) {
        disReg = xInReg + yInReg;
    }   
    else if (opReg == 11)    {
        disReg = xInReg - yInReg;
    }
    else if (opReg == 12)    {
        disReg = xInReg * yInReg;
    }
    else if (opReg == 13)    {
        disReg = xInReg / yInReg;
    }
    
    PORTD = disReg;
    __delay_ms(200);
    button = keyPress();
    while (button == 16)
        button = keyPress();
    return;
}

unsigned char keyPress () {
    unsigned int keypad[4][4] = {1,2,3,10,
                                  4,5,6,11,
                                  7,8,9,12,
                                  21,0,22,13};
    unsigned char temp = 0, COL = 0, ROW = 0;
    __delay_ms(100);    
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
    PORTB = 0xF0;       //reset portB
    return keypad[ROW][COL];
    
    
}
