/*
 * ---------------------
 * Title: Simple Calculator
 * ---------------------
 * Program Details:
 *  This program runs a simple calculator circuit
 * Inputs:
 *      PORTB (Keypad)
 * Outputs:
 *      PORTD (LEDs)
 * Setup: C- Simulator
 * Compiler: xc8 v2.46
 * Author: Mathew Teague
 * Versions:
 *      V0.1: 4/7/24 - Test version. This code outputs which keypad button 
 *                     is pressed to the output LEDs.
 *      V1.0: 4/7/24 - First working version. The program is nearly ready to be
 *                     shipped, just need to add comments.
 *      V1.1: 4/9/24 - Added comments. Updated code for the final button press
 *                     to match the other buttons in the sequence.
 *      V1.2: 4/10/24 - Moved the code for performing the operation out of main()
 *                      and into it's own function that is called by main().
 *                      Added more comments.
*/


#include <xc.h> // must have this
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"
#include "/CalculatorHeader.h"

#define _XTAL_FREQ 4000000       // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned char keyPress(void);   //check which key is pressed, if any
char performOperation( char opReg, char xInReg, char yInReg );
//perform an operation based on the values entered with the keypad


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
    while  (button == 16) {     //wait for key press
        button = keyPress();
    }
    if (button > 9)         //check for valid input (0-9)
        return;
    xInReg = button;        //store x value
    PORTD = 1;             //input 1 LED on
    __delay_ms(200);
    
    button = keyPress();    //operator
    while  (button == 16) {     //wait for key press
        button = keyPress();
    }
    if (button <= 9)        // check for valid input (a-d))
        return;
    if (button >= 14)
        return;
    opReg = button;         //store operator value
    __delay_ms(200);
    
    button = keyPress();    //digit 2
    while  (button == 16) {     //wait for key press
        button = keyPress();
    }
    if (button > 9)         // check for valid input (0-9)
        return;
    yInReg = button;        //store value
    PORTD = 2;             // input 2 LED on (FIX)
    __delay_ms(200);
    
//    while (button != 22) {
//        button = keyPress();  // Original code
//    }
    button = keyPress();
    while  (button == 16) {     //wait for key press
        button = keyPress();
    }
    if (button != 22)       // check for valid key press (#)
        return;
    
    PORTD = performOperation(opReg, xInReg, yInReg);     //output to LEDs
    
    __delay_ms(200);
    button = keyPress();    //wait for key press
    while (button == 16)
        button = keyPress();
    return;
}

unsigned char keyPress () {
    unsigned char keypad[4][4] = {1,2,3,10,     //this represents the keypad
                                  4,5,6,11,
                                  7,8,9,12,
                                  21,0,22,13};
    unsigned char temp = 0, COL = 0, ROW = 0;
    __delay_ms(100);
    temp = PORTB;
    temp ^= 0xF0;               //only the grounded pin will be recorded in temp
    if(!temp) return 16;
    while(temp<<= 1) COL ++;    //check which column was pressed by shifting
                                //temp until it equals zero
    PORTB = 0xFE;
    if(PORTB != 0xFE)           //these if statements find which row was pressed
        ROW = 0;                //by grounding each row one by one
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
    return keypad[ROW][COL];    //return the key that was pressed
       
}

char performOperation( char opReg, char xInReg, char yInReg ) {
    char disReg = 0;
    if (opReg == 10) {      //addition
        disReg = xInReg + yInReg;
    }   
    else if (opReg == 11)    {      //subtraction
        disReg = xInReg - yInReg;
    }
    else if (opReg == 12)    {      //multiplication
        disReg = xInReg * yInReg;
    }
    else if (opReg == 13)    {      //division
        disReg = xInReg / yInReg;
    }
    return disReg;  //return result of operation
}
