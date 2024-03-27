;-----------------------------------------------
;   Title: 7 Segment Display Counter
;-----------------------------------------------
;Purpose: This program counts either up or down on a 7-seg display
;Dependencies: NONE
;Compiler: MPLAB X IDE v6.20
;Author: Mathew Teague
;OUTPUTS:
;   PortD outputs to 7 seg display
;INPUTS:
;   Button A
;   Button B
;Versions:
;   V1.0: 3/26/24 - First Verison. Most of the code is finished, just need testing
;   V2.0: 3/27/24 - Added code that deals with cycling the counter back to
;		    0 or F
;		  - Added delay code
;		  - Fixed portB setup
;		  - Fixed reset trigger code
;   V2.1: 3/27/24 - Added comments for code that was missing explanation
;---------------------
; Initialization
;---------------------
#include "./AssemblyConfig.inc"
#include <xc.inc>

;---------------------
; Program Inputs
;---------------------
#define	    buttonA	PORTB,1	    ;increment button
#define	    buttonB	PORTB,2	    ;decrement button

;---------------------
; Registers
;---------------------
delayREG    EQU	    0x10    ;used for _DELAY outer loop
delayREGB   EQU	    0x11    ;used for _DELAY inner loop
tbltop	    EQU	    0x12    ;location of F's display code
tblbottom   EQU	    0x13    ;location of 0's display code
   
;---------------------
; Program Outputs
;---------------------
displayOut	EQU	PORTD   ;7 seg display is connected to portD

;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld

    ORG	    0x20
    
    CLRF   TRISD   ;set PORTD bits to all 0 (output)
    BANKSEL ANSELD
    CLRF   ANSELD	;set PORTD to digital
    
    SETF   TRISB   ;set PORTB bits to all 1 (input)
    BANKSEL ANSELB
    CLRF   ANSELB	;set PORTB to digital
    
    MOVLW   0x11
    MOVWF   tbltop	;this is for when TBLPTR moves too high or low
    MOVLW   0x01
    MOVWF   tblbottom
	
_RESET:	    ;point tblptr at 0's 7 seg display code, and put 0 on display
    MOVLW   0x01
    MOVWF   TBLPTRL
    MOVLW   0x04
    MOVWF   TBLPTRH	;point TBLPTR at 0x401 in prog mem, which is
    MOVLW   0x00	;the location of 0's 7 seg display code
    MOVLW   TBLPTRU
    TBLRD*
    MOVFF   TABLAT,displayOut	;move 0's 7 seg display code to display
    CALL    _DELAY
    
_BUTTONCHECK:	;check what buttons are pressed, if any
    BTFSS	buttonA
    GOTO	_INCREMENT  ;button A has been pressed
    BTFSS	buttonB
    GOTO	_DECREMENT  ;button B has been pressed
    GOTO	_BUTTONCHECK	;neither button is pressed
    

_INCREMENT:  ;increment the dispay
    BTFSS   buttonB	;check if button B is also pressed
    GOTO    _RESET	;button B is pressed
    TBLRD*+
    TBLRD*		;increment TBLPTR
    BTFSC   TABLAT,7	;the only case where this bit is set, is if TBLPTR
			;is either to high or too low
    CALL    _TOOFAR	;point TBLPTR at the 0's 7 seg display
    TBLRD*
    MOVFF   TABLAT,displayOut	;move display code to display
    CALL    _DELAY
    GOTO    _BUTTONCHECK
_DECREMENT: ;decrement the display
    BTFSS   buttonA	;check if button A is also pressed
    GOTO    _RESET	;button A is pressed
    TBLRD*-		;decrement TBLPTR
    TBLRD*
    BTFSC   TABLAT,7	;the only case where this bit is set, is if TBLPTR
			;is either to high or too low
    CALL    _TOOFAR	;point TBLPTR at the F's 7 seg code
    TBLRD*
    MOVFF   TABLAT,displayOut	;move display code to display
    CALL    _DELAY
    GOTO    _BUTTONCHECK

_TOOFAR:    ;TBLPTR has gone too far up or down
    MOVLW   0xF1	    ;marker for too low
    CPFSEQ  TABLAT
    MOVFF   tbltop,TBLPTRL  ;TBLPTR went too low
    MOVLW   0xF0	    ;marker for too high
    CPFSEQ  TABLAT
    MOVFF   tblbottom,TBLPTRL	;TBLPTR went too high
    RETURN
    

_DELAY:	;create an artificial delay, after updating the display
    MOVLW   0xFF
    MOVWF   delayREG	;outerloop reg
_DECDELAYA: ;outerloop
    MOVLW   0xFF
    MOVWF   delayREGB	;innerloop reg
_DECDELAYB: ;innerloop
    NOP
    NOP
    DECF    delayREGB, 1 ;decrement delayREGB, store result in delayREGB
    BNZ	    _DECDELAYB
    DECF    delayREG, 1	 ;innerloop has reached 0, repeat and dec outerloop once
    BNZ	    _DECDELAYA
    return		 ;innerloop and outerloop = 0

    
    ORG	    0x400   ;storing 7 seg display code in prog mem
    DB	    0xF0    ;Used for _TOOFAR
    DB	    0x3F    ;0
    DB	    0x06    ;1
    DB	    0x5B    ;2
    DB	    0x4F    ;3
    DB	    0x66    ;4
    DB	    0x6D    ;5
    DB	    0x7C    ;6
    DB	    0x07    ;7
    DB	    0x7F    ;8
    DB	    0x67    ;9
    DB	    0x77    ;A
    DB	    0x7F    ;B
    DB	    0x39    ;C
    DB	    0x3F    ;D
    DB	    0x79    ;E
    DB	    0x71    ;F
    DB	    0xF1    ;Used for _TOOFAR
    
    END